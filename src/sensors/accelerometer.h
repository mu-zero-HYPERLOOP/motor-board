#pragma once

#include "firmware/motor_board.h"
#include "util/metrics.h"
namespace sensors::accelerometer {

constexpr Frequency MEAS_FREQUENCY = 3.2_kHz;
constexpr motor_board::AccelerometerRange range = motor_board::ACCEL_RANGE_2G;

void begin();

void calibrate();

void update();

};
