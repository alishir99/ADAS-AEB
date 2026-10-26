#include <gtest/gtest.h>
#include "aeb/radar.hpp"

TEST(Radar, PerfectWhenNoNoise) {
    aeb::Radar radar;                                   // default: sigma 0
    aeb::VehicleState ego  {0.0, 20.0, 0.0};
    aeb::VehicleState lead {40.0, 10.0, 0.0};
    aeb::Measurement m = radar.measure(ego, lead);
    EXPECT_DOUBLE_EQ(m.gap, 40.0);
    EXPECT_DOUBLE_EQ(m.closing, 10.0);
}

TEST(Radar, SameSeedSameNoise) {
    aeb::RadarConfig cfg {0.5, 0.5, 7};                 // gap σ, speed σ, seed
    aeb::Radar a(cfg), b(cfg);
    aeb::VehicleState ego {0, 20, 0}, lead {40, 10, 0};
    EXPECT_DOUBLE_EQ(a.measure(ego, lead).gap, b.measure(ego, lead).gap);
}

TEST(Radar, NoiseAveragesToTruth) {
    aeb::Radar radar({1.0, 1.0, 42});
    aeb::VehicleState ego {0, 20, 0}, lead {40, 10, 0};
    double sum = 0.0;
    for (int i = 0; i < 10000; i++) sum += radar.measure(ego, lead).gap;
    EXPECT_NEAR(sum / 10000, 40.0, 0.05);   // noisy, but right onaverage
}