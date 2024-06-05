#include "control.h"
#include "firmware/motor_pwm.h"
#include "util/lina.h"
#include <cmath>
#include <complex>
#include "canzero/canzero.h"
#include "print.h"

void control::begin() {
}



static Frequency rotational_frequency = 1_Hz;
static float modulation_index = 0.5;

static float theta = 0.0f;

constexpr float TWO_PI = M_PI * 2.0f;
static float sqrt3over2 = std::sqrt(3.0f) / 2.0f;

MotorPwmControl control::control_loop(Current current_w2, Current current_v2,
                                      Current current_u2, Current current_w1,
                                      Current current_v1, Current current_u1) {
  
  theta += rotational_frequency * TWO_PI / pwm::frequency();
  while (theta >= TWO_PI) {
    theta -= TWO_PI;
  }
  while(theta < 0) {
    theta += TWO_PI;
  }

  const complex u = complex::from_polar(modulation_index * sqrt3over2 / 2.0f, theta);

  // Clark transformation
  float u_u = u.r();
  float u_v = -0.5 * u.r() + sqrt3over2 * u.im();
  float u_w = -0.5 * u.r() - sqrt3over2 * u.im();
  
  float control_u1 = 0.5 + u_u / 2.0f;
  float control_v1 = 0.5 + u_v / 2.0f;
  float control_w1 = 0.5 + u_w / 2.0f;

  MotorPwmControl control;
  control.U1_duty = control_u1;
  control.V1_duty = control_v1;
  control.W1_duty = control_w1;
  control.W2_duty = 0.5;
  control.V2_duty = 0.5;
  control.U2_duty = 0.5;
  return control;
}

void control::update() {
  // rotational_frequency = Frequency(canzero_get_frequency());
}
