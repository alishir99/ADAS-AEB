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
    
}