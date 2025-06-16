#pragma once

#include "canzero/canzero.h"
#include "firmware/motor_board.h"

namespace feedthrough_mosfet {

static inline void close(){
  motor_board::set_digital(ctrl_pin::precharge_done, true);
}

static inline void open(){
  motor_board::set_digital(ctrl_pin::precharge_start, false);
}

}
