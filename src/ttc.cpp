#include "aeb/ttc.hpp"
#include <limits>

namespace aeb {
    const double INF = std::numeric_limits<double>::infinity();
    double ttc(const VehicleState& ego, const VehicleState& lead){
        double gap = lead.position - ego.position;
        double closing = (ego.velocity-lead.velocity);
        if (gap <=0){
            return 0.0;
        }
        else if(closing <=0){
            return INF;
        }
        else{
            return gap/closing;
        }
    }

}