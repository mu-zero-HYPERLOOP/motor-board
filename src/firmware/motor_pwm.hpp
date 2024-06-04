#pragma once


#include <cstdint>

struct pwm_control_t {
  float U1_duty = 0.5; // range [0,1] 
  float V1_duty = 0.5;
  float W1_duty = 0.5;
  float U2_duty = 0.5;
  float V2_duty = 0.5;
  float W2_duty = 0.5;
};

void pwm_init();

void pwm_enable_reload_interrupt_isr();

void pwm_disable_reload_interrupt_isr();

void set_PWM_freq(float pwm_freq);

void pwm_set_control(pwm_control_t& pwm_control);

void pwm_on_isr();
