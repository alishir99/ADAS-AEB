#include "aeb/kalman.hpp"

namespace aeb{
    Kalman::Kalman(Measurement first, double gap_sigma, double speed_sigma, double accel_sigma)
    : r_gap_(gap_sigma*gap_sigma), r_speed_(speed_sigma*speed_sigma), q_(accel_sigma*accel_sigma){
    x_[0] = first.gap;
    x_[1] = first.closing;
    P_[0][0] = r_gap_; 
    P_[1][1] = r_speed_;
    P_[0][1] = P_[1][0] = 0.0;
    }

    void Kalman::predict(double dt, double ego_accel){
        x_[0] =x_[0] -x_[1]*dt - 0.5*ego_accel*dt*dt;
        x_[1] = x_[1] + ego_accel*dt;
        double a = P_[0][0], b = P_[0][1], d = P_[1][1];
        double dt2 = dt * dt;
        P_[0][0] = a - 2*dt*b + dt2*d + q_*dt2*dt2/4;
        P_[0][1] = P_[1][0] = b - dt*d - q_*dt2*dt/2;
        P_[1][1] = d + q_*dt2;
    }
    
}