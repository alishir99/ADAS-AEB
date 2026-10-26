#include <gtest/gtest.h>
#include "aeb/kalman.hpp"

TEST(Kalman, StartsAtFirstMeasurement) {
    aeb::Kalman kf({40.0, 10.0}, 0.2, 1.0, 20.0);   // first reading, gap σ, speed σ, accel σ
    EXPECT_DOUBLE_EQ(kf.gap(), 40.0);
    EXPECT_DOUBLE_EQ(kf.closing(), 10.0);
}