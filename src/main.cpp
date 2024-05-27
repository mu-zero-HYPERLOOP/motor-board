#include "adc_settings.h"
#include "canzero/canzero.h"
#include "control.h"
#include "firmware/adc_etc.hpp"
#include "firmware/motor_board.h"
#include "firmware/motor_pwm.h"
#include "firmware/pinout.h"
#include "firmware/pwm.hpp"
#include "firmware/xbar.hpp"
#include "fsm/fsm.h"
#include "sensors/accelerometer.h"
#include "sensors/ext_ntcs.h"
#include "sensors/mcu_temperature.h"
#include "sensors/on_board_ntcs.h"
#include "sensors/vdc_voltage.hpp"
#include "util/lina.h"

int main() {

  canzero_init();

  fsm::begin();
  canzero_update_continue(canzero_get_time());

  motor_board::BeginInfo beginInfo;
  beginInfo.pwmBeginInfo = control::begin();
  beginInfo.adcBeginInfo = adc_settings::begin();
  motor_board::begin(beginInfo);

  sensors::accelerometer::begin();
  sensors::ext_ntcs::begin();
  sensors::mcu_temperature::begin();
  sensors::on_board_ntcs::begin();
  sensors::vdc_voltage::begin();

  sensors::accelerometer::calibrate();
  sensors::ext_ntcs::calibrate();
  sensors::mcu_temperature::calibrate();
  sensors::on_board_ntcs::calibrate();
  sensors::vdc_voltage::calibrate();

  fsm::update(); // transition into the idle state.
  while (true) {
    canzero_can0_poll();
    canzero_can1_poll();

    motor_board::update_continue();

    sensors::accelerometer::update();
    sensors::ext_ntcs::update();
    sensors::mcu_temperature::update();
    sensors::on_board_ntcs::update();
    sensors::vdc_voltage::update();

    fsm::update();

    canzero_update_continue(canzero_get_time());
  }
}
