#include "aeb/radar.hpp"

namespace aeb {
    Measurement Radar::measure(const VehicleState& ego, const VehicleState& lead){
        double gap     = lead.position - ego.position;          // true gap 
        double closing = ego.velocity - lead.velocity;          // true closing speed
        return { gap + cfg_.gap_sigma * noise_(rng_),
                 closing+cfg_.speed_sigma * noise_(rng_)};                // same idea for closing, with speed_sigma
    }
}