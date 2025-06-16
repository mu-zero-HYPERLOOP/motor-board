#include "adc_config.h"
#include "adc_isr.h"
#include "control.h"
#include "feedthrough_mosfet.h"
#include "firmware/motor_board.h"
#include "precharge_mosfet.h"
#include "pwm_config.h"
#include "sdc_brake.h"
#include "xbar_config.h"
#include "print.h"
#include <cstdio>


int main() {
  motor_board::begin();
  // Configure PWM
  pwm_config();

  // Configure ADC
  adc_config();

  // Connect PWM trig0 with ADC trig0
  // On PWM reload start a ADC convertion
  xbar_config();


  // Start firmware.
  sdc_brake::begin();
  adc_isr::begin();
  control::begin();

  pwm::disable_trig0();
  pwm::disable_output();

  precharge_mosfet::open();
  feedthrough_mosfet::open();

  debugPrintf("Initalizated and waiting");
  motor_board::delay(5_s);
  debugPrintf("Close SDC");
  if (!sdc_brake::request_close()) {
    while(true) {
      motor_board::delay(1_s);
      debugPrintf("Failed to open SDC.");
    }
  }
  debugPrintf("Start precharge");
  precharge_mosfet::close();
  feedthrough_mosfet::open();


  motor_board::delay(1_s);
  precharge_mosfet::open();
  feedthrough_mosfet::close();
  debugPrintf("Precharge done");

  motor_board::delay(5_s);

  
  pwm::enable_trig0();
  pwm::enable_output();
  // see control.cpp the control_loop is executed from this point on.


  while (true) {
    motor_board::update();
  }
}
