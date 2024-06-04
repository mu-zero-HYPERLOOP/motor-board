#pragma once

#include "util/metrics.h"
namespace sensors::ext_ntcs {

constexpr Frequency MEAS_FREQUENCY = 100_Hz;

constexpr Resistance R_MEAS = 1_kOhm;

constexpr Resistance PT_R0 = 1_kOhm;
constexpr float PT_ALPHA = 0.0004;

void begin();
void calibrate();
void update();
} // namespace sensors::ext_ntcs
