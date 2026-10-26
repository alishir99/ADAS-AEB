#include <gtest/gtest.h>
#include "aeb/kalman.hpp"
#include <cmath>
#include <algorithm>
#include "aeb/radar.hpp"

TEST(Kalman, EqualTrustMeetsHalfway) {
    aeb::Kalman kf({40.0, 10.0}, 0.2, 1.0, 20.0);
    kf.correct({41.0, 10.0});              // radar says 41; filter and radar are equally unsure
     EXPECT_NEAR(kf.gap(), 40.5, 1e-9);   
}

TEST(Kalman, SmoothsNoisyRadar) {
    aeb::Radar radar({0.2, 1.0, 42});
    aeb::VehicleState ego {0, 10, 0}, lead {20, 10, 0};
    aeb::Kalman kf(radar.measure(ego, lead), 0.2, 1.0, 20.0);
    double worst_raw = 0, worst_filtered = 0;
    for (int i = 0; i < 1000; i++) {
         aeb::Measurement m = radar.measure(ego, lead);
        kf.predict(0.01, 0.0);
        kf.correct(m);
        if (i >= 100) {
            worst_raw      = std::max(worst_raw, std::abs(m.closing));
            worst_filtered = std::max(worst_filtered, std::abs(kf.closing()));
        }
    }
    EXPECT_LT(worst_filtered, worst_raw / 2);               // at least twice as calm
}

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