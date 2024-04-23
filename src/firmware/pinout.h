#pragma once

#include <inttypes.h>

enum InverterPin : uint8_t {
  MUX_SEL0 = 25, //TODO put the correct pins here!
  MUX_SEL1 = 26,
  MUX_SEL2 = 27,
  ACCEL_CS = 10,
  CTRL_W2_HIGH = 6,
  CTRL_V1_LOW = 7,
  CTRL_V1_HIGH = 8,
  CTRL_V2_LOW = 9,
  CTRL_W1_LOW = 28,
  CTRL_W1_HIGH = 29,
  CTRL_U2_LOW = 37,
  CTRL_U2_HIGH = 36,
  CTRL_U1_LOW = 33,
  SDC_TRIG = 32,
  PRECHARGE_START = 23,
  PRECHARGE_DONE = 22,
  AIN_TEMP_MEAS = 21,
  AIN_MUX = AIN_TEMP_MEAS,
  AIN_VDC_MEAS = 20,
  AIN_I_MEAS_W2 = 19,
  AIN_I_MEAS_V2 = 18,
  AIN_I_MEAS_U2 = 17,
  AIN_I_MEAS_U1 = 16,
  AIN_I_MEAS_V1 = 15,
  AIN_I_MEAS_W1 = 14
};
