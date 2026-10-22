#include "aeb/aeb.hpp"

namespace aeb{
    Aebstate decide(double ttc, const AebConfig& cfg){
        if (ttc <= cfg.full_ttc) return Aebstate::FullBrake;
        if (ttc <= cfg.partial_ttc) return Aebstate::PartialBrake;
        if(ttc<=cfg.warn_ttc) return Aebstate::Warning;
        return Aebstate::Idle;
    }
    static double threshold(Aebstate s, const AebConfig& cfg){
        switch (s){
            case Aebstate::FullBrake:       return cfg.full_ttc;
            case Aebstate::PartialBrake:    return cfg.partial_ttc;
            case Aebstate::Warning:         
            case Aebstate::Idle:            return cfg.warn_ttc;
        }
        return cfg.warn_ttc;
    }

    Aebstate AebController::update(double ttc){
        Aebstate wanted = decide(ttc, cfg_);
        if(wanted >= state_){
            state_ = wanted;
        }
        else {
            if(ttc > threshold(state_, cfg_) +cfg_.hysteresis){
                state_ = wanted;
            }
        }
        return state_;
    }
}
