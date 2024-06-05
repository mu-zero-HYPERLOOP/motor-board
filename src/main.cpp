#include "adc_config.h"
#include "adc_isr.h"
#include "canzero/canzero.h"
#include "control.h"
#include "firmware/adc_etc.h"
#include "firmware/motor_board.h"
#include "firmware/motor_pwm.h"
#include "firmware/pinout.h"
#include "firmware/pwm.h"
#include "firmware/xbar.h"
#include "fsm/fsm.h"
#include "pwm_config.h"
#include "sdc_brake.h"
#include "sensors/accelerometer.h"
#include "sensors/ext_ntcs.h"
#include "sensors/mcu_temperature.h"
#include "sensors/on_board_ntcs.h"
#include "sensors/phase_current.h"
#include "sensors/vdc.h"
#include "util/interval.h"
#include "util/lina.h"
#include "xbar_config.h"
#include "util/timing.h"
#include "print.h"
#include <ctime>


static IntervalTiming mainLoopIntervalTimer;

int main() {
  motor_board::delay(4_s);
  canzero_init();
  

  fsm::begin();
  canzero_update_continue(canzero_get_time());

  motor_board::begin();
  
  pwm_config();
  adc_config();
  xbar_config();


  sdc_brake::begin();

  // Setup sensors
  /* sensors::accelerometer::begin(); */
  sensors::ext_ntcs::begin();
  sensors::mcu_temperature::begin();
  sensors::on_board_ntcs::begin();
  sensors::phase_current::begin();
  sensors::vdc::begin();

  // Calibrate sensors
  /* sensors::accelerometer::calibrate(); */
  sensors::ext_ntcs::calibrate();
  sensors::mcu_temperature::calibrate();
  sensors::on_board_ntcs::calibrate();
  sensors::phase_current::calibrate();
  sensors::vdc::calibrate();

  adc_isr::begin();
  control::begin();

  mainLoopIntervalTimer.reset();

  // init -> idle
  fsm::finish_init();
  debugPrintf("Init finished\n");
  debugPrintFlush();
  while (true) {
    canzero_can0_poll();
    canzero_can1_poll();

    motor_board::update();

    /* sensors::accelerometer::update(); */
    sensors::ext_ntcs::update();
    sensors::mcu_temperature::update();
    sensors::on_board_ntcs::update();
    sensors::phase_current::update();
    sensors::vdc::update();

    adc_isr::update();
    control::update();

    sdc_brake::update();

    fsm::update();

    mainLoopIntervalTimer.tick();
    canzero_set_loop_frequency(static_cast<float>(mainLoopIntervalTimer.frequency()) / 1e3);

    canzero_update_continue(canzero_get_time());
  }
}
