#pragma once

#include "util/metrics.h"
namespace sensors::phase_current {

constexpr Frequency MEAS_FREQUENCY = 1_kHz;

constexpr float CURRENT_SENSE_GAIN = 20.0f;


void begin();

void calibrate();

Current convert_v1(Voltage);
Current convert_w1(Voltage);
Current convert_u1(Voltage);
Current convert_v2(Voltage);
Current convert_w2(Voltage);
Current convert_u2(Voltage);

void update();

}
