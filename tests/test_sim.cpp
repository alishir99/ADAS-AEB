#include <gtest/gtest.h>
#include "aeb/sim.hpp"
#include "aeb/radar.hpp"

TEST(Sim,  CCRs50AvoidsCrash){
    aeb::VehicleState ego{0.0, aeb::kmh(50), 0.0};
    aeb::VehicleState lead{aeb::kmh(50)*4, 0.0, 0.0}; // standing still, 4 s ahead
    EXPECT_FALSE(aeb::run(ego, lead).collided);
}

TEST(Sim,  CCRs50WithoutAebCrashes){
    aeb::VehicleState ego{0.0, aeb::kmh(50), 0.0};
    aeb::VehicleState lead{aeb::kmh(50)*4, 0.0, 0.0};
    aeb::AebConfig off;
    off.warn_ttc = -1.0;  //disable all three triggers
    off.partial_ttc = -1.0;
    off.full_ttc = -1.0;

    aeb::SimResult r = aeb::run(ego, lead, off);
    EXPECT_TRUE(r.collided);
    EXPECT_NEAR(r.impact_speed, aeb::kmh(50), 0.1);
}

TEST(Sim, TraceRecordsEachStep) {
    aeb::VehicleState ego  {0, aeb::kmh(50), 0};
    aeb::VehicleState lead {aeb::kmh(50) * 4, 0, 0};
    aeb::SimResult r = aeb::run(ego, lead);

    ASSERT_FALSE(r.trace.empty());
    EXPECT_DOUBLE_EQ(r.trace.front().t, 0.0);              // first snapshot at t = 0
    EXPECT_EQ(r.trace.size(), 3000u);                       // no crash → runs the full 30 s at dt 0.01
    EXPECT_GT(r.trace.back().gap, 0.0);                    // never touched
}

static int braking_steps(const aeb::SimResult& r) {
    int n = 0;
    for (const aeb::Sample& p : r.trace)
        if (p.state >= aeb::Aebstate::PartialBrake) n++;
    return n;
}

TEST(Sim, PerfectRadarNeverBrakesInQueue) {
    aeb::VehicleState ego  {0.0, aeb::kmh(20), 0.0};
    aeb::VehicleState lead {5.0, aeb::kmh(20), 0.0};
    aeb::SimResult r = aeb::run(ego, lead);
    EXPECT_EQ(braking_steps(r), 0);
}

TEST(Sim, NoisyRadarCausesPhantomBraking) {
    aeb::VehicleState ego  {0.0, aeb::kmh(20), 0.0};
    aeb::VehicleState lead {5.0, aeb::kmh(20), 0.0};
    aeb::RadarConfig noisy {0.2, 1.0, 42};             // 20 cm, 1 m/s, seed
    aeb::SimResult r = aeb::run(ego, lead, {}, 0.01, 30.0, noisy);
    EXPECT_GT(braking_steps(r), 0);   
}   

TEST(Sim, KalmanStopsPhantomBraking) {
    aeb::VehicleState ego  {0.0, aeb::kmh(20), 0.0};
    aeb::VehicleState lead {5.0, aeb::kmh(20), 0.0};
    aeb::RadarConfig filtered {0.2, 1.0, 42, true};
    aeb::SimResult r = aeb::run(ego, lead, {}, 0.01, 30.0, filtered);
    EXPECT_EQ(braking_steps(r), 0);                 // smooth enough: no false alarms
}

TEST(Sim, KalmanStillAvoidsCCRb) {
    aeb::VehicleState ego  {0.0, aeb::kmh(50), 0.0};
    aeb::VehicleState lead {12.0, aeb::kmh(50), -6.0};   // hardest case: hard braking, close
    aeb::RadarConfig filtered {0.2, 1.0, 42, true};
    aeb::SimResult r = aeb::run(ego, lead, {}, 0.01, 30.0, filtered);
    EXPECT_FALSE(r.collided);                       // fast enough: still reacts to a real threat
}