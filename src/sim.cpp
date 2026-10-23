#include "aeb/sim.hpp"
#include "aeb/ttc.hpp"

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
                double dt, double duration){
    AebController ctrl(cfg);
    SimResult r;
    int steps = static_cast<int>(duration/dt); 

    for (int i=0; i<steps; i++){
        Aebstate s = ctrl.update(ttc(ego, lead));
        ego.acceleration = brake_decel(s, cfg);
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

