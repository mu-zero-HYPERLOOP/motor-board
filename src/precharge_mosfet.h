#pragma once

#include "firmware/motor_board.h"

namespace precharge_mosfet{

static inline void close(){
  motor_board::set_digital(ctrl_pin::precharge_start, true);
}

static inline void open(){
  motor_board::set_digital(ctrl_pin::precharge_start, false);
}
}
