#include "aeb/aeb.hpp"

namespace aeb{
    Aebstate decide(double ttc, const AebConfig& cfg){
        if (ttc <= cfg.full_ttc) return Aebstate::FullBrake;
        if (ttc <= cfg.partial_ttc) return Aebstate::PartialBrake;
        if(ttc<=cfg.warn_ttc) return Aebstate::Warning;
        return Aebstate::Idle;
    }
}
