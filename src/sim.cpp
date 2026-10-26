#include "aeb/sim.hpp"
#include "aeb/ttc.hpp"
#include "aeb/radar.hpp"

namespace aeb{
static double brake_decel(Aebstate s, const AebConfig& cfg){
    switch (s)
    {
    case Aebstate::FullBrake:       return cfg.full_decel;
    case Aebstate::PartialBrake:    return cfg.partial_decel;
    case Aebstate::Warning:
    case Aebstate::Idle:            return 0.0; //a warning alone doesn't brake
    }
    return 0.0; 
}

SimResult run(VehicleState ego, VehicleState lead, const AebConfig& cfg, 
                double dt, double duration, const RadarConfig& radar_cfg){
    AebController ctrl(cfg);
    SimResult r;
    int steps = static_cast<int>(duration/dt); 
    Radar radar(radar_cfg);
    for (int i=0; i<steps; i++){
        Measurement m = radar.measure(ego, lead);
        double t = ttc(m.gap, m.closing);
        Aebstate s = ctrl.update(t);
        ego.acceleration = brake_decel(s, cfg);
        r.trace.push_back({ i * dt, ego.velocity, lead.velocity, (lead.position - ego.position), t, s });
        ego = step(ego, dt);
        lead = step(lead, dt);
        if(lead.position -ego.position <= 0){
            r.collided = true;
            r.impact_speed = ego.velocity-lead.velocity;
            return r;
        }

    }
    return r;
}
}

