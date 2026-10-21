#include "aeb/vehicle.hpp"
#include <cmath>

namespace aeb {
    VehicleState step(const VehicleState& s, double dt){
        VehicleState next = s;
        double v_new = s.velocity + s.acceleration *dt;
        if (v_new < 0.0){
            next.velocity = 0; 
            next.position = s.position+ s.velocity*s.velocity/(2*std::abs(s.acceleration));
        }
        else{
        next.velocity = v_new;
        next.position = s.position + s.velocity*dt + 0.5*s.acceleration*dt*dt;
        }
        return next;

    }
}