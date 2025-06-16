#include "control.h"
#include "firmware/motor_pwm.h"
#include "util/lina.h"
#include <cmath>

static Frequency rotational_frequency = 1_Hz;
static float modulation_index = 0.5;

static float theta = 0.0f;

constexpr float TWO_PI = M_PI * 2.0f;
static float sqrt3over2 = std::sqrt(3.0f) / 2.0f;

void control::begin() {
}


// TODO vdc measurement is not implemented see adc_isr::adc_etc_done0_isr
MotorPwmControl control::control_loop(Voltage vdc) {
  
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

  MotorPwmControl control;
  control.U1_duty = 0.5 + u_u / 1.0f;
  control.V1_duty = 0.5 + u_v / 1.0f;
  control.W1_duty = 0.5 + u_w / 1.0f;
  control.U2_duty = 0.5 - u_u / 1.0f;
  control.V2_duty = 0.5 - u_v / 1.0f;
  control.W2_duty = 0.5 - u_w / 1.0f;
  return control;
}

void control::update() {
  // rotational_frequency = Frequency(canzero_get_frequency());
}
