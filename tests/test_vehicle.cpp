#include <gtest/gtest.h>
#include <aeb/vehicle.hpp>

TEST(Vehicle, ConstatSpeed){
    aeb::VehicleState s{0.0, 10.0, 0.0}; // p, v, a 
    aeb::VehicleState next = aeb::step(s, 1.0);
    EXPECT_DOUBLE_EQ(next.position, 10.0);
}

TEST(Vehicle, Acce){
    aeb::VehicleState s{0.0, 0.0, 2.0}; // p, v, a 
    aeb::VehicleState next = aeb::step(s, 1.0);
    EXPECT_DOUBLE_EQ(next.position, 1);
    EXPECT_DOUBLE_EQ(next.velocity, 2);
}

TEST(Vehicle, BrakingStopsAtZero){
    aeb::VehicleState s{0.0, 2.0, -8.0}; // 2 m/s, braking hard
    aeb::VehicleState next = aeb::step(s, 1.0);
    EXPECT_DOUBLE_EQ(next.velocity, 0);
    EXPECT_DOUBLE_EQ(next.position, 0.25);
}