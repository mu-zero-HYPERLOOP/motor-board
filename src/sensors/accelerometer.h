#pragma once

#include "util/metrics.h"
#include "firmware/motor_board.h"

namespace sensors::accelerometer {
  
constexpr Frequency MEAS_FREQUENCY = 3.2_kHz;
constexpr motor_board::AccelerometerRange range = motor_board::ACCEL_RANGE_2G;
void begin();
void calibrate();
void update();
}
