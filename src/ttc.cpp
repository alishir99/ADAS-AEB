#include "aeb/ttc.hpp"
#include <limits>

namespace aeb {
    const double INF = std::numeric_limits<double>::infinity();

    double ttc(double gap, double closing){
        if (gap <=0)    return 0.0;
        else if (closing <= 0)  return INF;
        else    return gap/closing;
    }
    double ttc(const VehicleState& ego, const VehicleState& lead){
        double gap = lead.position - ego.position;
        double clsoing = ego.velocity - lead.velocity;
        return ttc(gap, clsoing);
    }



}