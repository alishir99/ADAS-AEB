#include "aeb/vehicle.hpp"

namespace aeb {
    VehicleState step(const VehicleState& s, double dt){
        VehicleState next = s;
        next.velocity = s.velocity + s.acceleration*dt;
        next.position = s.position + s.velocity*dt + 0.5*s.acceleration*dt*dt;
        return next;

    }
}