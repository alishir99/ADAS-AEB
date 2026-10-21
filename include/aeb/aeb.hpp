#pragma once 

namespace aeb {
    enum class Aebstate {Idle, Warning, PartialBrake, FullBrake};
    struct AebConfig {
        double warn_ttc  = 2.6;  //seconds
        double partial_ttc = 1.6;
        double full_ttc = 0.9;
    };

    // Picks the action for a given ttc
    Aebstate decide (double ttc, const AebConfig& cfg ={});
    
}