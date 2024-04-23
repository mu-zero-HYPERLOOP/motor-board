#include "adc_mux.hpp"
#include "core_pins.h"
#include "pinout.h"

uint16_t mux_read_blocking(uint8_t sel) {
  digitalWrite(MUX_SEL0_PIN, (sel & 0x1) != 0);
  digitalWrite(MUX_SEL1_PIN, (sel & 0x2) != 0);
  digitalWrite(MUX_SEL2_PIN, (sel & 0x4) != 0);
  delay(100); // 10ms
  return analogRead(MUX_AIN_PIN);
}
