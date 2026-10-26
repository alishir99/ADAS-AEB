#pragma once 
#include"aeb/vehicle.hpp"
#include "aeb/aeb.hpp"
#include <vector>
#include "aeb/radar.hpp"

namespace aeb{

struct Sample {
    double t;
    double ego_v;
    double lead_v;
    double gap;
    double ttc;
    Aebstate state;
    double raw_closing;
    double est_closing; 
};

struct SimResult{
    bool collided = false;
    double impact_speed = 0.0;  // ms/s, how hard we hit (0 if no crash)
    std::vector<Sample> trace; 
};
// Drives both cars for `duration` seconds with AEB active.
// To make the lead car brake (CCRb), give it a negative acceleration.
SimResult run(VehicleState ego, VehicleState lead,
              const AebConfig& cfg = {}, double dt = 0.01, double duration = 30.0,
              const RadarConfig& radar_cfg = {});

}  // namespace aeb



