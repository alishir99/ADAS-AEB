# ADAS-AEB: Automatic Emergency Braking in C++17
![CI](https://github.com/alishir99/ADAS-AEB/actions/workflows/ci.yml/badge.svg)


A simulator of **Forward Collision Warning (FCW)** and **Automatic Emergency
Braking (AEB)**, the car safety feature that warns the driver and brakes by
itself to avoid hitting the car ahead.

> **Status: in progress.** Built step by step, test-first, with GoogleTest.

## How it works

1. **Vehicle model:** each car moves with constant-acceleration physics.
   A braking car stops at zero speed and never rolls backwards.
2. **Time-to-collision (TTC):** seconds until impact if nothing changes:
   `gap / closing speed`. Infinity when not closing in, 0 when already touching.
3. **Decision logic:** TTC thresholds pick the action:

   | TTC         | Action        |
   |-------------|---------------|
   | > 2.6 s     | Nothing       |
   | ≤ 2.6 s     | Warn driver   |
   | ≤ 1.6 s     | Partial brake |
   | ≤ 0.9 s     | Full brake    |

## Euro NCAP results

Simulated against the Euro NCAP rear-end tests used to rate new cars
(start distance = 4 s before impact):

- **CCRs:** car ahead is standing still
- **CCRm:** car ahead drives slowly at 20 km/h
- **CCRb:** both at 50 km/h, then the car ahead brakes hard

| Test | Speeds | Result |
|------|--------|--------|
| CCRs | 10–70 km/h | Avoided all 7 |
| CCRs | 80 km/h | Hit at 24 km/h (speed cut by 70%) |
| CCRm | 30, 50, 80 km/h | Avoided all 3 |
| CCRb | 12 / 40 m gap, −2 / −6 m/s² | Avoided all 4 |

**14 of 15 avoided.** At 80 km/h, full braking at TTC 0.9 s starts too late
to stop completely, so the crash is only softened. Run `./build/ncap` to reproduce.

## Roadmap

- [x] CMake + GoogleTest setup
- [x] Vehicle kinematics with braking clamp
- [x] Time-to-collision with edge cases (collision, equal speed, lead faster)
- [x] Threshold-based warn / brake decision
- [x] Hysteresis so sensor noise can't make the brakes flicker
- [x] Euro NCAP rear-end scenarios (CCRs, CCRm, CCRb) with pass/fail
- [ ] Plots of speed, gap and TTC over time
- [ ] Noisy radar + Kalman filter
- [x] GitHub Actions CI

## Build and test

Requires CMake ≥ 3.20 and a C++17 compiler.

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```
