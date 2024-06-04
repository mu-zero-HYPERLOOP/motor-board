#pragma once

#include "canzero/canzero.h"
#include "firmware/motor_board.h"

namespace precharge_mosfet{

static inline void close(){
  motor_board::set_digital(ctrl_pin::precharge_start, true);
  canzero_set_precharge_status(sdc_status_CLOSED);
}

static inline void open(){
  motor_board::set_digital(ctrl_pin::precharge_start, false);
  canzero_set_precharge_status(sdc_status_OPEN);
}
}
