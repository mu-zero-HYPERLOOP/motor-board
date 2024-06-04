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
    return PwmControl{.duty20 = U1_duty,
                      .duty22 = V2_duty,
                      .duty23 = U2_duty,
                      .duty42 = W2_duty,
                      .duty31 = W1_duty,
                      .duty13 = V1_duty};
  }
};
