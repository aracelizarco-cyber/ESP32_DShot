import time
from typing import List

import pytest


@pytest.mark.stress
def test_latency(tester, fan_indices: List[int], thresholds: dict):
    idx = fan_indices[0]
    res = tester.scenario_latency(idx, iterations=10)
    assert res["first_p95"] <= thresholds["lat_first_p95"], f"first_p95 too high: {res}"
    assert res["settle_p95"] <= thresholds["lat_settle_p95"], f"settle_p95 too high: {res}"


@pytest.mark.stress
def test_toggle_storm(tester, fan_indices: List[int]):
    idx = fan_indices[0]
    res = tester.scenario_toggle_storm(idx, cycles=50, interval=0.05)
    assert res["flips"] == 100


@pytest.mark.stress
def test_spam(tester, fan_indices: List[int], rate: int, duration: int):
    idx = fan_indices[0]
    res = tester.scenario_spam(idx, rate_hz=rate, duration_s=duration)
    assert res["sent"] >= int(0.8 * rate * duration)  # allow some slack


@pytest.mark.stress
def test_multi_mix(tester, fan_indices: List[int], rate: int, duration: int):
    res = tester.scenario_multi_mix(rate_hz=rate, duration_s=duration)
    assert res["sent"] >= int(0.8 * rate * duration * len(fan_indices))


@pytest.mark.stress
def test_invalid_payloads(tester, fan_indices: List[int]):
    idx = fan_indices[0]
    res = tester.scenario_invalid(idx)
    assert res["sent"] >= 2


@pytest.mark.stress
def test_rmt_refresh_health(tester, fan_indices: List[int], thresholds: dict):
    res = tester.scenario_rmt_check()
    # each fan delta should be within threshold
    for idx in fan_indices:
        key = f"fan{idx}"
        assert res.get(key, 0) <= thresholds["rmt_refresh_max"], f"High rmt_refresh_count for {key}: {res}"
