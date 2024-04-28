

#include "firmware/pwm.hpp"
struct MotorPwmControl {
  float U1_duty; // range [0,1]
  float V2_duty;
  float U2_duty;
  float W2_duty;
  float W1_duty;
  float V1_duty;
  constexpr operator PwmControl() const {
    return PwmControl{.duty20 = U1_duty,
                      .duty22 = V2_duty,
                      .duty23 = U2_duty,
                      .duty42 = W2_duty,
                      .duty31 = W1_duty,
                      .duty13 = V1_duty};
  }
};
