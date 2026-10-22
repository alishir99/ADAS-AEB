#include <gtest/gtest.h>
#include <aeb/aeb.hpp>
#include <limits>

TEST(Decide, FarAwayIsIdle){
    EXPECT_EQ(aeb::decide(5.0), aeb::Aebstate::Idle);
}

TEST(Decide, ExactlyOnLineWarning){
    EXPECT_EQ(aeb::decide(2.6), aeb::Aebstate::Warning);
}

TEST(Decide, PartialBrake){
    EXPECT_EQ(aeb::decide(1.0), aeb::Aebstate::PartialBrake);
}

TEST(Decide, FullBrake){
    EXPECT_EQ(aeb::decide(0.5), aeb::Aebstate::FullBrake);
}

TEST(Decide, Collided){
    EXPECT_EQ(aeb::decide(0.0), aeb::Aebstate::FullBrake);
}

TEST(Decide, NoDangerIsIdle){
    double inf = std::numeric_limits<double>::infinity();
    EXPECT_EQ(aeb::decide(inf), aeb::Aebstate::Idle);
}

TEST(Decide, InsideWarningBand){
    EXPECT_EQ(aeb::decide(2.0), aeb::Aebstate::Warning);
}

TEST(Update, EscalatesImmediately){
    aeb::AebController c;
    EXPECT_EQ(c.update(5.0), aeb::Aebstate::Idle);
    EXPECT_EQ(c.update(0.5), aeb::Aebstate::FullBrake);
}

TEST(Update, HoldsThroughNoise){
    aeb::AebController c;
    EXPECT_EQ(c.update(1.5), aeb::Aebstate::PartialBrake);
    EXPECT_EQ(c.update(1.7), aeb::Aebstate::PartialBrake);
}

TEST(Update, ReleasesAboveMargin){
    aeb::AebController c;
    EXPECT_EQ(c.update(1.5), aeb::Aebstate::PartialBrake);
    EXPECT_EQ(c.update(2.0), aeb::Aebstate::Warning);
}