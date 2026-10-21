#pragma once 
#include "aeb/vehicle.hpp"

namespace aeb{
    double ttc(const VehicleState& ego, const VehicleState& lead);
}