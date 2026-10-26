#include <gtest/gtest.h>
#include <limits>
#include "aeb/ttc.hpp"

const double INF = std::numeric_limits<double>::infinity();

TEST(TTC, ClosingIn) {
    aeb::VehicleState ego {0.0,  20.0, 0.0};   // position, velocity, acceleration
    aeb::VehicleState lead{40.0, 10.0, 0.0};
    EXPECT_DOUBLE_EQ(aeb::ttc(ego, lead), 4);   // gap / closing speed = ?
}

TEST(TTC, SameSpeed) {
    aeb::VehicleState ego {0.0,  20.0, 0.0};   // position, velocity, acceleration
    aeb::VehicleState lead{40.0, 20.0, 0.0};
    EXPECT_DOUBLE_EQ(aeb::ttc(ego, lead), INF);
}

TEST(TTC, LeadFaster) {
    aeb::VehicleState ego {0.0,  20.0, 0.0};   // position, velocity, acceleration
    aeb::VehicleState lead{40.0, 30.0, 0.0};
    EXPECT_DOUBLE_EQ(aeb::ttc(ego, lead), INF);
}

TEST(TTC, AlreadyCollided) {
    aeb::VehicleState ego {45.0, 20.0, 0.0};
    aeb::VehicleState lead{40.0, 10.0, 0.0};
    EXPECT_DOUBLE_EQ(aeb::ttc(ego, lead), 0.0);
}

TEST(TTC, TouchingSameSpeed) {
    aeb::VehicleState ego {40.0, 20.0, 0.0};
    aeb::VehicleState lead {40.0, 20.0, 0.0};
    EXPECT_DOUBLE_EQ(aeb::ttc(ego, lead), 0.0);
    // you write it: both at position 40, both at 20 m/s → expect 0
}

TEST(TTC, FromGapAndClosing){
    EXPECT_DOUBLE_EQ(aeb::ttc(20.0, 5.0), 4.0);
}