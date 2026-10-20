#pragma once

namespace aeb {
    struct VehicleState{
        double position;         // m
        double velocity;        // m/s
        double acceleration;    // m/s^2
    };

    // returns the state after dt seconds. Does not modify the input
    VehicleState step(const VehicleState& s, double dt);
} 

