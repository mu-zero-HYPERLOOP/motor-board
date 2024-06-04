#include "canzero/canzero.h"
#include "feedthrough_mosfet.h"
#include "firmware/motor_board.h"
#include "precharge_mosfet.h"
#include "sdc_brake.h"

/**
 * This function get's invoked if a assertion fails
 */
void __assert_func(const char *filename, int line, const char *assert_func,
                   const char *expr) {
  canzero_set_assertion_fault(error_flag_ERROR);
  canzero_update_continue(canzero_get_time());

  pwm::disable_trig0();
  pwm::disable_trig1();
  pwm::control(PwmControl());
  pwm::enable_output();
  sdc_brake::brake_immediatly();
  precharge_mosfet::open();
  feedthrough_mosfet::open();
  while (true) {
    motor_board::update();
  }
}
