#pragma once 
#include "aeb/vehicle.hpp"

namespace aeb{
    double ttc(double gap, double closing);
    double ttc(const VehicleState& ego, const VehicleState& lead);
}