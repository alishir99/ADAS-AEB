#include <gtest/gtest.h>
#include "aeb/kalman.hpp"

TEST(Kalman, StartsAtFirstMeasurement) {
    aeb::Kalman kf({40.0, 10.0}, 0.2, 1.0, 20.0);   // first reading, gap σ, speed σ, accel σ
    EXPECT_DOUBLE_EQ(kf.gap(), 40.0);
    EXPECT_DOUBLE_EQ(kf.closing(), 10.0);
}

TEST(Kalman, PredictMovesWithPhysics) {
    aeb::Kalman kf({40.0, 10.0}, 0.2, 1.0, 20.0);
    kf.predict(0.1, 0.0);                       // 0.1 s, no braking
    EXPECT_NEAR(kf.gap(), 39, 1e-9);          // 40 m, closing at 10 m/s for 0.1 s → ?
    EXPECT_NEAR(kf.closing(), 10.0, 1e-9);
}

TEST(Kalman, PredictIncludesOwnBraking) {
    aeb::Kalman kf({40.0, 10.0}, 0.2, 1.0, 20.0);
    kf.predict(0.1, -4.0);                      // we brake at -4 m/s² for 0.1 s
    EXPECT_NEAR(kf.closing(), 9.6, 1e-9);      // 10 + (-4)(0.1) 
    EXPECT_NEAR(kf.gap(), 39.02, 1e-9);         // 40 − 10(0.1) − ½(−4)(0.1)²
}