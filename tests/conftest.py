import os
import sys
from pathlib import Path
from typing import Generator, List, Optional, Tuple

import pytest


def pytest_addoption(parser: pytest.Parser) -> None:
    group = parser.getgroup("greenhouse_esp")
    group.addoption("--broker", action="store", default=os.getenv("MQTT_BROKER", "mqtt://127.0.0.1:1883"),
                    help="MQTT broker URI, e.g. mqtt://host:1883 or mqtts://host:8883")
    group.addoption("--mqtt-username", action="store", default=os.getenv("MQTT_USERNAME"))
    group.addoption("--mqtt-password", action="store", default=os.getenv("MQTT_PASSWORD"))
    group.addoption("--topic-prefix", action="store", default=os.getenv("MQTT_TOPIC_PREFIX", "greenhouse_esp"))
    group.addoption("--fan-start", action="store", type=int, default=int(os.getenv("FAN_INDEX_START", "1")))
    group.addoption("--fan-count", action="store", type=int, default=int(os.getenv("FAN_COUNT", "1")))
    group.addoption("--rate", action="store", type=int, default=int(os.getenv("STRESS_RATE", "30")))
    group.addoption("--duration", action="store", type=int, default=int(os.getenv("STRESS_DURATION", "15")))
    # Thresholds
    group.addoption("--latency-first-p95", action="store", type=float, default=float(os.getenv("LAT_FIRST_P95", "1.5")),
                    help="Max acceptable p95 latency (s) for first update")
    group.addoption("--latency-settle-p95", action="store", type=float, default=float(os.getenv("LAT_SETTLE_P95", "6.0")),
                    help="Max acceptable p95 latency (s) to reach target")
    group.addoption("--rmt-refresh-max", action="store", type=int, default=int(os.getenv("RMT_REFRESH_MAX_5S", "5")),
                    help="Max acceptable rmt_refresh_count delta per 5s window")


@pytest.fixture(scope="session")
def broker(request: pytest.FixtureRequest) -> str:
    return str(request.config.getoption("--broker"))


@pytest.fixture(scope="session")
def mqtt_credentials(request: pytest.FixtureRequest) -> Tuple[Optional[str], Optional[str]]:
    return (
        request.config.getoption("--mqtt-username"),
        request.config.getoption("--mqtt-password"),
    )


@pytest.fixture(scope="session")
def topic_prefix(request: pytest.FixtureRequest) -> str:
    return str(request.config.getoption("--topic-prefix")).rstrip('/')


@pytest.fixture(scope="session")
def fan_indices(request: pytest.FixtureRequest) -> List[int]:
    start = int(request.config.getoption("--fan-start"))
    count = int(request.config.getoption("--fan-count"))
    return list(range(start, start + count))


@pytest.fixture(scope="session")
def rate(request: pytest.FixtureRequest) -> int:
    return int(request.config.getoption("--rate"))


@pytest.fixture(scope="session")
def duration(request: pytest.FixtureRequest) -> int:
    return int(request.config.getoption("--duration"))


@pytest.fixture(scope="session")
def thresholds(request: pytest.FixtureRequest) -> dict:
    return {
        "lat_first_p95": float(request.config.getoption("--latency-first-p95")),
        "lat_settle_p95": float(request.config.getoption("--latency-settle-p95")),
        "rmt_refresh_max": int(request.config.getoption("--rmt-refresh-max")),
    }


@pytest.fixture(scope="session")
def tester(broker: str, mqtt_credentials, topic_prefix: str, fan_indices: List[int]):
    # Add tools to path and import StressTester
    repo_root = Path(__file__).resolve().parents[1]
    tools_dir = repo_root / "tools"
    sys.path.insert(0, str(tools_dir))
    from stress_test import StressTester  # type: ignore

    username, password = mqtt_credentials
    t = StressTester(broker_uri=broker, username=username, password=password,
                     topic_prefix=topic_prefix, fan_indices=fan_indices)
    t.connect()
    yield t
    t.disconnect()
