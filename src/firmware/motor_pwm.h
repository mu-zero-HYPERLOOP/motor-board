#pragma once

#include "firmware/pwm.h"

struct MotorPwmControl {
  float U1_duty = 0.5; // range [0,1]
  float V2_duty = 0.5;
  float U2_duty = 0.5;
  float W2_duty = 0.5;
  float W1_duty = 0.5;
  float V1_duty = 0.5;
  constexpr operator PwmControl() const {
    return PwmControl(U1_duty, V2_duty, U2_duty, W2_duty, W1_duty, V1_duty);
  }
};
