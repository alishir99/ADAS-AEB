#include <gtest/gtest.h>
#include "aeb/sim.hpp"

constexpr double kmh(double v){ return v/3.6;} // km/h -> m/s

TEST(Sim,  CCRs50AvoidsCrash){
    aeb::VehicleState ego{0.0, kmh(50), 0.0};
    aeb::VehicleState lead{kmh(50)*4, 0.0, 0.0}; // standing still, 4 s ahead
    EXPECT_FALSE(aeb::run(ego, lead).collided);
}

TEST(Sim,  CCRs50WithoutAebCrashes){
    aeb::VehicleState ego{0.0, kmh(50), 0.0};
    aeb::VehicleState lead{kmh(50)*4, 0.0, 0.0};
    aeb::AebConfig off;
    off.warn_ttc = -1.0;  //disable all three triggers
    off.partial_ttc = -1.0;
    off.full_ttc = -1.0;

    aeb::SimResult r = aeb::run(ego, lead, off);
    EXPECT_TRUE(r.collided);
    EXPECT_NEAR(r.impact_speed, kmh(50), 0.1);
}