#include "canzero/canzero.h"
#include "feedthrough_mosfet.h"
#include "firmware/motor_board.h"
#include "fsm/states.h"
#include "precharge_mosfet.h"
#include "sdc_brake.h"

constexpr Duration MIN_PRECHARGE_TIME = 3_s;
constexpr Duration MAX_PRECHARGE_TIME = 5_s;
constexpr Voltage REQUIRED_VDC_VOLTAGE = 40_V;

motor_state fsm::states::precharge(motor_command cmd,
                                   Duration time_since_last_transition) {

  if (motor_command_DISARM45 == cmd || motor_command_ABORT == cmd) {
    return motor_state_DISARMING45;
  }

  if (time_since_last_transition > MAX_PRECHARGE_TIME){
    return motor_state_DISARMING45;
  }

  if (time_since_last_transition > MIN_PRECHARGE_TIME &&
      canzero_get_vdc_voltage() > static_cast<float>(REQUIRED_VDC_VOLTAGE)) {
    return motor_state_READY;
  }


  pwm::control(PwmControl());
  pwm::enable_output();
  pwm::disable_trig0();
  pwm::disable_trig1();

  if (!sdc_brake::request_close()){
    canzero_set_command(motor_command_NONE);
    return motor_state_DISARMING45;
  }
  precharge_mosfet::close();
  feedthrough_mosfet::open();


  return motor_state_PRECHARGE;
}
