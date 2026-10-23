#pragma once 

namespace aeb {
    enum class Aebstate {Idle, Warning, PartialBrake, FullBrake};
    struct AebConfig {
        double warn_ttc  = 2.6;  //seconds
        double partial_ttc = 1.6;
        double full_ttc = 0.9;
        double hysteresis = 0.3; 
        double partial_decel = -4.0;  // m/s^2
        double full_decel    = -9.0; 
    };

    // Picks the action for a given ttc
    Aebstate decide (double ttc, const AebConfig& cfg ={});

    class AebController {
    public:
        explicit AebController(AebConfig cfg = {}): cfg_(cfg) {} 
        Aebstate update(double ttc);
        Aebstate state() const {return state_;}
        
    private:
        AebConfig cfg_;
        Aebstate state_ = Aebstate::Idle;
    };
}