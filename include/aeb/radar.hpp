#pragma once
#include <random>
#include "aeb/vehicle.hpp"

namespace aeb {

struct RadarConfig {
    double gap_sigma   = 0.0;    // m
    double speed_sigma = 0.0;    // m/s
    unsigned seed      = 42;
    bool   kalman      = false;   // smooth readings with a Kalman filter
    double accel_sigma = 20.0;    // filter tuning: how suddenly the car ahead may change speed
};
struct Measurement { double gap; double closing; };

class Radar {
public:
    explicit Radar(RadarConfig cfg = {}) : cfg_(cfg), rng_(cfg.seed) {}
    Measurement measure(const VehicleState& ego, const VehicleState& lead);
private:
    RadarConfig cfg_;
    std::mt19937 rng_;
    std::normal_distribution<double> noise_{0.0, 1.0};
};

}