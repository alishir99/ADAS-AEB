#pragma once

namespace aeb {
    constexpr double kmh(double v) { return v / 3.6; }   // km/h → m/s
    struct VehicleState{
        double position;         // m
        double velocity;        // m/s
        double acceleration;    // m/s^2
    };

    // returns the state after dt seconds. Does not modify the input
    VehicleState step(const VehicleState& s, double dt);
} 

