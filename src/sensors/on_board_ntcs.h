#pragma once

#include "util/metrics.h"
namespace sensors::on_board_ntcs {

constexpr Frequency MEAS_FREQUENCY = 100_Hz;

constexpr Resistance R_MEAS = 1_kOhm;

constexpr Temperature NTC_BETA = 3400_K;
constexpr Resistance NTC_R_REF = 10_kOhm;
constexpr Temperature NTC_T_REF = 25_Celcius;

void begin();
void calibrate();
void update();
}
