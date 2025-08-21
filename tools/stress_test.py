#!/usr/bin/env python3
"""
MQTT stress and resilience tester for greenhouse_esp DShot fan controller.

Scenarios:
- latency: measure time from command -> first percentage update, and to reach target
- toggle_storm: rapid ON/OFF publishes
- spam: high-rate random percentage commands
- multi_mix: concurrent random control across multiple fans
- invalid: send invalid payloads and out-of-range values
- rmt_check: sample rmt_refresh_count before/after to flag high refresh rates

Usage example:
  python tools/stress_test.py --broker mqtt://192.168.53.174:1883 \
      --username greenhouse_esp --password 1LoveW33D \
      --fan-start 1 --fan-count 2 --scenarios latency,spam,toggle,multi_mix,invalid,rmt_check \
      --duration 30
"""
import argparse
import logging
import random
import threading
import time
from collections import defaultdict, deque
from dataclasses import dataclass
from typing import Dict, List, Optional, Tuple

try:
    import paho.mqtt.client as mqtt  # type: ignore
except Exception as e:  # pragma: no cover
    raise SystemExit("Missing dependency paho-mqtt. Install with: pip install -r tools/requirements.txt") from e


def parse_broker_uri(uri: str) -> Tuple[str, int, bool]:
    # Simple parser for mqtt://host:port and mqtts://host:port
    if uri.startswith("mqtt://"):
        secure = False
        hostport = uri[len("mqtt://") :]
    elif uri.startswith("mqtts://"):
        secure = True
        hostport = uri[len("mqtts://") :]
    else:
        # Assume host[:port]
        secure = False
        hostport = uri
    if ":" in hostport:
        host, port_s = hostport.split(":", 1)
        try:
            port = int(port_s)
        except ValueError:
            port = 8883 if secure else 1883
    else:
        host = hostport
        port = 8883 if secure else 1883
    return host, port, secure


@dataclass
class FanState:
    percentage: int = 0
    state: str = "OFF"
    rmt_refresh_count: int = 0
    last_update_ts: float = 0.0


class StressTester:
    def __init__(self, *, broker_uri: str, username: Optional[str], password: Optional[str],
                 topic_prefix: str, fan_indices: List[int], keepalive: int = 30) -> None:
        self.topic_prefix = topic_prefix.rstrip('/')
        self.fan_indices = fan_indices
        self.host, self.port, self.secure = parse_broker_uri(broker_uri)
        self.client = mqtt.Client(client_id=f"stress_{int(time.time())}")
        if username:
            self.client.username_pw_set(username, password)
        self.client.on_connect = self._on_connect
        self.client.on_message = self._on_message
        # paho v1 keepalive seconds
        self.keepalive = keepalive
        self._conn_event = threading.Event()
        self._stop = threading.Event()
        self.states: Dict[int, FanState] = defaultdict(FanState)
        self._cond = threading.Condition()
        self._subscriptions_done = threading.Event()
        self._msg_queue: deque[Tuple[str, str]] = deque(maxlen=1000)

    # MQTT callbacks
    def _on_connect(self, client, userdata, flags, rc):  # type: ignore[no-untyped-def]
        if rc == 0:
            logging.info("Connected to MQTT %s:%d", self.host, self.port)
            # Subscribe to state/percentage/metrics
            for idx in self.fan_indices:
                self.client.subscribe(f"{self.topic_prefix}/fan{idx}/state", qos=1)
                self.client.subscribe(f"{self.topic_prefix}/fan{idx}/percentage", qos=1)
                self.client.subscribe(f"{self.topic_prefix}/fan{idx}/rmt_refresh_count", qos=1)
            # Availability (wildcard, we don't know MAC suffix)
            self.client.subscribe(f"{self.topic_prefix}/status/+", qos=1)
            self._subscriptions_done.set()
            self._conn_event.set()
        else:
            logging.error("MQTT connect failed rc=%s", rc)

    def _on_message(self, client, userdata, msg):  # type: ignore[no-untyped-def]
        topic = msg.topic
        payload = msg.payload.decode('utf-8', errors='ignore')
        ts = time.time()
        self._msg_queue.append((topic, payload))
        for idx in self.fan_indices:
            if topic == f"{self.topic_prefix}/fan{idx}/percentage":
                try:
                    pct = int(payload)
                except ValueError:
                    pct = -1
                with self._cond:
                    st = self.states[idx]
                    st.percentage = pct
                    st.last_update_ts = ts
                    self._cond.notify_all()
                return
            if topic == f"{self.topic_prefix}/fan{idx}/state":
                with self._cond:
                    st = self.states[idx]
                    st.state = payload
                    st.last_update_ts = ts
                    self._cond.notify_all()
                return
            if topic == f"{self.topic_prefix}/fan{idx}/rmt_refresh_count":
                try:
                    cnt = int(payload)
                except ValueError:
                    cnt = -1
                with self._cond:
                    st = self.states[idx]
                    st.rmt_refresh_count = cnt
                    st.last_update_ts = ts
                    self._cond.notify_all()
                return

    # Connection helpers
    def connect(self) -> None:
        if self.secure:
            try:
                self.client.tls_set()  # use system CAs
            except Exception:
                logging.warning("TLS requested but default CA load failed; proceeding insecurely")
        self.client.connect(self.host, self.port, keepalive=self.keepalive)
        self.client.loop_start()
        if not self._conn_event.wait(10):
            raise TimeoutError("Timeout waiting for MQTT connect")
        if not self._subscriptions_done.wait(5):
            logging.warning("Subscriptions not confirmed; continuing anyway")

    def disconnect(self) -> None:
        self._stop.set()
        self.client.loop_stop()
        self.client.disconnect()

    # Publish helpers
    def pub_pct(self, idx: int, pct: int, qos: int = 0) -> None:
        topic = f"{self.topic_prefix}/fan{idx}/percentage/set"
        self.client.publish(topic, str(pct), qos=qos, retain=False)

    def pub_set(self, idx: int, on: bool, qos: int = 0) -> None:
        topic = f"{self.topic_prefix}/fan{idx}/set"
        self.client.publish(topic, "ON" if on else "OFF", qos=qos, retain=False)

    # Wait until percentage satisfies predicate
    def wait_pct(self, idx: int, pred, timeout: float) -> bool:
        deadline = time.time() + timeout
        with self._cond:
            while time.time() < deadline:
                pct = self.states[idx].percentage
                if pred(pct):
                    return True
                remaining = max(0.0, deadline - time.time())
                self._cond.wait(timeout=min(0.2, remaining))
        return False

    # Scenarios
    def scenario_latency(self, idx: int, iterations: int = 10) -> Dict[str, float]:
        """Measure latency to first update and to reach target percentage."""
        first_latencies: List[float] = []
        settle_latencies: List[float] = []
        for _ in range(iterations):
            target = random.randint(0, 100)
            t0 = time.time()
            self.pub_pct(idx, target, qos=0)
            # First update (any change)
            got_first = self.wait_pct(idx, lambda p: p >= 0, timeout=3.0)
            t_first = time.time() - t0 if got_first else float('inf')
            # Settle when equals target (allow +/-1 due to rounding)
            got_settle = self.wait_pct(idx, lambda p: p != -1 and abs(p - target) <= 1, timeout=8.0)
            t_settle = time.time() - t0 if got_settle else float('inf')
            first_latencies.append(t_first)
            settle_latencies.append(t_settle)
        return {
            "first_p50": percentile(first_latencies, 50),
            "first_p95": percentile(first_latencies, 95),
            "settle_p50": percentile(settle_latencies, 50),
            "settle_p95": percentile(settle_latencies, 95),
        }

    def scenario_toggle_storm(self, idx: int, cycles: int = 50, interval: float = 0.05) -> Dict[str, int]:
        flips = 0
        for i in range(cycles):
            self.pub_set(idx, True, qos=0)
            self.pub_set(idx, False, qos=0)
            flips += 2
            time.sleep(interval)
        return {"flips": flips}

    def scenario_spam(self, idx: int, rate_hz: int = 50, duration_s: int = 10) -> Dict[str, int]:
        count = 0
        period = 1.0 / max(1, rate_hz)
        end = time.time() + duration_s
        while time.time() < end:
            self.pub_pct(idx, random.randint(0, 100), qos=0)
            count += 1
            # Busy-loop-ish but sleep a bit to avoid zero-delay
            time.sleep(period)
        return {"sent": count}

    def scenario_multi_mix(self, rate_hz: int = 20, duration_s: int = 15) -> Dict[str, int]:
        total = 0
        period = 1.0 / max(1, rate_hz)
        end = time.time() + duration_s
        while time.time() < end:
            for idx in self.fan_indices:
                if random.random() < 0.3:
                    self.pub_set(idx, random.choice([True, False]), qos=0)
                else:
                    self.pub_pct(idx, random.randint(0, 100), qos=0)
                total += 1
            time.sleep(period)
        return {"sent": total}

    def scenario_invalid(self, idx: int) -> Dict[str, int]:
        bad_payloads = ["banana", "-1", "101", "", "NaN", " ", "255", "-9999"]
        count = 0
        for p in bad_payloads:
            self.client.publish(f"{self.topic_prefix}/fan{idx}/percentage/set", p, qos=0, retain=False)
            self.client.publish(f"{self.topic_prefix}/fan{idx}/set", p, qos=0, retain=False)
            count += 2
        return {"sent": count}

    def scenario_rmt_check(self) -> Dict[str, int]:
        before: Dict[int, int] = {}
        after: Dict[int, int] = {}
        # Wait a bit to receive baseline metrics
        time.sleep(1.0)
        with self._cond:
            for idx in self.fan_indices:
                before[idx] = self.states[idx].rmt_refresh_count
        # Let system run a little
        time.sleep(5.0)
        with self._cond:
            for idx in self.fan_indices:
                after[idx] = self.states[idx].rmt_refresh_count
        deltas = {idx: max(0, after.get(idx, 0) - before.get(idx, 0)) for idx in self.fan_indices}
        # Warn if high refresh rate (> 5 per 5s)
        for idx, d in deltas.items():
            if d > 5:
                logging.warning("High rmt_refresh_count increment for fan%d: %d in 5s", idx, d)
        return {f"fan{idx}": d for idx, d in deltas.items()}


def percentile(values: List[float], p: float) -> float:
    vals = [v for v in values if v != float('inf')]
    if not vals:
        return float('inf')
    vals.sort()
    k = max(0, min(len(vals) - 1, int(round((p / 100.0) * (len(vals) - 1)))))
    return vals[k]


def main() -> None:
    ap = argparse.ArgumentParser(description="MQTT stress test for greenhouse_esp")
    ap.add_argument("--broker", required=True, help="Broker URI (e.g., mqtt://host:1883 or mqtts://host:8883)")
    ap.add_argument("--username", default=None)
    ap.add_argument("--password", default=None)
    ap.add_argument("--topic-prefix", default="greenhouse_esp")
    ap.add_argument("--fan-start", type=int, default=1)
    ap.add_argument("--fan-count", type=int, default=1)
    ap.add_argument("--scenarios", default="latency,spam,toggle,multi_mix,invalid,rmt_check",
                    help="Comma-separated scenarios to run")
    ap.add_argument("--duration", type=int, default=20, help="Base duration for time-based scenarios")
    ap.add_argument("--rate", type=int, default=30, help="Base rate (Hz) for spam/multi_mix")
    ap.add_argument("--log-level", default="INFO")
    args = ap.parse_args()

    logging.basicConfig(level=getattr(logging, args.log_level.upper(), logging.INFO),
                        format="%(asctime)s %(levelname)s %(message)s")

    fan_indices = list(range(args.fan_start, args.fan_start + args.fan_count))
    tester = StressTester(broker_uri=args.broker, username=args.username, password=args.password,
                          topic_prefix=args.topic_prefix, fan_indices=fan_indices)
    tester.connect()
    logging.info("Running scenarios on fans %s", fan_indices)

    results: Dict[str, object] = {}
    try:
        for scen in [s.strip() for s in args.scenarios.split(',') if s.strip()]:
            if scen == "latency":
                res = tester.scenario_latency(fan_indices[0], iterations=10)
                logging.info("latency: %s", res)
                results[scen] = res
            elif scen == "toggle":
                res = tester.scenario_toggle_storm(fan_indices[0], cycles=50, interval=0.05)
                logging.info("toggle: %s", res)
                results[scen] = res
            elif scen == "spam":
                res = tester.scenario_spam(fan_indices[0], rate_hz=args.rate, duration_s=args.duration)
                logging.info("spam: %s", res)
                results[scen] = res
            elif scen == "multi_mix":
                res = tester.scenario_multi_mix(rate_hz=args.rate, duration_s=args.duration)
                logging.info("multi_mix: %s", res)
                results[scen] = res
            elif scen == "invalid":
                res = tester.scenario_invalid(fan_indices[0])
                logging.info("invalid: %s", res)
                results[scen] = res
            elif scen == "rmt_check":
                res = tester.scenario_rmt_check()
                logging.info("rmt_check: %s", res)
                results[scen] = res
            else:
                logging.warning("Unknown scenario '%s'", scen)
    finally:
        tester.disconnect()

    # Summary
    print("\n==== Stress Test Summary ====")
    for k, v in results.items():
        print(f"{k}: {v}")


if __name__ == "__main__":
    main()
