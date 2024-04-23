#include "adc_mux.hpp"
#include "core_pins.h"
#include "pinout.h"
#include "firmware/adc_etc.hpp"

uint16_t mux_read_blocking(uint8_t sel) {
  digitalWrite(MUX_SEL0, (sel & 0x1) != 0);
  digitalWrite(MUX_SEL1, (sel & 0x2) != 0);
  digitalWrite(MUX_SEL2, (sel & 0x4) != 0);
  delay(1); // 10ms
  return AdcEtc::readSingle(AIN_MUX);
}
