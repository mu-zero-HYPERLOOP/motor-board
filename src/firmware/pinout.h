#pragma once

enum ctrl_pin {
  mux_sel0 = 27,
  mux_sel1 = 26,
  mux_sel2 = 25,
  accel_cs = 10,
  sdc_trig = 32,
  precharge_start = 23,
  precharge_done = 22,
};

enum pwm_pin {
  ctrl_w2_high = 6,
  ctrl_v1_low = 7,
  ctrl_v1_high = 8,
  ctrl_v2_low = 9,
  ctrl_w1_low = 28,
  ctrl_w1_high = 29,
  ctrl_u2_low = 37,
  ctrl_u2_high = 36,
  ctrl_u1_low = 33,
};

enum ain_pin {
  ain_mux_21 = 21,
  ain_vdc_20 = 20,
  ain_imeas_w2_19 = 19,
  ain_imeas_v2_18 = 18,
  ain_imeas_u2_17 = 17,
  ain_imeas_u1_16 = 16,
  ain_imeas_v1_15 = 15,
  ain_imeas_w1_14 = 14,
};

enum mux_pin {
  mux_ntc_1_2 = ain_mux_21 | (2 << 8),
  mux_ntc_2_1 = ain_mux_21 | (1 << 8),
  mux_ntc_3_0 = ain_mux_21 | (0 << 8),
  mux_ntc_ext_1_4 = ain_mux_21 | (4 << 8),
  mux_ntc_ext_2_5 = ain_mux_21 | (5 << 8),
  mux_ntc_ext_3_6 = ain_mux_21 | (6 << 8),
  mux_ntc_ext_4_7 = ain_mux_21 | (7 << 8),

  MUX_PIN_SHR = 0, // shift left
  MUX_PIN_MASK = 0xFF,
  MUX_SEL_SHR = 8, // shift left
  MUX_SEL_MASK  = 0xFF << 8,
};
