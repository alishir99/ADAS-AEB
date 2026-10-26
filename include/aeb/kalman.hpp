#pragma once
#include "aeb/radar.hpp"     // for Measurement

namespace aeb {

class Kalman {
public:
    Kalman(Measurement first, double gap_sigma, double speed_sigma, double accel_sigma);
    void predict(double dt, double ego_accel);
    void correct(Measurement m);
    double gap() const     { return x_[0]; }
    double closing() const { return x_[1]; }
private:
    void correct_one(int i, double z, double r);
    double x_[2];          // the guess: [0] gap, [1] closing speed
    double P_[2][2];       // how unsure the guess is (a 2x2 table)
    double r_gap_;         // radar gap noise², how much to distrust the radar
    double r_speed_;       // radar speed noise²
    double q_;             // accel noise², how much to distrust the physics
};

}