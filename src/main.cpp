#include "firmware/adc_mux.hpp"
#include <Arduino.h>

int main() {

  Serial.begin(9600);

  while (true) {

    Serial.printf("Reading....\n");
    uint16_t mux0 = mux_read_blocking(0);
    uint16_t mux1 = mux_read_blocking(1);
    uint16_t mux2 = mux_read_blocking(2);
    uint16_t mux3 = mux_read_blocking(3);
    uint16_t mux4 = mux_read_blocking(4);
    uint16_t mux5 = mux_read_blocking(5);
    uint16_t mux6 = mux_read_blocking(6);
    uint16_t mux7 = mux_read_blocking(7);

    Serial.printf("mux[0..7] = {%f,%f,%f,%f,%f,%f,%f,%f}\n", mux0, mux1, mux2,
                  mux3, mux4, mux5, mux6, mux7);
  }
}
