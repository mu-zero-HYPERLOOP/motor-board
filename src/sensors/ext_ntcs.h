#pragma once

#include "util/metrics.h"
namespace sensors::ext_ntcs {

constexpr Frequency MEAS_FREQUENCY = 100_Hz;

constexpr Resistance R_MEAS = 1_kOhm;

constexpr Temperature NTC_BETA = 3900_K;
constexpr Temperature NTC_T_REF = 25_Celcius;
constexpr Resistance NTC_R_REF = 10_kOhm;

void begin();
void calibrate();
void update();
} // namespace sensors::ext_ntcs
