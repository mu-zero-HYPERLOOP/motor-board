#include "canzero.h"
#include <avr/pgmspace.h>
uint32_t min_u32(uint32_t v, uint32_t max) {
    if (v > max) {
        return max;
    }
    return v;
}
uint64_t min_u64(uint64_t v, uint64_t max) {
    if (v > max) {
        return max;
    }
    return v;
}
uint64_t DMAMEM __oe_config_hash;
date_time DMAMEM __oe_build_time;
motor_state DMAMEM __oe_state;
sdc_status DMAMEM __oe_sdc_status;
error_flag DMAMEM __oe_assertion_fault;
error_flag DMAMEM __oe_error_arming_failed;
error_flag DMAMEM __oe_error_precharge_failed;
error_flag DMAMEM __oe_error_acceleration_out_of_range;
error_flag DMAMEM __oe_error_acceleration_calibration_failed;
error_flag DMAMEM __oe_error_invalid_target_acceleration;
error_flag DMAMEM __oe_error_board_temperature1_invalid;
error_flag DMAMEM __oe_error_board_temperature2_invalid;
error_flag DMAMEM __oe_error_board_temperature3_invalid;
error_flag DMAMEM __oe_error_mcu_temperature_invalid;
error_flag DMAMEM __oe_error_lim_temperature1_invalid;
error_flag DMAMEM __oe_error_lim_temperature2_invalid;
error_flag DMAMEM __oe_error_lim_temperature3_invalid;
error_flag DMAMEM __oe_error_lim_temperature4_invalid;
error_flag DMAMEM __oe_error_heartbeat_miss;
error_level DMAMEM __oe_error_level_current_u1;
error_level DMAMEM __oe_error_level_current_v1;
error_level DMAMEM __oe_error_level_current_w1;
error_level DMAMEM __oe_error_level_current_u2;
error_level DMAMEM __oe_error_level_current_v2;
error_level DMAMEM __oe_error_level_current_w2;
error_level DMAMEM __oe_error_level_vdc_voltage;
error_level DMAMEM __oe_error_level_board_temperature;
error_level DMAMEM __oe_error_level_mcu_temperature;
error_level DMAMEM __oe_error_level_lim_temperature;
sdc_status DMAMEM __oe_precharge_status;
sdc_status DMAMEM __oe_feedthrough_status;
motor_command DMAMEM __oe_command;
bool_t DMAMEM __oe_control_active;
float DMAMEM __oe_current_u1;
float DMAMEM __oe_current_v1;
float DMAMEM __oe_current_w1;
float DMAMEM __oe_current_u2;
float DMAMEM __oe_current_v2;
float DMAMEM __oe_current_w2;
float DMAMEM __oe_current_max;
float DMAMEM __oe_current_average;
error_level_config DMAMEM __oe_error_level_config_current_phase;
float DMAMEM __oe_frequency;
float DMAMEM __oe_modulation_index;
float DMAMEM __oe_acceleration;
double DMAMEM __oe_acceleration_calibration_variance;
double DMAMEM __oe_acceleration_calibration_offset;
float DMAMEM __oe_target_acceleration;
float DMAMEM __oe_external_acceleration;
float DMAMEM __oe_external_velocity;
float DMAMEM __oe_external_position;
float DMAMEM __oe_vdc_voltage;
error_flag DMAMEM __oe_error_vdc_voltage_invalid;
error_level_config DMAMEM __oe_error_level_config_vdc_voltage;
float DMAMEM __oe_board_temperature1;
float DMAMEM __oe_board_temperature2;
float DMAMEM __oe_board_temperature3;
float DMAMEM __oe_board_avg_temperature;
float DMAMEM __oe_board_max_temperature;
float DMAMEM __oe_board_min_temperature;
error_level_config DMAMEM __oe_error_level_config_board_temperature;
float DMAMEM __oe_mcu_temperature;
error_level_config DMAMEM __oe_error_level_config_mcu_temperature;
float DMAMEM __oe_lim_temperature1;
float DMAMEM __oe_lim_temperature2;
float DMAMEM __oe_lim_temperature3;
float DMAMEM __oe_lim_temperature4;
float DMAMEM __oe_lim_average_temperature;
float DMAMEM __oe_lim_max_temperature;
float DMAMEM __oe_lim_min_temperature;
error_level_config DMAMEM __oe_error_level_config_lim_temperature;
float DMAMEM __oe_loop_frequency;
uint8_t DMAMEM __oe_last_node_missed;
bool_t DMAMEM __oe_ignore_45v;
static void canzero_serialize_canzero_message_get_resp(canzero_message_get_resp* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  for(uint8_t i = 0; i < 8; ++i){
    data[i] = 0;
  }
  frame->id = 0x1BD;
  frame->dlc = 8;
  ((uint32_t*)data)[0] = (uint8_t)(msg->m_header.m_sof & (0xFF >> (8 - 1)));
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_header.m_eof & (0xFF >> (8 - 1))) << 1;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_header.m_toggle & (0xFF >> (8 - 1))) << 2;
  ((uint32_t*)data)[0] |= (uint16_t)(msg->m_header.m_od_index & (0xFFFF >> (16 - 13))) << 3;
  ((uint32_t*)data)[0] |= msg->m_header.m_client_id << 16;
  ((uint32_t*)data)[0] |= msg->m_header.m_server_id << 24;
  ((uint32_t*)data)[1] = msg->m_data;
}
static void canzero_serialize_canzero_message_set_resp(canzero_message_set_resp* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  for(uint8_t i = 0; i < 8; ++i){
    data[i] = 0;
  }
  frame->id = 0x1DD;
  frame->dlc = 4;
  ((uint32_t*)data)[0] = (uint16_t)(msg->m_header.m_od_index & (0xFFFF >> (16 - 13)));
  ((uint32_t*)data)[0] |= msg->m_header.m_client_id << 13;
  ((uint32_t*)data)[0] |= msg->m_header.m_server_id << 21;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_header.m_erno & (0xFF >> (8 - 1))) << 29;
}
static void canzero_serialize_canzero_message_motor_driver_stream_state(canzero_message_motor_driver_stream_state* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  for(uint8_t i = 0; i < 8; ++i){
    data[i] = 0;
  }
  frame->id = 0x95;
  frame->dlc = 2;
  ((uint32_t*)data)[0] = (uint8_t)(msg->m_state & (0xFF >> (8 - 3)));
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_sdc_status & (0xFF >> (8 - 1))) << 3;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_command & (0xFF >> (8 - 3))) << 4;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_control_active & (0xFF >> (8 - 1))) << 7;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_precharge_status & (0xFF >> (8 - 1))) << 8;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_feedthrough_status & (0xFF >> (8 - 1))) << 9;
}
static void canzero_serialize_canzero_message_motor_driver_stream_config_hash(canzero_message_motor_driver_stream_config_hash* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  for(uint8_t i = 0; i < 8; ++i){
    data[i] = 0;
  }
  frame->id = 0x55;
  frame->dlc = 8;
  ((uint64_t*)data)[0] = msg->m_config_hash;
}
static void canzero_serialize_canzero_message_motor_driver_stream_errors(canzero_message_motor_driver_stream_errors* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  for(uint8_t i = 0; i < 8; ++i){
    data[i] = 0;
  }
  frame->id = 0x75;
  frame->dlc = 6;
  ((uint32_t*)data)[0] = (uint8_t)(msg->m_assertion_fault & (0xFF >> (8 - 1)));
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_error_arming_failed & (0xFF >> (8 - 1))) << 1;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_error_precharge_failed & (0xFF >> (8 - 1))) << 2;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_error_acceleration_out_of_range & (0xFF >> (8 - 1))) << 3;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_error_acceleration_calibration_failed & (0xFF >> (8 - 1))) << 4;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_error_invalid_target_acceleration & (0xFF >> (8 - 1))) << 5;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_error_board_temperature1_invalid & (0xFF >> (8 - 1))) << 6;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_error_board_temperature2_invalid & (0xFF >> (8 - 1))) << 7;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_error_board_temperature3_invalid & (0xFF >> (8 - 1))) << 8;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_error_mcu_temperature_invalid & (0xFF >> (8 - 1))) << 9;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_error_lim_temperature1_invalid & (0xFF >> (8 - 1))) << 10;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_error_lim_temperature2_invalid & (0xFF >> (8 - 1))) << 11;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_error_lim_temperature3_invalid & (0xFF >> (8 - 1))) << 12;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_error_lim_temperature4_invalid & (0xFF >> (8 - 1))) << 13;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_error_heartbeat_miss & (0xFF >> (8 - 1))) << 14;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_error_level_current_u1 & (0xFF >> (8 - 2))) << 15;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_error_level_current_v1 & (0xFF >> (8 - 2))) << 17;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_error_level_current_w1 & (0xFF >> (8 - 2))) << 19;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_error_level_current_u2 & (0xFF >> (8 - 2))) << 21;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_error_level_current_v2 & (0xFF >> (8 - 2))) << 23;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_error_level_current_w2 & (0xFF >> (8 - 2))) << 25;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_error_level_vdc_voltage & (0xFF >> (8 - 2))) << 27;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_error_level_board_temperature & (0xFF >> (8 - 2))) << 29;
  ((uint64_t*)data)[0] |= ((uint64_t)(uint8_t)(msg->m_error_level_mcu_temperature & (0xFF >> (8 - 2)))) << 31 ;
  ((uint32_t*)data)[1] |= (uint8_t)(msg->m_error_level_lim_temperature & (0xFF >> (8 - 2))) << 1;
  ((uint32_t*)data)[1] |= (uint8_t)(msg->m_error_vdc_voltage_invalid & (0xFF >> (8 - 1))) << 3;
  ((uint32_t*)data)[1] |= msg->m_last_node_missed << 4;
}
static void canzero_serialize_canzero_message_motor_driver_stream_debug(canzero_message_motor_driver_stream_debug* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  for(uint8_t i = 0; i < 8; ++i){
    data[i] = 0;
  }
  frame->id = 0x99;
  frame->dlc = 4;
  uint32_t loop_frequency_0 = ((msg->m_loop_frequency - 0) / 0.00000023283064370807974) + 0.5f;
  if (loop_frequency_0 > 0xFFFFFFFF) {
    loop_frequency_0 = 0xFFFFFFFF;
  }
  ((uint32_t*)data)[0] = loop_frequency_0;
}
static void canzero_serialize_canzero_message_motor_driver_stream_currents_u1v1w1(canzero_message_motor_driver_stream_currents_u1v1w1* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  for(uint8_t i = 0; i < 8; ++i){
    data[i] = 0;
  }
  frame->id = 0x79;
  frame->dlc = 8;
  uint32_t current_u1_0 = ((msg->m_current_u1 - -100) / 0.0030518043793392844) + 0.5f;
  if (current_u1_0 > 0xFFFF) {
    current_u1_0 = 0xFFFF;
  }
  ((uint32_t*)data)[0] = current_u1_0;
  uint32_t current_v1_16 = ((msg->m_current_v1 - -100) / 0.0030518043793392844) + 0.5f;
  if (current_v1_16 > 0xFFFF) {
    current_v1_16 = 0xFFFF;
  }
  ((uint32_t*)data)[0] |= current_v1_16 << 16;
  uint32_t current_w1_32 = ((msg->m_current_w1 - -100) / 0.0030518043793392844) + 0.5f;
  if (current_w1_32 > 0xFFFF) {
    current_w1_32 = 0xFFFF;
  }
  ((uint32_t*)data)[1] = current_w1_32;
  uint32_t current_max_48 = ((msg->m_current_max - -100) / 0.0030518043793392844) + 0.5f;
  if (current_max_48 > 0xFFFF) {
    current_max_48 = 0xFFFF;
  }
  ((uint32_t*)data)[1] |= current_max_48 << 16;
}
static void canzero_serialize_canzero_message_motor_driver_stream_current_u2v2w2(canzero_message_motor_driver_stream_current_u2v2w2* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  for(uint8_t i = 0; i < 8; ++i){
    data[i] = 0;
  }
  frame->id = 0x59;
  frame->dlc = 8;
  uint32_t current_u2_0 = ((msg->m_current_u2 - -100) / 0.0030518043793392844) + 0.5f;
  if (current_u2_0 > 0xFFFF) {
    current_u2_0 = 0xFFFF;
  }
  ((uint32_t*)data)[0] = current_u2_0;
  uint32_t current_v2_16 = ((msg->m_current_v2 - -100) / 0.0030518043793392844) + 0.5f;
  if (current_v2_16 > 0xFFFF) {
    current_v2_16 = 0xFFFF;
  }
  ((uint32_t*)data)[0] |= current_v2_16 << 16;
  uint32_t current_w2_32 = ((msg->m_current_w2 - -100) / 0.0030518043793392844) + 0.5f;
  if (current_w2_32 > 0xFFFF) {
    current_w2_32 = 0xFFFF;
  }
  ((uint32_t*)data)[1] = current_w2_32;
  uint32_t current_average_48 = ((msg->m_current_average - -100) / 0.0030518043793392844) + 0.5f;
  if (current_average_48 > 0xFFFF) {
    current_average_48 = 0xFFFF;
  }
  ((uint32_t*)data)[1] |= current_average_48 << 16;
}
static void canzero_serialize_canzero_message_motor_driver_stream_control_info(canzero_message_motor_driver_stream_control_info* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  for(uint8_t i = 0; i < 8; ++i){
    data[i] = 0;
  }
  frame->id = 0x19A;
  frame->dlc = 8;
  uint32_t frequency_0 = ((msg->m_frequency - 0) / 0.0007629510948348211) + 0.5f;
  if (frequency_0 > 0xFFFF) {
    frequency_0 = 0xFFFF;
  }
  ((uint32_t*)data)[0] = frequency_0;
  uint32_t modulation_index_16 = ((msg->m_modulation_index - 0) / 0.000015259021896696422) + 0.5f;
  if (modulation_index_16 > 0xFFFF) {
    modulation_index_16 = 0xFFFF;
  }
  ((uint32_t*)data)[0] |= modulation_index_16 << 16;
  uint32_t acceleration_32 = ((msg->m_acceleration - -10) / 0.00030518043793392844) + 0.5f;
  if (acceleration_32 > 0xFFFF) {
    acceleration_32 = 0xFFFF;
  }
  ((uint32_t*)data)[1] = acceleration_32;
  uint32_t vdc_voltage_48 = ((msg->m_vdc_voltage - -100) / 0.0030518043793392844) + 0.5f;
  if (vdc_voltage_48 > 0xFFFF) {
    vdc_voltage_48 = 0xFFFF;
  }
  ((uint32_t*)data)[1] |= vdc_voltage_48 << 16;
}
static void canzero_serialize_canzero_message_motor_driver_stream_board_temperature(canzero_message_motor_driver_stream_board_temperature* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  for(uint8_t i = 0; i < 8; ++i){
    data[i] = 0;
  }
  frame->id = 0x17A;
  frame->dlc = 7;
  uint32_t board_temperature1_0 = ((msg->m_board_temperature1 - -1) / 0.592156862745098) + 0.5f;
  if (board_temperature1_0 > 0xFF) {
    board_temperature1_0 = 0xFF;
  }
  ((uint32_t*)data)[0] = board_temperature1_0;
  uint32_t board_temperature2_8 = ((msg->m_board_temperature2 - -1) / 0.592156862745098) + 0.5f;
  if (board_temperature2_8 > 0xFF) {
    board_temperature2_8 = 0xFF;
  }
  ((uint32_t*)data)[0] |= board_temperature2_8 << 8;
  uint32_t board_temperature3_16 = ((msg->m_board_temperature3 - -1) / 0.592156862745098) + 0.5f;
  if (board_temperature3_16 > 0xFF) {
    board_temperature3_16 = 0xFF;
  }
  ((uint32_t*)data)[0] |= board_temperature3_16 << 16;
  uint32_t board_avg_temperature_24 = ((msg->m_board_avg_temperature - -1) / 0.592156862745098) + 0.5f;
  if (board_avg_temperature_24 > 0xFF) {
    board_avg_temperature_24 = 0xFF;
  }
  ((uint32_t*)data)[0] |= board_avg_temperature_24 << 24;
  uint32_t board_max_temperature_32 = ((msg->m_board_max_temperature - -1) / 0.592156862745098) + 0.5f;
  if (board_max_temperature_32 > 0xFF) {
    board_max_temperature_32 = 0xFF;
  }
  ((uint32_t*)data)[1] = board_max_temperature_32;
  uint32_t board_min_temperature_40 = ((msg->m_board_min_temperature - -1) / 0.592156862745098) + 0.5f;
  if (board_min_temperature_40 > 0xFF) {
    board_min_temperature_40 = 0xFF;
  }
  ((uint32_t*)data)[1] |= board_min_temperature_40 << 8;
  uint32_t mcu_temperature_48 = ((msg->m_mcu_temperature - -1) / 0.592156862745098) + 0.5f;
  if (mcu_temperature_48 > 0xFF) {
    mcu_temperature_48 = 0xFF;
  }
  ((uint32_t*)data)[1] |= mcu_temperature_48 << 16;
}
static void canzero_serialize_canzero_message_motor_driver_stream_lim_temperature(canzero_message_motor_driver_stream_lim_temperature* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  for(uint8_t i = 0; i < 8; ++i){
    data[i] = 0;
  }
  frame->id = 0xB9;
  frame->dlc = 7;
  uint32_t lim_temperature1_0 = ((msg->m_lim_temperature1 - -1) / 0.592156862745098) + 0.5f;
  if (lim_temperature1_0 > 0xFF) {
    lim_temperature1_0 = 0xFF;
  }
  ((uint32_t*)data)[0] = lim_temperature1_0;
  uint32_t lim_temperature2_8 = ((msg->m_lim_temperature2 - -1) / 0.592156862745098) + 0.5f;
  if (lim_temperature2_8 > 0xFF) {
    lim_temperature2_8 = 0xFF;
  }
  ((uint32_t*)data)[0] |= lim_temperature2_8 << 8;
  uint32_t lim_temperature3_16 = ((msg->m_lim_temperature3 - -1) / 0.592156862745098) + 0.5f;
  if (lim_temperature3_16 > 0xFF) {
    lim_temperature3_16 = 0xFF;
  }
  ((uint32_t*)data)[0] |= lim_temperature3_16 << 16;
  uint32_t lim_temperature4_24 = ((msg->m_lim_temperature4 - -1) / 0.592156862745098) + 0.5f;
  if (lim_temperature4_24 > 0xFF) {
    lim_temperature4_24 = 0xFF;
  }
  ((uint32_t*)data)[0] |= lim_temperature4_24 << 24;
  uint32_t lim_average_temperature_32 = ((msg->m_lim_average_temperature - -1) / 0.592156862745098) + 0.5f;
  if (lim_average_temperature_32 > 0xFF) {
    lim_average_temperature_32 = 0xFF;
  }
  ((uint32_t*)data)[1] = lim_average_temperature_32;
  uint32_t lim_max_temperature_40 = ((msg->m_lim_max_temperature - -1) / 0.592156862745098) + 0.5f;
  if (lim_max_temperature_40 > 0xFF) {
    lim_max_temperature_40 = 0xFF;
  }
  ((uint32_t*)data)[1] |= lim_max_temperature_40 << 8;
  uint32_t lim_min_temperature_48 = ((msg->m_lim_min_temperature - -1) / 0.592156862745098) + 0.5f;
  if (lim_min_temperature_48 > 0xFF) {
    lim_min_temperature_48 = 0xFF;
  }
  ((uint32_t*)data)[1] |= lim_min_temperature_48 << 16;
}
static void canzero_serialize_canzero_message_heartbeat_can0(canzero_message_heartbeat_can0* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  for(uint8_t i = 0; i < 8; ++i){
    data[i] = 0;
  }
  frame->id = 0x1F4;
  frame->dlc = 2;
  ((uint32_t*)data)[0] = msg->m_node_id;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_unregister & (0xFF >> (8 - 1))) << 8;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_ticks_next & (0xFF >> (8 - 7))) << 9;
}
static void canzero_serialize_canzero_message_heartbeat_can1(canzero_message_heartbeat_can1* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  for(uint8_t i = 0; i < 8; ++i){
    data[i] = 0;
  }
  frame->id = 0x1F3;
  frame->dlc = 2;
  ((uint32_t*)data)[0] = msg->m_node_id;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_unregister & (0xFF >> (8 - 1))) << 8;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_ticks_next & (0xFF >> (8 - 7))) << 9;
}
static void canzero_deserialize_canzero_message_get_req(canzero_frame* frame, canzero_message_get_req* msg) {
  uint8_t* data = frame->data;
  msg->m_header.m_od_index = (((uint32_t*)data)[0] & (0xFFFFFFFF >> (32 - 13)));
  msg->m_header.m_client_id = ((((uint32_t*)data)[0] >> 13) & (0xFFFFFFFF >> (32 - 8)));
  msg->m_header.m_server_id = ((((uint32_t*)data)[0] >> 21) & (0xFFFFFFFF >> (32 - 8)));
}
static void canzero_deserialize_canzero_message_set_req(canzero_frame* frame, canzero_message_set_req* msg) {
  uint8_t* data = frame->data;
  msg->m_header.m_sof = (((uint32_t*)data)[0] & (0xFFFFFFFF >> (32 - 1)));
  msg->m_header.m_eof = ((((uint32_t*)data)[0] >> 1) & (0xFFFFFFFF >> (32 - 1)));
  msg->m_header.m_toggle = ((((uint32_t*)data)[0] >> 2) & (0xFFFFFFFF >> (32 - 1)));
  msg->m_header.m_od_index = ((((uint32_t*)data)[0] >> 3) & (0xFFFFFFFF >> (32 - 13)));
  msg->m_header.m_client_id = ((((uint32_t*)data)[0] >> 16) & (0xFFFFFFFF >> (32 - 8)));
  msg->m_header.m_server_id = ((((uint32_t*)data)[0] >> 24) & (0xFFFFFFFF >> (32 - 8)));
  msg->m_data = (((uint32_t*)data)[1] & (0xFFFFFFFF >> (32 - 32)));
}
static void canzero_deserialize_canzero_message_mother_board_stream_motor_command(canzero_frame* frame, canzero_message_mother_board_stream_motor_command* msg) {
  uint8_t* data = frame->data;
  msg->m_target_acceleration = (((uint32_t*)data)[0] & (0xFFFFFFFF >> (32 - 32))) * 0.000000023283064370807974 + -50;
  msg->m_motor_driver_command = (motor_command)(((uint32_t*)data)[1] & (0xFFFFFFFF >> (32 - 3)));
  msg->m_pod_grounded = (bool_t)((((uint32_t*)data)[1] >> 3) & (0xFFFFFFFF >> (32 - 1)));
}
static void canzero_deserialize_canzero_message_mother_board_stream_debug_settings(canzero_frame* frame, canzero_message_mother_board_stream_debug_settings* msg) {
  uint8_t* data = frame->data;
  msg->m_ignore_45v = (bool_t)(((uint32_t*)data)[0] & (0xFFFFFFFF >> (32 - 1)));
}
static void canzero_deserialize_canzero_message_heartbeat_can0(canzero_frame* frame, canzero_message_heartbeat_can0* msg) {
  uint8_t* data = frame->data;
  msg->m_node_id = (((uint32_t*)data)[0] & (0xFFFFFFFF >> (32 - 8)));
  msg->m_unregister = ((((uint32_t*)data)[0] >> 8) & (0xFFFFFFFF >> (32 - 1)));
  msg->m_ticks_next = ((((uint32_t*)data)[0] >> 9) & (0xFFFFFFFF >> (32 - 7)));
}
static void canzero_deserialize_canzero_message_heartbeat_can1(canzero_frame* frame, canzero_message_heartbeat_can1* msg) {
  uint8_t* data = frame->data;
  msg->m_node_id = (((uint32_t*)data)[0] & (0xFFFFFFFF >> (32 - 8)));
  msg->m_unregister = ((((uint32_t*)data)[0] >> 8) & (0xFFFFFFFF >> (32 - 1)));
  msg->m_ticks_next = ((((uint32_t*)data)[0] >> 9) & (0xFFFFFFFF >> (32 - 7)));
}
__attribute__((weak)) void canzero_can0_wdg_timeout(uint8_t node_id) {}
__attribute__((weak)) void canzero_can0_wdg_recovered(uint8_t node_id) {}
__attribute__((weak)) void canzero_can1_wdg_timeout(uint8_t node_id) {}
__attribute__((weak)) void canzero_can1_wdg_recovered(uint8_t node_id) {}

typedef enum {
  HEARTBEAT_JOB_TAG = 0,
  HEARTBEAT_WDG_JOB_TAG = 1,
  GET_RESP_FRAGMENTATION_JOB_TAG = 2,
  STREAM_INTERVAL_JOB_TAG = 3,
} job_tag;

typedef struct {
  uint32_t *buffer;
  uint8_t offset;
  uint8_t size;
  uint8_t od_index;
  uint8_t client_id;
} get_resp_fragmentation_job;

typedef struct {
  uint32_t last_schedule; 
  uint32_t stream_id;
} stream_interval_job;

typedef struct {
  unsigned int* can0_static_wdg_armed;
  int* can0_static_tick_countdowns;
  unsigned int* can0_dynamic_wdg_armed;
  int* can0_dynamic_tick_countdowns;

  unsigned int* can1_static_wdg_armed;
  int* can1_static_tick_countdowns;
  unsigned int* can1_dynamic_wdg_armed;
  int* can1_dynamic_tick_countdowns;
} heartbeat_wdg_job_t;

typedef struct {
  uint32_t climax;
  uint32_t position;
  job_tag tag;
  union {
    get_resp_fragmentation_job get_fragmentation_job;
    stream_interval_job stream_job;
    heartbeat_wdg_job_t wdg_job;
  } job;
} job_t;

union job_pool_allocator_entry {
  job_t job;
  union job_pool_allocator_entry *next;
};

typedef struct {
  union job_pool_allocator_entry job[64];
  union job_pool_allocator_entry *freelist;
} job_pool_allocator;

static job_pool_allocator DMAMEM job_allocator;
static void job_pool_allocator_init() {
  for (uint8_t i = 1; i < 64; i++) {
    job_allocator.job[i - 1].next = job_allocator.job + i;
  }
  job_allocator.job[64 - 1].next = NULL;
  job_allocator.freelist = job_allocator.job;
}

static job_t *job_pool_allocator_alloc() {
  if (job_allocator.freelist != NULL) {
    job_t *job = &job_allocator.freelist->job;
    job_allocator.freelist = job_allocator.freelist->next;
    return job;
  } else {
    return NULL;
  }
}

static void job_pool_allocator_free(job_t *job) {
  union job_pool_allocator_entry *entry = (union job_pool_allocator_entry *)job;
  entry->next = job_allocator.freelist;
  job_allocator.freelist = entry;
}

#define SCHEDULER_HEAP_SIZE 75
typedef struct {
  job_t *heap[SCHEDULER_HEAP_SIZE]; // job**
  uint32_t size;
} job_scheduler_t;

static job_scheduler_t DMAMEM scheduler;
static void scheduler_promote_job(job_t *job) {
  int index = job->position;
  if (index == 0) {
    return;
  }
  int parent = (job->position - 1) / 2;
  while (scheduler.heap[parent]->climax > scheduler.heap[index]->climax) {
    job_t *tmp = scheduler.heap[parent];
    scheduler.heap[parent] = scheduler.heap[index];
    scheduler.heap[index] = tmp;
    scheduler.heap[parent]->position = parent;
    scheduler.heap[index]->position = index;
    index = parent;
    parent = (index - 1) / 2;
  }
  if (index == 0) {
    canzero_request_update(job->climax);
  }
}

static void scheduler_schedule(job_t *job) {
  if (scheduler.size >= SCHEDULER_HEAP_SIZE) {
    return;
  }
  job->position = scheduler.size;
  scheduler.heap[scheduler.size] = job;
  scheduler.size += 1;
  scheduler_promote_job(job);
}

static int scheduler_continue(job_t **job, uint32_t time) {
  *job = scheduler.heap[0];
  return scheduler.heap[0]->climax <= time;
}

static void scheduler_reschedule(uint32_t climax) {
  job_t *job = scheduler.heap[0];
  job->climax = climax;
  int index = 0;
  int hsize = scheduler.size / 2;
  while (index < hsize) {
    int left = index * 2 + 1;
    int right = left + 1;
    int min;
    if (right < scheduler.size &&
        scheduler.heap[left]->climax >= scheduler.heap[right]->climax) {
      min = right;
    } else {
    min = left;
    }
    if (climax <= scheduler.heap[min]->climax) {
      break;
    }
    scheduler.heap[index] = scheduler.heap[min];
    scheduler.heap[index]->position = index;
    index = min;
  }
  scheduler.heap[index] = job;
  scheduler.heap[index]->position = index;
}
static void scheduler_unschedule() {
  scheduler.heap[0] = scheduler.heap[scheduler.size - 1];
  scheduler.heap[0]->position = 0;
  scheduler.size -= 1;
  scheduler_reschedule(scheduler.heap[0]->climax);
}
static const uint32_t get_resp_fragmentation_interval = 100;
static void schedule_get_resp_fragmentation_job(uint32_t *fragmentation_buffer, uint8_t size, uint8_t od_index, uint8_t client_id) {
  job_t *fragmentation_job = job_pool_allocator_alloc();
  fragmentation_job->climax = canzero_get_time() + get_resp_fragmentation_interval;
  fragmentation_job->tag = GET_RESP_FRAGMENTATION_JOB_TAG;
  fragmentation_job->job.get_fragmentation_job.buffer = fragmentation_buffer;
  fragmentation_job->job.get_fragmentation_job.offset = 1;
  fragmentation_job->job.get_fragmentation_job.size = size;
  fragmentation_job->job.get_fragmentation_job.od_index = od_index;
  fragmentation_job->job.get_fragmentation_job.client_id = client_id;
  scheduler_schedule(fragmentation_job);
}

static job_t heartbeat_job;
static const uint32_t heartbeat_interval = 100;
static void schedule_heartbeat_job() {
  heartbeat_job.climax = canzero_get_time();
  heartbeat_job.tag = HEARTBEAT_JOB_TAG;
  scheduler_schedule(&heartbeat_job);
}

static job_t heartbeat_wdg_job;
static const uint32_t heartbeat_wdg_tick_duration = 50;
unsigned int wdg_job_can0_static_wdg_armed[node_id_count];
int wdg_job_can0_static_tick_countdowns[node_id_count];
unsigned int wdg_job_can0_dynamic_wdg_armed[MAX_DYN_HEARTBEATS];
int wdg_job_can0_dynamic_tick_countdowns[MAX_DYN_HEARTBEATS];
unsigned int wdg_job_can1_static_wdg_armed[node_id_count];
int wdg_job_can1_static_tick_countdowns[node_id_count];
unsigned int wdg_job_can1_dynamic_wdg_armed[MAX_DYN_HEARTBEATS];
int wdg_job_can1_dynamic_tick_countdowns[MAX_DYN_HEARTBEATS];

static void schedule_heartbeat_wdg_job() {
  heartbeat_wdg_job.climax = canzero_get_time() + 100;
  heartbeat_wdg_job.tag = HEARTBEAT_WDG_JOB_TAG;
  heartbeat_wdg_job.job.wdg_job.can0_static_wdg_armed = wdg_job_can0_static_wdg_armed;
  heartbeat_wdg_job.job.wdg_job.can0_static_tick_countdowns = wdg_job_can0_static_tick_countdowns;
  heartbeat_wdg_job.job.wdg_job.can0_dynamic_wdg_armed = wdg_job_can0_dynamic_wdg_armed;
  heartbeat_wdg_job.job.wdg_job.can0_dynamic_tick_countdowns = wdg_job_can0_dynamic_tick_countdowns;
  heartbeat_wdg_job.job.wdg_job.can1_static_wdg_armed = wdg_job_can1_static_wdg_armed;
  heartbeat_wdg_job.job.wdg_job.can1_static_tick_countdowns = wdg_job_can1_static_tick_countdowns;
  heartbeat_wdg_job.job.wdg_job.can1_dynamic_wdg_armed = wdg_job_can1_dynamic_wdg_armed;
  heartbeat_wdg_job.job.wdg_job.can1_dynamic_tick_countdowns = wdg_job_can1_dynamic_tick_countdowns;
  for (unsigned int i = 0; i < node_id_count; ++i) {
    heartbeat_wdg_job.job.wdg_job.can0_static_tick_countdowns[i] = 10;
    heartbeat_wdg_job.job.wdg_job.can0_static_wdg_armed[i] = 0;
    heartbeat_wdg_job.job.wdg_job.can1_static_tick_countdowns[i] = 10;
    heartbeat_wdg_job.job.wdg_job.can1_static_wdg_armed[i] = 0;
  }
  for (unsigned int i = 0; i < MAX_DYN_HEARTBEATS; ++i) {
    heartbeat_wdg_job.job.wdg_job.can0_dynamic_tick_countdowns[i] = 4;
    heartbeat_wdg_job.job.wdg_job.can0_dynamic_wdg_armed[i] = 0;
    heartbeat_wdg_job.job.wdg_job.can1_dynamic_tick_countdowns[i] = 4;
    heartbeat_wdg_job.job.wdg_job.can1_dynamic_wdg_armed[i] = 0;
  }
  scheduler_schedule(&heartbeat_wdg_job);
}

static job_t state_interval_job;
static const uint32_t state_interval = 1;
static void schedule_state_interval_job(){
  uint32_t time = canzero_get_time();
  state_interval_job.climax = time + state_interval;
  state_interval_job.tag = STREAM_INTERVAL_JOB_TAG;
  state_interval_job.job.stream_job.stream_id = 0;
  state_interval_job.job.stream_job.last_schedule = time;
  scheduler_schedule(&state_interval_job);
}
static job_t config_hash_interval_job;
static const uint32_t config_hash_interval = 0;
static void schedule_config_hash_interval_job(){
  uint32_t time = canzero_get_time();
  config_hash_interval_job.climax = time + config_hash_interval;
  config_hash_interval_job.tag = STREAM_INTERVAL_JOB_TAG;
  config_hash_interval_job.job.stream_job.stream_id = 1;
  config_hash_interval_job.job.stream_job.last_schedule = time;
  scheduler_schedule(&config_hash_interval_job);
}
static job_t errors_interval_job;
static const uint32_t errors_interval = 1;
static void schedule_errors_interval_job(){
  uint32_t time = canzero_get_time();
  errors_interval_job.climax = time + errors_interval;
  errors_interval_job.tag = STREAM_INTERVAL_JOB_TAG;
  errors_interval_job.job.stream_job.stream_id = 2;
  errors_interval_job.job.stream_job.last_schedule = time;
  scheduler_schedule(&errors_interval_job);
}
static job_t debug_interval_job;
static const uint32_t debug_interval = 500;
static void schedule_debug_interval_job(){
  uint32_t time = canzero_get_time();
  debug_interval_job.climax = time + debug_interval;
  debug_interval_job.tag = STREAM_INTERVAL_JOB_TAG;
  debug_interval_job.job.stream_job.stream_id = 3;
  debug_interval_job.job.stream_job.last_schedule = time;
  scheduler_schedule(&debug_interval_job);
}
static job_t currents_u1v1w1_interval_job;
static const uint32_t currents_u1v1w1_interval = 50;
static void schedule_currents_u1v1w1_interval_job(){
  uint32_t time = canzero_get_time();
  currents_u1v1w1_interval_job.climax = time + currents_u1v1w1_interval;
  currents_u1v1w1_interval_job.tag = STREAM_INTERVAL_JOB_TAG;
  currents_u1v1w1_interval_job.job.stream_job.stream_id = 4;
  currents_u1v1w1_interval_job.job.stream_job.last_schedule = time;
  scheduler_schedule(&currents_u1v1w1_interval_job);
}
static job_t current_u2v2w2_interval_job;
static const uint32_t current_u2v2w2_interval = 50;
static void schedule_current_u2v2w2_interval_job(){
  uint32_t time = canzero_get_time();
  current_u2v2w2_interval_job.climax = time + current_u2v2w2_interval;
  current_u2v2w2_interval_job.tag = STREAM_INTERVAL_JOB_TAG;
  current_u2v2w2_interval_job.job.stream_job.stream_id = 5;
  current_u2v2w2_interval_job.job.stream_job.last_schedule = time;
  scheduler_schedule(&current_u2v2w2_interval_job);
}
static job_t control_info_interval_job;
static const uint32_t control_info_interval = 100;
static void schedule_control_info_interval_job(){
  uint32_t time = canzero_get_time();
  control_info_interval_job.climax = time + control_info_interval;
  control_info_interval_job.tag = STREAM_INTERVAL_JOB_TAG;
  control_info_interval_job.job.stream_job.stream_id = 6;
  control_info_interval_job.job.stream_job.last_schedule = time;
  scheduler_schedule(&control_info_interval_job);
}
static job_t board_temperature_interval_job;
static const uint32_t board_temperature_interval = 250;
static void schedule_board_temperature_interval_job(){
  uint32_t time = canzero_get_time();
  board_temperature_interval_job.climax = time + board_temperature_interval;
  board_temperature_interval_job.tag = STREAM_INTERVAL_JOB_TAG;
  board_temperature_interval_job.job.stream_job.stream_id = 7;
  board_temperature_interval_job.job.stream_job.last_schedule = time;
  scheduler_schedule(&board_temperature_interval_job);
}
static job_t lim_temperature_interval_job;
static const uint32_t lim_temperature_interval = 250;
static void schedule_lim_temperature_interval_job(){
  uint32_t time = canzero_get_time();
  lim_temperature_interval_job.climax = time + lim_temperature_interval;
  lim_temperature_interval_job.tag = STREAM_INTERVAL_JOB_TAG;
  lim_temperature_interval_job.job.stream_job.stream_id = 8;
  lim_temperature_interval_job.job.stream_job.last_schedule = time;
  scheduler_schedule(&lim_temperature_interval_job);
}

static void schedule_jobs(uint32_t time) {
  for (uint8_t i = 0; i < 100; ++i) {
    canzero_enter_critical();
    job_t *job;
    if (!scheduler_continue(&job, time)) {
      canzero_exit_critical();
      return;
    }
    switch (job->tag) {
      case STREAM_INTERVAL_JOB_TAG: {
        switch (job->job.stream_job.stream_id) {
      case 0: {
        job->job.stream_job.last_schedule = time;
        scheduler_reschedule(time + 500);
        canzero_exit_critical();
        canzero_message_motor_driver_stream_state stream_message;
        stream_message.m_state = __oe_state;
        stream_message.m_sdc_status = __oe_sdc_status;
        stream_message.m_command = __oe_command;
        stream_message.m_control_active = __oe_control_active;
        stream_message.m_precharge_status = __oe_precharge_status;
        stream_message.m_feedthrough_status = __oe_feedthrough_status;
        canzero_frame stream_frame;
        canzero_serialize_canzero_message_motor_driver_stream_state(&stream_message, &stream_frame);
        canzero_can0_send(&stream_frame);
        break;
      }
      case 1: {
        job->job.stream_job.last_schedule = time;
        scheduler_reschedule(time + 5000);
        canzero_exit_critical();
        canzero_message_motor_driver_stream_config_hash stream_message;
        stream_message.m_config_hash = __oe_config_hash;
        canzero_frame stream_frame;
        canzero_serialize_canzero_message_motor_driver_stream_config_hash(&stream_message, &stream_frame);
        canzero_can1_send(&stream_frame);
        break;
      }
      case 2: {
        job->job.stream_job.last_schedule = time;
        scheduler_reschedule(time + 1000);
        canzero_exit_critical();
        canzero_message_motor_driver_stream_errors stream_message;
        stream_message.m_assertion_fault = __oe_assertion_fault;
        stream_message.m_error_arming_failed = __oe_error_arming_failed;
        stream_message.m_error_precharge_failed = __oe_error_precharge_failed;
        stream_message.m_error_acceleration_out_of_range = __oe_error_acceleration_out_of_range;
        stream_message.m_error_acceleration_calibration_failed = __oe_error_acceleration_calibration_failed;
        stream_message.m_error_invalid_target_acceleration = __oe_error_invalid_target_acceleration;
        stream_message.m_error_board_temperature1_invalid = __oe_error_board_temperature1_invalid;
        stream_message.m_error_board_temperature2_invalid = __oe_error_board_temperature2_invalid;
        stream_message.m_error_board_temperature3_invalid = __oe_error_board_temperature3_invalid;
        stream_message.m_error_mcu_temperature_invalid = __oe_error_mcu_temperature_invalid;
        stream_message.m_error_lim_temperature1_invalid = __oe_error_lim_temperature1_invalid;
        stream_message.m_error_lim_temperature2_invalid = __oe_error_lim_temperature2_invalid;
        stream_message.m_error_lim_temperature3_invalid = __oe_error_lim_temperature3_invalid;
        stream_message.m_error_lim_temperature4_invalid = __oe_error_lim_temperature4_invalid;
        stream_message.m_error_heartbeat_miss = __oe_error_heartbeat_miss;
        stream_message.m_error_level_current_u1 = __oe_error_level_current_u1;
        stream_message.m_error_level_current_v1 = __oe_error_level_current_v1;
        stream_message.m_error_level_current_w1 = __oe_error_level_current_w1;
        stream_message.m_error_level_current_u2 = __oe_error_level_current_u2;
        stream_message.m_error_level_current_v2 = __oe_error_level_current_v2;
        stream_message.m_error_level_current_w2 = __oe_error_level_current_w2;
        stream_message.m_error_level_vdc_voltage = __oe_error_level_vdc_voltage;
        stream_message.m_error_level_board_temperature = __oe_error_level_board_temperature;
        stream_message.m_error_level_mcu_temperature = __oe_error_level_mcu_temperature;
        stream_message.m_error_level_lim_temperature = __oe_error_level_lim_temperature;
        stream_message.m_error_vdc_voltage_invalid = __oe_error_vdc_voltage_invalid;
        stream_message.m_last_node_missed = __oe_last_node_missed;
        canzero_frame stream_frame;
        canzero_serialize_canzero_message_motor_driver_stream_errors(&stream_message, &stream_frame);
        canzero_can1_send(&stream_frame);
        break;
      }
      case 3: {
        job->job.stream_job.last_schedule = time;
        scheduler_reschedule(time + 500);
        canzero_exit_critical();
        canzero_message_motor_driver_stream_debug stream_message;
        stream_message.m_loop_frequency = __oe_loop_frequency;
        canzero_frame stream_frame;
        canzero_serialize_canzero_message_motor_driver_stream_debug(&stream_message, &stream_frame);
        canzero_can0_send(&stream_frame);
        break;
      }
      case 4: {
        job->job.stream_job.last_schedule = time;
        scheduler_reschedule(time + 50);
        canzero_exit_critical();
        canzero_message_motor_driver_stream_currents_u1v1w1 stream_message;
        stream_message.m_current_u1 = __oe_current_u1;
        stream_message.m_current_v1 = __oe_current_v1;
        stream_message.m_current_w1 = __oe_current_w1;
        stream_message.m_current_max = __oe_current_max;
        canzero_frame stream_frame;
        canzero_serialize_canzero_message_motor_driver_stream_currents_u1v1w1(&stream_message, &stream_frame);
        canzero_can1_send(&stream_frame);
        break;
      }
      case 5: {
        job->job.stream_job.last_schedule = time;
        scheduler_reschedule(time + 50);
        canzero_exit_critical();
        canzero_message_motor_driver_stream_current_u2v2w2 stream_message;
        stream_message.m_current_u2 = __oe_current_u2;
        stream_message.m_current_v2 = __oe_current_v2;
        stream_message.m_current_w2 = __oe_current_w2;
        stream_message.m_current_average = __oe_current_average;
        canzero_frame stream_frame;
        canzero_serialize_canzero_message_motor_driver_stream_current_u2v2w2(&stream_message, &stream_frame);
        canzero_can0_send(&stream_frame);
        break;
      }
      case 6: {
        job->job.stream_job.last_schedule = time;
        scheduler_reschedule(time + 100);
        canzero_exit_critical();
        canzero_message_motor_driver_stream_control_info stream_message;
        stream_message.m_frequency = __oe_frequency;
        stream_message.m_modulation_index = __oe_modulation_index;
        stream_message.m_acceleration = __oe_acceleration;
        stream_message.m_vdc_voltage = __oe_vdc_voltage;
        canzero_frame stream_frame;
        canzero_serialize_canzero_message_motor_driver_stream_control_info(&stream_message, &stream_frame);
        canzero_can0_send(&stream_frame);
        break;
      }
      case 7: {
        job->job.stream_job.last_schedule = time;
        scheduler_reschedule(time + 250);
        canzero_exit_critical();
        canzero_message_motor_driver_stream_board_temperature stream_message;
        stream_message.m_board_temperature1 = __oe_board_temperature1;
        stream_message.m_board_temperature2 = __oe_board_temperature2;
        stream_message.m_board_temperature3 = __oe_board_temperature3;
        stream_message.m_board_avg_temperature = __oe_board_avg_temperature;
        stream_message.m_board_max_temperature = __oe_board_max_temperature;
        stream_message.m_board_min_temperature = __oe_board_min_temperature;
        stream_message.m_mcu_temperature = __oe_mcu_temperature;
        canzero_frame stream_frame;
        canzero_serialize_canzero_message_motor_driver_stream_board_temperature(&stream_message, &stream_frame);
        canzero_can0_send(&stream_frame);
        break;
      }
      case 8: {
        job->job.stream_job.last_schedule = time;
        scheduler_reschedule(time + 250);
        canzero_exit_critical();
        canzero_message_motor_driver_stream_lim_temperature stream_message;
        stream_message.m_lim_temperature1 = __oe_lim_temperature1;
        stream_message.m_lim_temperature2 = __oe_lim_temperature2;
        stream_message.m_lim_temperature3 = __oe_lim_temperature3;
        stream_message.m_lim_temperature4 = __oe_lim_temperature4;
        stream_message.m_lim_average_temperature = __oe_lim_average_temperature;
        stream_message.m_lim_max_temperature = __oe_lim_max_temperature;
        stream_message.m_lim_min_temperature = __oe_lim_min_temperature;
        canzero_frame stream_frame;
        canzero_serialize_canzero_message_motor_driver_stream_lim_temperature(&stream_message, &stream_frame);
        canzero_can1_send(&stream_frame);
        break;
      }
        default:
          canzero_exit_critical();
          break;
        }
        break;
      }
      case HEARTBEAT_JOB_TAG: {
        scheduler_reschedule(time + heartbeat_interval);
        canzero_exit_critical();
        canzero_frame heartbeat_frame;
        canzero_message_heartbeat_can0 heartbeat_can0;
        heartbeat_can0.m_node_id = node_id_motor_driver;
        heartbeat_can0.m_unregister = 0;
        heartbeat_can0.m_ticks_next = 20;
        canzero_serialize_canzero_message_heartbeat_can0(&heartbeat_can0, &heartbeat_frame);
        canzero_can0_send(&heartbeat_frame);
        canzero_message_heartbeat_can1 heartbeat_can1;
        heartbeat_can1.m_node_id = node_id_motor_driver;
        heartbeat_can1.m_unregister = 0;
        heartbeat_can1.m_ticks_next = 20;
        canzero_serialize_canzero_message_heartbeat_can1(&heartbeat_can1, &heartbeat_frame);
        canzero_can1_send(&heartbeat_frame);
        break;
      }
      case HEARTBEAT_WDG_JOB_TAG: {
        scheduler_reschedule(time + heartbeat_wdg_tick_duration);
        canzero_exit_critical();
        for (unsigned int i = 0; i < node_id_count; ++i) {
          heartbeat_wdg_job.job.wdg_job.can0_static_tick_countdowns[i] 
            -= heartbeat_wdg_job.job.wdg_job.can0_static_wdg_armed[i];
          heartbeat_wdg_job.job.wdg_job.can1_static_tick_countdowns[i] 
            -= heartbeat_wdg_job.job.wdg_job.can1_static_wdg_armed[i];
        }
        for (unsigned int i = 0; i < MAX_DYN_HEARTBEATS; ++i) {
          heartbeat_wdg_job.job.wdg_job.can0_dynamic_tick_countdowns[i] 
            -= heartbeat_wdg_job.job.wdg_job.can0_dynamic_wdg_armed[i];
          heartbeat_wdg_job.job.wdg_job.can1_dynamic_tick_countdowns[i] 
            -= heartbeat_wdg_job.job.wdg_job.can1_dynamic_wdg_armed[i];
        }
        for (unsigned int i = 0; i < node_id_count; ++i) {
          if (heartbeat_wdg_job.job.wdg_job.can0_static_tick_countdowns[i] <= 0) {
            canzero_can0_wdg_timeout(i);
          }
          if (heartbeat_wdg_job.job.wdg_job.can1_static_tick_countdowns[i] <= 0) {
            canzero_can1_wdg_timeout(i);
          }
        }
        for (unsigned int i = 0; i < MAX_DYN_HEARTBEATS; ++i) {
          if (heartbeat_wdg_job.job.wdg_job.can0_dynamic_tick_countdowns[i] <= 0) {
            canzero_can0_wdg_timeout(node_id_count + i);
          }
          if (heartbeat_wdg_job.job.wdg_job.can1_dynamic_tick_countdowns[i] <= 0) {
            canzero_can1_wdg_timeout(node_id_count + i);
          }
        }
        break;
      }
      case GET_RESP_FRAGMENTATION_JOB_TAG: {
        get_resp_fragmentation_job *fragmentation_job = &job->job.get_fragmentation_job;
        canzero_message_get_resp fragmentation_response;
        fragmentation_response.m_header.m_sof = 0;
        fragmentation_response.m_header.m_toggle = fragmentation_job->offset % 2;
        fragmentation_response.m_header.m_od_index = fragmentation_job->od_index;
        fragmentation_response.m_header.m_client_id = fragmentation_job->client_id;
        fragmentation_response.m_header.m_server_id = 0x2;
        fragmentation_response.m_data = fragmentation_job->buffer[fragmentation_job->offset];
        fragmentation_job->offset += 1;
        if (fragmentation_job->offset == fragmentation_job->size) {
          fragmentation_response.m_header.m_eof = 1;
          scheduler_unschedule();
        } else {
          fragmentation_response.m_header.m_eof = 0;
          scheduler_reschedule(time + get_resp_fragmentation_interval);
        }
        canzero_exit_critical();
        canzero_frame fragmentation_frame;
        canzero_serialize_canzero_message_get_resp(&fragmentation_response, &fragmentation_frame);
        canzero_can0_send(&fragmentation_frame);
        break;
      }
      default: {
        canzero_exit_critical();
        break;
      }
    }
  }
}

static uint32_t scheduler_next_job_timeout() {
  return scheduler.heap[0]->climax;
}

static uint32_t DMAMEM __oe_config_hash_rx_fragmentation_buffer[2];
static uint32_t DMAMEM __oe_build_time_rx_fragmentation_buffer[2];
static uint32_t DMAMEM __oe_error_level_config_current_phase_rx_fragmentation_buffer[7];
static uint32_t DMAMEM __oe_acceleration_calibration_variance_rx_fragmentation_buffer[2];
static uint32_t DMAMEM __oe_acceleration_calibration_offset_rx_fragmentation_buffer[2];
static uint32_t DMAMEM __oe_error_level_config_vdc_voltage_rx_fragmentation_buffer[7];
static uint32_t DMAMEM __oe_error_level_config_board_temperature_rx_fragmentation_buffer[7];
static uint32_t DMAMEM __oe_error_level_config_mcu_temperature_rx_fragmentation_buffer[7];
static uint32_t DMAMEM __oe_error_level_config_lim_temperature_rx_fragmentation_buffer[7];
static PROGMEM void canzero_handle_get_req(canzero_frame* frame) {
  canzero_message_get_req msg;
  canzero_deserialize_canzero_message_get_req(frame, &msg);
  if (msg.m_header.m_server_id != node_id_motor_driver) {
    return;
  }
  canzero_message_get_resp resp{};
  switch (msg.m_header.m_od_index) {
  case 0: {
    {
      uint64_t masked = (__oe_config_hash & (0xFFFFFFFFFFFFFFFF >> (64 - 64)));
      __oe_config_hash_rx_fragmentation_buffer[0] = ((uint32_t*)&masked)[0];
      __oe_config_hash_rx_fragmentation_buffer[1] = ((uint32_t*)&masked)[1];
    }
    resp.m_data = __oe_config_hash_rx_fragmentation_buffer[0];
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 0;
    resp.m_header.m_toggle = 0;
    schedule_get_resp_fragmentation_job(__oe_config_hash_rx_fragmentation_buffer, 2, 0, msg.m_header.m_client_id);
    break;
  }
  case 1: {
    __oe_build_time_rx_fragmentation_buffer[0] = (__oe_build_time.m_year & (0xFFFFFFFF >> (32 - 16)));
    __oe_build_time_rx_fragmentation_buffer[0] |= ((__oe_build_time.m_month & (0xFFFFFFFF >> (32 - 8))) << 16);
    __oe_build_time_rx_fragmentation_buffer[0] |= ((__oe_build_time.m_day & (0xFFFFFFFF >> (32 - 8))) << 24);
    __oe_build_time_rx_fragmentation_buffer[1] = (__oe_build_time.m_hour & (0xFFFFFFFF >> (32 - 8)));
    __oe_build_time_rx_fragmentation_buffer[1] |= ((__oe_build_time.m_min & (0xFFFFFFFF >> (32 - 8))) << 8);
    __oe_build_time_rx_fragmentation_buffer[1] |= ((__oe_build_time.m_sec & (0xFFFFFFFF >> (32 - 8))) << 16);

    resp.m_data = __oe_build_time_rx_fragmentation_buffer[0];
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 0;
    resp.m_header.m_toggle = 0;
    schedule_get_resp_fragmentation_job(__oe_build_time_rx_fragmentation_buffer, 2, 1, msg.m_header.m_client_id);
    break;
  }
  case 2: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_state) & (0xFF >> (8 - 3)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 3: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_sdc_status) & (0xFF >> (8 - 1)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 4: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_assertion_fault) & (0xFF >> (8 - 1)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 5: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_error_arming_failed) & (0xFF >> (8 - 1)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 6: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_error_precharge_failed) & (0xFF >> (8 - 1)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 7: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_error_acceleration_out_of_range) & (0xFF >> (8 - 1)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 8: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_error_acceleration_calibration_failed) & (0xFF >> (8 - 1)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 9: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_error_invalid_target_acceleration) & (0xFF >> (8 - 1)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 10: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_error_board_temperature1_invalid) & (0xFF >> (8 - 1)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 11: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_error_board_temperature2_invalid) & (0xFF >> (8 - 1)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 12: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_error_board_temperature3_invalid) & (0xFF >> (8 - 1)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 13: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_error_mcu_temperature_invalid) & (0xFF >> (8 - 1)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 14: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_error_lim_temperature1_invalid) & (0xFF >> (8 - 1)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 15: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_error_lim_temperature2_invalid) & (0xFF >> (8 - 1)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 16: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_error_lim_temperature3_invalid) & (0xFF >> (8 - 1)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 17: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_error_lim_temperature4_invalid) & (0xFF >> (8 - 1)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 18: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_error_heartbeat_miss) & (0xFF >> (8 - 1)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 19: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_error_level_current_u1) & (0xFF >> (8 - 2)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 20: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_error_level_current_v1) & (0xFF >> (8 - 2)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 21: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_error_level_current_w1) & (0xFF >> (8 - 2)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 22: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_error_level_current_u2) & (0xFF >> (8 - 2)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 23: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_error_level_current_v2) & (0xFF >> (8 - 2)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 24: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_error_level_current_w2) & (0xFF >> (8 - 2)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 25: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_error_level_vdc_voltage) & (0xFF >> (8 - 2)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 26: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_error_level_board_temperature) & (0xFF >> (8 - 2)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 27: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_error_level_mcu_temperature) & (0xFF >> (8 - 2)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 28: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_error_level_lim_temperature) & (0xFF >> (8 - 2)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 29: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_precharge_status) & (0xFF >> (8 - 1)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 30: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_feedthrough_status) & (0xFF >> (8 - 1)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 31: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_command) & (0xFF >> (8 - 3)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 32: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_control_active) & (0xFF >> (8 - 1)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 33: {
    resp.m_data |= min_u32((__oe_current_u1 - (-100)) / 0.0030518043793392844, 0xFFFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 34: {
    resp.m_data |= min_u32((__oe_current_v1 - (-100)) / 0.0030518043793392844, 0xFFFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 35: {
    resp.m_data |= min_u32((__oe_current_w1 - (-100)) / 0.0030518043793392844, 0xFFFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 36: {
    resp.m_data |= min_u32((__oe_current_u2 - (-100)) / 0.0030518043793392844, 0xFFFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 37: {
    resp.m_data |= min_u32((__oe_current_v2 - (-100)) / 0.0030518043793392844, 0xFFFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 38: {
    resp.m_data |= min_u32((__oe_current_w2 - (-100)) / 0.0030518043793392844, 0xFFFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 39: {
    resp.m_data |= min_u32((__oe_current_max - (-100)) / 0.0030518043793392844, 0xFFFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 40: {
    resp.m_data |= min_u32((__oe_current_average - (-100)) / 0.0030518043793392844, 0xFFFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 41: {
    __oe_error_level_config_current_phase_rx_fragmentation_buffer[0] = (min_u32((__oe_error_level_config_current_phase.m_info_thresh - ((float)-1000)) / (float)0.0000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
    __oe_error_level_config_current_phase_rx_fragmentation_buffer[1] = (min_u32((__oe_error_level_config_current_phase.m_info_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
    __oe_error_level_config_current_phase_rx_fragmentation_buffer[2] = (min_u32((__oe_error_level_config_current_phase.m_warning_thresh - ((float)-1000)) / (float)0.0000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
    __oe_error_level_config_current_phase_rx_fragmentation_buffer[3] = (min_u32((__oe_error_level_config_current_phase.m_warning_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
    __oe_error_level_config_current_phase_rx_fragmentation_buffer[4] = (min_u32((__oe_error_level_config_current_phase.m_error_thresh - ((float)-1000)) / (float)0.0000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
    __oe_error_level_config_current_phase_rx_fragmentation_buffer[5] = (min_u32((__oe_error_level_config_current_phase.m_error_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
    __oe_error_level_config_current_phase_rx_fragmentation_buffer[6] = (__oe_error_level_config_current_phase.m_ignore_info & (0xFFFFFFFF >> (32 - 1)));
    __oe_error_level_config_current_phase_rx_fragmentation_buffer[6] |= ((__oe_error_level_config_current_phase.m_ignore_warning & (0xFFFFFFFF >> (32 - 1))) << 1);
    __oe_error_level_config_current_phase_rx_fragmentation_buffer[6] |= ((__oe_error_level_config_current_phase.m_ignore_error & (0xFFFFFFFF >> (32 - 1))) << 2);

    resp.m_data = __oe_error_level_config_current_phase_rx_fragmentation_buffer[0];
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 0;
    resp.m_header.m_toggle = 0;
    schedule_get_resp_fragmentation_job(__oe_error_level_config_current_phase_rx_fragmentation_buffer, 7, 41, msg.m_header.m_client_id);
    break;
  }
  case 42: {
    resp.m_data |= min_u32((__oe_frequency - (0)) / 0.0007629510948348211, 0xFFFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 43: {
    resp.m_data |= min_u32((__oe_modulation_index - (0)) / 0.000015259021896696422, 0xFFFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 44: {
    resp.m_data |= min_u32((__oe_acceleration - (-10)) / 0.00030518043793392844, 0xFFFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 45: {
    {
      uint64_t masked = (min_u64((__oe_acceleration_calibration_variance - ((double)-1)) / (double)0.00000000000000000010842021724855044, 0xFFFFFFFFFFFFFFFFull) & (0xFFFFFFFFFFFFFFFF >> (64 - 64)));
      __oe_acceleration_calibration_variance_rx_fragmentation_buffer[0] = ((uint32_t*)&masked)[0];
      __oe_acceleration_calibration_variance_rx_fragmentation_buffer[1] = ((uint32_t*)&masked)[1];
    }
    resp.m_data = __oe_acceleration_calibration_variance_rx_fragmentation_buffer[0];
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 0;
    resp.m_header.m_toggle = 0;
    schedule_get_resp_fragmentation_job(__oe_acceleration_calibration_variance_rx_fragmentation_buffer, 2, 45, msg.m_header.m_client_id);
    break;
  }
  case 46: {
    {
      uint64_t masked = (min_u64((__oe_acceleration_calibration_offset - ((double)-1)) / (double)0.00000000000000000010842021724855044, 0xFFFFFFFFFFFFFFFFull) & (0xFFFFFFFFFFFFFFFF >> (64 - 64)));
      __oe_acceleration_calibration_offset_rx_fragmentation_buffer[0] = ((uint32_t*)&masked)[0];
      __oe_acceleration_calibration_offset_rx_fragmentation_buffer[1] = ((uint32_t*)&masked)[1];
    }
    resp.m_data = __oe_acceleration_calibration_offset_rx_fragmentation_buffer[0];
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 0;
    resp.m_header.m_toggle = 0;
    schedule_get_resp_fragmentation_job(__oe_acceleration_calibration_offset_rx_fragmentation_buffer, 2, 46, msg.m_header.m_client_id);
    break;
  }
  case 47: {
    resp.m_data |= min_u32((__oe_target_acceleration - (-50)) / 0.000000023283064370807974, 0xFFFFFFFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 48: {
    resp.m_data |= min_u32((__oe_external_acceleration - (-10)) / 0.00030518043793392844, 0xFFFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 49: {
    resp.m_data |= min_u32((__oe_external_velocity - (-10)) / 0.00030518043793392844, 0xFFFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 50: {
    resp.m_data |= min_u32((__oe_external_position - (-50)) / 0.0015259021896696422, 0xFFFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 51: {
    resp.m_data |= min_u32((__oe_vdc_voltage - (-100)) / 0.0030518043793392844, 0xFFFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 52: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_error_vdc_voltage_invalid) & (0xFF >> (8 - 1)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 53: {
    __oe_error_level_config_vdc_voltage_rx_fragmentation_buffer[0] = (min_u32((__oe_error_level_config_vdc_voltage.m_info_thresh - ((float)-1000)) / (float)0.0000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
    __oe_error_level_config_vdc_voltage_rx_fragmentation_buffer[1] = (min_u32((__oe_error_level_config_vdc_voltage.m_info_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
    __oe_error_level_config_vdc_voltage_rx_fragmentation_buffer[2] = (min_u32((__oe_error_level_config_vdc_voltage.m_warning_thresh - ((float)-1000)) / (float)0.0000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
    __oe_error_level_config_vdc_voltage_rx_fragmentation_buffer[3] = (min_u32((__oe_error_level_config_vdc_voltage.m_warning_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
    __oe_error_level_config_vdc_voltage_rx_fragmentation_buffer[4] = (min_u32((__oe_error_level_config_vdc_voltage.m_error_thresh - ((float)-1000)) / (float)0.0000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
    __oe_error_level_config_vdc_voltage_rx_fragmentation_buffer[5] = (min_u32((__oe_error_level_config_vdc_voltage.m_error_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
    __oe_error_level_config_vdc_voltage_rx_fragmentation_buffer[6] = (__oe_error_level_config_vdc_voltage.m_ignore_info & (0xFFFFFFFF >> (32 - 1)));
    __oe_error_level_config_vdc_voltage_rx_fragmentation_buffer[6] |= ((__oe_error_level_config_vdc_voltage.m_ignore_warning & (0xFFFFFFFF >> (32 - 1))) << 1);
    __oe_error_level_config_vdc_voltage_rx_fragmentation_buffer[6] |= ((__oe_error_level_config_vdc_voltage.m_ignore_error & (0xFFFFFFFF >> (32 - 1))) << 2);

    resp.m_data = __oe_error_level_config_vdc_voltage_rx_fragmentation_buffer[0];
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 0;
    resp.m_header.m_toggle = 0;
    schedule_get_resp_fragmentation_job(__oe_error_level_config_vdc_voltage_rx_fragmentation_buffer, 7, 53, msg.m_header.m_client_id);
    break;
  }
  case 54: {
    resp.m_data |= min_u32((__oe_board_temperature1 - (-1)) / 0.592156862745098, 0xFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 55: {
    resp.m_data |= min_u32((__oe_board_temperature2 - (-1)) / 0.592156862745098, 0xFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 56: {
    resp.m_data |= min_u32((__oe_board_temperature3 - (-1)) / 0.592156862745098, 0xFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 57: {
    resp.m_data |= min_u32((__oe_board_avg_temperature - (-1)) / 0.592156862745098, 0xFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 58: {
    resp.m_data |= min_u32((__oe_board_max_temperature - (-1)) / 0.592156862745098, 0xFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 59: {
    resp.m_data |= min_u32((__oe_board_min_temperature - (-1)) / 0.592156862745098, 0xFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 60: {
    __oe_error_level_config_board_temperature_rx_fragmentation_buffer[0] = (min_u32((__oe_error_level_config_board_temperature.m_info_thresh - ((float)-1000)) / (float)0.0000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
    __oe_error_level_config_board_temperature_rx_fragmentation_buffer[1] = (min_u32((__oe_error_level_config_board_temperature.m_info_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
    __oe_error_level_config_board_temperature_rx_fragmentation_buffer[2] = (min_u32((__oe_error_level_config_board_temperature.m_warning_thresh - ((float)-1000)) / (float)0.0000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
    __oe_error_level_config_board_temperature_rx_fragmentation_buffer[3] = (min_u32((__oe_error_level_config_board_temperature.m_warning_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
    __oe_error_level_config_board_temperature_rx_fragmentation_buffer[4] = (min_u32((__oe_error_level_config_board_temperature.m_error_thresh - ((float)-1000)) / (float)0.0000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
    __oe_error_level_config_board_temperature_rx_fragmentation_buffer[5] = (min_u32((__oe_error_level_config_board_temperature.m_error_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
    __oe_error_level_config_board_temperature_rx_fragmentation_buffer[6] = (__oe_error_level_config_board_temperature.m_ignore_info & (0xFFFFFFFF >> (32 - 1)));
    __oe_error_level_config_board_temperature_rx_fragmentation_buffer[6] |= ((__oe_error_level_config_board_temperature.m_ignore_warning & (0xFFFFFFFF >> (32 - 1))) << 1);
    __oe_error_level_config_board_temperature_rx_fragmentation_buffer[6] |= ((__oe_error_level_config_board_temperature.m_ignore_error & (0xFFFFFFFF >> (32 - 1))) << 2);

    resp.m_data = __oe_error_level_config_board_temperature_rx_fragmentation_buffer[0];
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 0;
    resp.m_header.m_toggle = 0;
    schedule_get_resp_fragmentation_job(__oe_error_level_config_board_temperature_rx_fragmentation_buffer, 7, 60, msg.m_header.m_client_id);
    break;
  }
  case 61: {
    resp.m_data |= min_u32((__oe_mcu_temperature - (-1)) / 0.592156862745098, 0xFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 62: {
    __oe_error_level_config_mcu_temperature_rx_fragmentation_buffer[0] = (min_u32((__oe_error_level_config_mcu_temperature.m_info_thresh - ((float)-1000)) / (float)0.0000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
    __oe_error_level_config_mcu_temperature_rx_fragmentation_buffer[1] = (min_u32((__oe_error_level_config_mcu_temperature.m_info_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
    __oe_error_level_config_mcu_temperature_rx_fragmentation_buffer[2] = (min_u32((__oe_error_level_config_mcu_temperature.m_warning_thresh - ((float)-1000)) / (float)0.0000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
    __oe_error_level_config_mcu_temperature_rx_fragmentation_buffer[3] = (min_u32((__oe_error_level_config_mcu_temperature.m_warning_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
    __oe_error_level_config_mcu_temperature_rx_fragmentation_buffer[4] = (min_u32((__oe_error_level_config_mcu_temperature.m_error_thresh - ((float)-1000)) / (float)0.0000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
    __oe_error_level_config_mcu_temperature_rx_fragmentation_buffer[5] = (min_u32((__oe_error_level_config_mcu_temperature.m_error_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
    __oe_error_level_config_mcu_temperature_rx_fragmentation_buffer[6] = (__oe_error_level_config_mcu_temperature.m_ignore_info & (0xFFFFFFFF >> (32 - 1)));
    __oe_error_level_config_mcu_temperature_rx_fragmentation_buffer[6] |= ((__oe_error_level_config_mcu_temperature.m_ignore_warning & (0xFFFFFFFF >> (32 - 1))) << 1);
    __oe_error_level_config_mcu_temperature_rx_fragmentation_buffer[6] |= ((__oe_error_level_config_mcu_temperature.m_ignore_error & (0xFFFFFFFF >> (32 - 1))) << 2);

    resp.m_data = __oe_error_level_config_mcu_temperature_rx_fragmentation_buffer[0];
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 0;
    resp.m_header.m_toggle = 0;
    schedule_get_resp_fragmentation_job(__oe_error_level_config_mcu_temperature_rx_fragmentation_buffer, 7, 62, msg.m_header.m_client_id);
    break;
  }
  case 63: {
    resp.m_data |= min_u32((__oe_lim_temperature1 - (-1)) / 0.592156862745098, 0xFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 64: {
    resp.m_data |= min_u32((__oe_lim_temperature2 - (-1)) / 0.592156862745098, 0xFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 65: {
    resp.m_data |= min_u32((__oe_lim_temperature3 - (-1)) / 0.592156862745098, 0xFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 66: {
    resp.m_data |= min_u32((__oe_lim_temperature4 - (-1)) / 0.592156862745098, 0xFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 67: {
    resp.m_data |= min_u32((__oe_lim_average_temperature - (-1)) / 0.592156862745098, 0xFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 68: {
    resp.m_data |= min_u32((__oe_lim_max_temperature - (-1)) / 0.592156862745098, 0xFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 69: {
    resp.m_data |= min_u32((__oe_lim_min_temperature - (-1)) / 0.592156862745098, 0xFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 70: {
    __oe_error_level_config_lim_temperature_rx_fragmentation_buffer[0] = (min_u32((__oe_error_level_config_lim_temperature.m_info_thresh - ((float)-1000)) / (float)0.0000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
    __oe_error_level_config_lim_temperature_rx_fragmentation_buffer[1] = (min_u32((__oe_error_level_config_lim_temperature.m_info_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
    __oe_error_level_config_lim_temperature_rx_fragmentation_buffer[2] = (min_u32((__oe_error_level_config_lim_temperature.m_warning_thresh - ((float)-1000)) / (float)0.0000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
    __oe_error_level_config_lim_temperature_rx_fragmentation_buffer[3] = (min_u32((__oe_error_level_config_lim_temperature.m_warning_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
    __oe_error_level_config_lim_temperature_rx_fragmentation_buffer[4] = (min_u32((__oe_error_level_config_lim_temperature.m_error_thresh - ((float)-1000)) / (float)0.0000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
    __oe_error_level_config_lim_temperature_rx_fragmentation_buffer[5] = (min_u32((__oe_error_level_config_lim_temperature.m_error_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
    __oe_error_level_config_lim_temperature_rx_fragmentation_buffer[6] = (__oe_error_level_config_lim_temperature.m_ignore_info & (0xFFFFFFFF >> (32 - 1)));
    __oe_error_level_config_lim_temperature_rx_fragmentation_buffer[6] |= ((__oe_error_level_config_lim_temperature.m_ignore_warning & (0xFFFFFFFF >> (32 - 1))) << 1);
    __oe_error_level_config_lim_temperature_rx_fragmentation_buffer[6] |= ((__oe_error_level_config_lim_temperature.m_ignore_error & (0xFFFFFFFF >> (32 - 1))) << 2);

    resp.m_data = __oe_error_level_config_lim_temperature_rx_fragmentation_buffer[0];
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 0;
    resp.m_header.m_toggle = 0;
    schedule_get_resp_fragmentation_job(__oe_error_level_config_lim_temperature_rx_fragmentation_buffer, 7, 70, msg.m_header.m_client_id);
    break;
  }
  case 71: {
    resp.m_data |= min_u32((__oe_loop_frequency - (0)) / 0.00000023283064370807974, 0xFFFFFFFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 72: {
    resp.m_data |= ((uint32_t)(__oe_last_node_missed & (0xFF >> (8 - 8)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 73: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_ignore_45v) & (0xFF >> (8 - 1)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  }
  resp.m_header.m_od_index = msg.m_header.m_od_index;
  resp.m_header.m_client_id = msg.m_header.m_client_id;
  resp.m_header.m_server_id = msg.m_header.m_server_id;
  canzero_frame resp_frame;
  canzero_serialize_canzero_message_get_resp(&resp, &resp_frame);
  canzero_can0_send(&resp_frame);
}
static uint32_t DMAMEM config_hash_tmp_tx_fragmentation_buffer[2];
static uint32_t DMAMEM config_hash_tmp_tx_fragmentation_offset = 0;
static uint32_t DMAMEM build_time_tmp_tx_fragmentation_buffer[2];
static uint32_t DMAMEM build_time_tmp_tx_fragmentation_offset = 0;
static uint32_t DMAMEM error_level_config_current_phase_tmp_tx_fragmentation_buffer[7];
static uint32_t DMAMEM error_level_config_current_phase_tmp_tx_fragmentation_offset = 0;
static uint32_t DMAMEM acceleration_calibration_variance_tmp_tx_fragmentation_buffer[2];
static uint32_t DMAMEM acceleration_calibration_variance_tmp_tx_fragmentation_offset = 0;
static uint32_t DMAMEM acceleration_calibration_offset_tmp_tx_fragmentation_buffer[2];
static uint32_t DMAMEM acceleration_calibration_offset_tmp_tx_fragmentation_offset = 0;
static uint32_t DMAMEM error_level_config_vdc_voltage_tmp_tx_fragmentation_buffer[7];
static uint32_t DMAMEM error_level_config_vdc_voltage_tmp_tx_fragmentation_offset = 0;
static uint32_t DMAMEM error_level_config_board_temperature_tmp_tx_fragmentation_buffer[7];
static uint32_t DMAMEM error_level_config_board_temperature_tmp_tx_fragmentation_offset = 0;
static uint32_t DMAMEM error_level_config_mcu_temperature_tmp_tx_fragmentation_buffer[7];
static uint32_t DMAMEM error_level_config_mcu_temperature_tmp_tx_fragmentation_offset = 0;
static uint32_t DMAMEM error_level_config_lim_temperature_tmp_tx_fragmentation_buffer[7];
static uint32_t DMAMEM error_level_config_lim_temperature_tmp_tx_fragmentation_offset = 0;
static PROGMEM void canzero_handle_set_req(canzero_frame* frame) {
  canzero_message_set_req msg;
  canzero_deserialize_canzero_message_set_req(frame, &msg);
  if (msg.m_header.m_server_id != 2) {
    return;
  }
  canzero_message_set_resp resp{};
  switch (msg.m_header.m_od_index) {
  case 0 : {
    if (msg.m_header.m_sof == 1) {
      if (msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 0) {
        return; //TODO proper error response frame!
      }
      config_hash_tmp_tx_fragmentation_offset = 0;
    }else {
      config_hash_tmp_tx_fragmentation_offset += 1;
      if (config_hash_tmp_tx_fragmentation_offset >= 2) {
        return;
      }
    }
    config_hash_tmp_tx_fragmentation_buffer[config_hash_tmp_tx_fragmentation_offset] = msg.m_data;
    if (msg.m_header.m_eof == 0) {
      return;
    }
    uint64_t config_hash_tmp;
    config_hash_tmp = (uint64_t)config_hash_tmp_tx_fragmentation_buffer[0] | (((uint64_t)(config_hash_tmp_tx_fragmentation_buffer[1] & (0xFFFFFFFF >> (32 - 32)))) << 32);
    canzero_set_config_hash(config_hash_tmp);
    break;
  }
  case 1 : {
    if (msg.m_header.m_sof == 1) {
      if (msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 0) {
        return; //TODO proper error response frame!
      }
      build_time_tmp_tx_fragmentation_offset = 0;
    }else {
      build_time_tmp_tx_fragmentation_offset += 1;
      if (build_time_tmp_tx_fragmentation_offset >= 2) {
        return;
      }
    }
    build_time_tmp_tx_fragmentation_buffer[build_time_tmp_tx_fragmentation_offset] = msg.m_data;
    if (msg.m_header.m_eof == 0) {
      return;
    }
    date_time build_time_tmp;
    build_time_tmp.m_year = (build_time_tmp_tx_fragmentation_buffer[0] & (0xFFFFFFFF >> (32 - 16)));
    build_time_tmp.m_month = (build_time_tmp_tx_fragmentation_buffer[0] >> 16) & (0xFFFFFFFF >> (32 - 8));
    build_time_tmp.m_day = (build_time_tmp_tx_fragmentation_buffer[0] >> 24) & (0xFFFFFFFF >> (32 - 8));
    build_time_tmp.m_hour = (build_time_tmp_tx_fragmentation_buffer[1] & (0xFFFFFFFF >> (32 - 8)));
    build_time_tmp.m_min = (build_time_tmp_tx_fragmentation_buffer[1] >> 8) & (0xFFFFFFFF >> (32 - 8));
    build_time_tmp.m_sec = (build_time_tmp_tx_fragmentation_buffer[1] >> 16) & (0xFFFFFFFF >> (32 - 8));
    canzero_set_build_time(build_time_tmp);
    break;
  }
  case 2 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    motor_state state_tmp;
    state_tmp = ((motor_state)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 3))));
    canzero_set_state(state_tmp);
    break;
  }
  case 3 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    sdc_status sdc_status_tmp;
    sdc_status_tmp = ((sdc_status)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_sdc_status(sdc_status_tmp);
    break;
  }
  case 4 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    error_flag assertion_fault_tmp;
    assertion_fault_tmp = ((error_flag)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_assertion_fault(assertion_fault_tmp);
    break;
  }
  case 5 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    error_flag error_arming_failed_tmp;
    error_arming_failed_tmp = ((error_flag)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_error_arming_failed(error_arming_failed_tmp);
    break;
  }
  case 6 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    error_flag error_precharge_failed_tmp;
    error_precharge_failed_tmp = ((error_flag)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_error_precharge_failed(error_precharge_failed_tmp);
    break;
  }
  case 7 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    error_flag error_acceleration_out_of_range_tmp;
    error_acceleration_out_of_range_tmp = ((error_flag)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_error_acceleration_out_of_range(error_acceleration_out_of_range_tmp);
    break;
  }
  case 8 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    error_flag error_acceleration_calibration_failed_tmp;
    error_acceleration_calibration_failed_tmp = ((error_flag)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_error_acceleration_calibration_failed(error_acceleration_calibration_failed_tmp);
    break;
  }
  case 9 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    error_flag error_invalid_target_acceleration_tmp;
    error_invalid_target_acceleration_tmp = ((error_flag)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_error_invalid_target_acceleration(error_invalid_target_acceleration_tmp);
    break;
  }
  case 10 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    error_flag error_board_temperature1_invalid_tmp;
    error_board_temperature1_invalid_tmp = ((error_flag)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_error_board_temperature1_invalid(error_board_temperature1_invalid_tmp);
    break;
  }
  case 11 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    error_flag error_board_temperature2_invalid_tmp;
    error_board_temperature2_invalid_tmp = ((error_flag)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_error_board_temperature2_invalid(error_board_temperature2_invalid_tmp);
    break;
  }
  case 12 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    error_flag error_board_temperature3_invalid_tmp;
    error_board_temperature3_invalid_tmp = ((error_flag)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_error_board_temperature3_invalid(error_board_temperature3_invalid_tmp);
    break;
  }
  case 13 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    error_flag error_mcu_temperature_invalid_tmp;
    error_mcu_temperature_invalid_tmp = ((error_flag)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_error_mcu_temperature_invalid(error_mcu_temperature_invalid_tmp);
    break;
  }
  case 14 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    error_flag error_lim_temperature1_invalid_tmp;
    error_lim_temperature1_invalid_tmp = ((error_flag)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_error_lim_temperature1_invalid(error_lim_temperature1_invalid_tmp);
    break;
  }
  case 15 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    error_flag error_lim_temperature2_invalid_tmp;
    error_lim_temperature2_invalid_tmp = ((error_flag)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_error_lim_temperature2_invalid(error_lim_temperature2_invalid_tmp);
    break;
  }
  case 16 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    error_flag error_lim_temperature3_invalid_tmp;
    error_lim_temperature3_invalid_tmp = ((error_flag)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_error_lim_temperature3_invalid(error_lim_temperature3_invalid_tmp);
    break;
  }
  case 17 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    error_flag error_lim_temperature4_invalid_tmp;
    error_lim_temperature4_invalid_tmp = ((error_flag)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_error_lim_temperature4_invalid(error_lim_temperature4_invalid_tmp);
    break;
  }
  case 18 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    error_flag error_heartbeat_miss_tmp;
    error_heartbeat_miss_tmp = ((error_flag)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_error_heartbeat_miss(error_heartbeat_miss_tmp);
    break;
  }
  case 19 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    error_level error_level_current_u1_tmp;
    error_level_current_u1_tmp = ((error_level)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 2))));
    canzero_set_error_level_current_u1(error_level_current_u1_tmp);
    break;
  }
  case 20 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    error_level error_level_current_v1_tmp;
    error_level_current_v1_tmp = ((error_level)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 2))));
    canzero_set_error_level_current_v1(error_level_current_v1_tmp);
    break;
  }
  case 21 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    error_level error_level_current_w1_tmp;
    error_level_current_w1_tmp = ((error_level)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 2))));
    canzero_set_error_level_current_w1(error_level_current_w1_tmp);
    break;
  }
  case 22 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    error_level error_level_current_u2_tmp;
    error_level_current_u2_tmp = ((error_level)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 2))));
    canzero_set_error_level_current_u2(error_level_current_u2_tmp);
    break;
  }
  case 23 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    error_level error_level_current_v2_tmp;
    error_level_current_v2_tmp = ((error_level)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 2))));
    canzero_set_error_level_current_v2(error_level_current_v2_tmp);
    break;
  }
  case 24 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    error_level error_level_current_w2_tmp;
    error_level_current_w2_tmp = ((error_level)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 2))));
    canzero_set_error_level_current_w2(error_level_current_w2_tmp);
    break;
  }
  case 25 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    error_level error_level_vdc_voltage_tmp;
    error_level_vdc_voltage_tmp = ((error_level)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 2))));
    canzero_set_error_level_vdc_voltage(error_level_vdc_voltage_tmp);
    break;
  }
  case 26 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    error_level error_level_board_temperature_tmp;
    error_level_board_temperature_tmp = ((error_level)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 2))));
    canzero_set_error_level_board_temperature(error_level_board_temperature_tmp);
    break;
  }
  case 27 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    error_level error_level_mcu_temperature_tmp;
    error_level_mcu_temperature_tmp = ((error_level)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 2))));
    canzero_set_error_level_mcu_temperature(error_level_mcu_temperature_tmp);
    break;
  }
  case 28 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    error_level error_level_lim_temperature_tmp;
    error_level_lim_temperature_tmp = ((error_level)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 2))));
    canzero_set_error_level_lim_temperature(error_level_lim_temperature_tmp);
    break;
  }
  case 29 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    sdc_status precharge_status_tmp;
    precharge_status_tmp = ((sdc_status)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_precharge_status(precharge_status_tmp);
    break;
  }
  case 30 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    sdc_status feedthrough_status_tmp;
    feedthrough_status_tmp = ((sdc_status)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_feedthrough_status(feedthrough_status_tmp);
    break;
  }
  case 31 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    motor_command command_tmp;
    command_tmp = ((motor_command)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 3))));
    canzero_set_command(command_tmp);
    break;
  }
  case 32 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    bool_t control_active_tmp;
    control_active_tmp = ((bool_t)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_control_active(control_active_tmp);
    break;
  }
  case 33 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float current_u1_tmp;
    current_u1_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 16))) * 0.0030518043793392844 + -100);
    canzero_set_current_u1(current_u1_tmp);
    break;
  }
  case 34 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float current_v1_tmp;
    current_v1_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 16))) * 0.0030518043793392844 + -100);
    canzero_set_current_v1(current_v1_tmp);
    break;
  }
  case 35 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float current_w1_tmp;
    current_w1_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 16))) * 0.0030518043793392844 + -100);
    canzero_set_current_w1(current_w1_tmp);
    break;
  }
  case 36 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float current_u2_tmp;
    current_u2_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 16))) * 0.0030518043793392844 + -100);
    canzero_set_current_u2(current_u2_tmp);
    break;
  }
  case 37 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float current_v2_tmp;
    current_v2_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 16))) * 0.0030518043793392844 + -100);
    canzero_set_current_v2(current_v2_tmp);
    break;
  }
  case 38 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float current_w2_tmp;
    current_w2_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 16))) * 0.0030518043793392844 + -100);
    canzero_set_current_w2(current_w2_tmp);
    break;
  }
  case 39 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float current_max_tmp;
    current_max_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 16))) * 0.0030518043793392844 + -100);
    canzero_set_current_max(current_max_tmp);
    break;
  }
  case 40 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float current_average_tmp;
    current_average_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 16))) * 0.0030518043793392844 + -100);
    canzero_set_current_average(current_average_tmp);
    break;
  }
  case 41 : {
    if (msg.m_header.m_sof == 1) {
      if (msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 0) {
        return; //TODO proper error response frame!
      }
      error_level_config_current_phase_tmp_tx_fragmentation_offset = 0;
    }else {
      error_level_config_current_phase_tmp_tx_fragmentation_offset += 1;
      if (error_level_config_current_phase_tmp_tx_fragmentation_offset >= 7) {
        return;
      }
    }
    error_level_config_current_phase_tmp_tx_fragmentation_buffer[error_level_config_current_phase_tmp_tx_fragmentation_offset] = msg.m_data;
    if (msg.m_header.m_eof == 0) {
      return;
    }
    error_level_config error_level_config_current_phase_tmp;
    error_level_config_current_phase_tmp.m_info_thresh = ((error_level_config_current_phase_tmp_tx_fragmentation_buffer[0] & (0xFFFFFFFF >> (32 - 32)))) * 0.0000004656612874161595 + -1000;
    error_level_config_current_phase_tmp.m_info_timeout = ((error_level_config_current_phase_tmp_tx_fragmentation_buffer[1] & (0xFFFFFFFF >> (32 - 32)))) * 0.000000013969838622484784 + 0;
    error_level_config_current_phase_tmp.m_warning_thresh = ((error_level_config_current_phase_tmp_tx_fragmentation_buffer[2] & (0xFFFFFFFF >> (32 - 32)))) * 0.0000004656612874161595 + -1000;
    error_level_config_current_phase_tmp.m_warning_timeout = ((error_level_config_current_phase_tmp_tx_fragmentation_buffer[3] & (0xFFFFFFFF >> (32 - 32)))) * 0.000000013969838622484784 + 0;
    error_level_config_current_phase_tmp.m_error_thresh = ((error_level_config_current_phase_tmp_tx_fragmentation_buffer[4] & (0xFFFFFFFF >> (32 - 32)))) * 0.0000004656612874161595 + -1000;
    error_level_config_current_phase_tmp.m_error_timeout = ((error_level_config_current_phase_tmp_tx_fragmentation_buffer[5] & (0xFFFFFFFF >> (32 - 32)))) * 0.000000013969838622484784 + 0;
    error_level_config_current_phase_tmp.m_ignore_info = ((bool_t)((error_level_config_current_phase_tmp_tx_fragmentation_buffer[6] & (0xFFFFFFFF >> (32 - 1)))));
    error_level_config_current_phase_tmp.m_ignore_warning = ((bool_t)((error_level_config_current_phase_tmp_tx_fragmentation_buffer[6] >> 1) & (0xFFFFFFFF >> (32 - 1))));
    error_level_config_current_phase_tmp.m_ignore_error = ((bool_t)((error_level_config_current_phase_tmp_tx_fragmentation_buffer[6] >> 2) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_error_level_config_current_phase(error_level_config_current_phase_tmp);
    break;
  }
  case 42 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float frequency_tmp;
    frequency_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 16))) * 0.0007629510948348211 + 0);
    canzero_set_frequency(frequency_tmp);
    break;
  }
  case 43 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float modulation_index_tmp;
    modulation_index_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 16))) * 0.000015259021896696422 + 0);
    canzero_set_modulation_index(modulation_index_tmp);
    break;
  }
  case 44 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float acceleration_tmp;
    acceleration_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 16))) * 0.00030518043793392844 + -10);
    canzero_set_acceleration(acceleration_tmp);
    break;
  }
  case 45 : {
    if (msg.m_header.m_sof == 1) {
      if (msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 0) {
        return; //TODO proper error response frame!
      }
      acceleration_calibration_variance_tmp_tx_fragmentation_offset = 0;
    }else {
      acceleration_calibration_variance_tmp_tx_fragmentation_offset += 1;
      if (acceleration_calibration_variance_tmp_tx_fragmentation_offset >= 2) {
        return;
      }
    }
    acceleration_calibration_variance_tmp_tx_fragmentation_buffer[acceleration_calibration_variance_tmp_tx_fragmentation_offset] = msg.m_data;
    if (msg.m_header.m_eof == 0) {
      return;
    }
    double acceleration_calibration_variance_tmp;
    acceleration_calibration_variance_tmp = ((uint64_t)acceleration_calibration_variance_tmp_tx_fragmentation_buffer[0] | (((uint64_t)(acceleration_calibration_variance_tmp_tx_fragmentation_buffer[1] & (0xFFFFFFFF >> (32 - 32)))) << 32)) * 0.00000000000000000010842021724855044 + -1;
    canzero_set_acceleration_calibration_variance(acceleration_calibration_variance_tmp);
    break;
  }
  case 46 : {
    if (msg.m_header.m_sof == 1) {
      if (msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 0) {
        return; //TODO proper error response frame!
      }
      acceleration_calibration_offset_tmp_tx_fragmentation_offset = 0;
    }else {
      acceleration_calibration_offset_tmp_tx_fragmentation_offset += 1;
      if (acceleration_calibration_offset_tmp_tx_fragmentation_offset >= 2) {
        return;
      }
    }
    acceleration_calibration_offset_tmp_tx_fragmentation_buffer[acceleration_calibration_offset_tmp_tx_fragmentation_offset] = msg.m_data;
    if (msg.m_header.m_eof == 0) {
      return;
    }
    double acceleration_calibration_offset_tmp;
    acceleration_calibration_offset_tmp = ((uint64_t)acceleration_calibration_offset_tmp_tx_fragmentation_buffer[0] | (((uint64_t)(acceleration_calibration_offset_tmp_tx_fragmentation_buffer[1] & (0xFFFFFFFF >> (32 - 32)))) << 32)) * 0.00000000000000000010842021724855044 + -1;
    canzero_set_acceleration_calibration_offset(acceleration_calibration_offset_tmp);
    break;
  }
  case 47 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float target_acceleration_tmp;
    target_acceleration_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 32))) * 0.000000023283064370807974 + -50);
    canzero_set_target_acceleration(target_acceleration_tmp);
    break;
  }
  case 48 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float external_acceleration_tmp;
    external_acceleration_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 16))) * 0.00030518043793392844 + -10);
    canzero_set_external_acceleration(external_acceleration_tmp);
    break;
  }
  case 49 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float external_velocity_tmp;
    external_velocity_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 16))) * 0.00030518043793392844 + -10);
    canzero_set_external_velocity(external_velocity_tmp);
    break;
  }
  case 50 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float external_position_tmp;
    external_position_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 16))) * 0.0015259021896696422 + -50);
    canzero_set_external_position(external_position_tmp);
    break;
  }
  case 51 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float vdc_voltage_tmp;
    vdc_voltage_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 16))) * 0.0030518043793392844 + -100);
    canzero_set_vdc_voltage(vdc_voltage_tmp);
    break;
  }
  case 52 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    error_flag error_vdc_voltage_invalid_tmp;
    error_vdc_voltage_invalid_tmp = ((error_flag)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_error_vdc_voltage_invalid(error_vdc_voltage_invalid_tmp);
    break;
  }
  case 53 : {
    if (msg.m_header.m_sof == 1) {
      if (msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 0) {
        return; //TODO proper error response frame!
      }
      error_level_config_vdc_voltage_tmp_tx_fragmentation_offset = 0;
    }else {
      error_level_config_vdc_voltage_tmp_tx_fragmentation_offset += 1;
      if (error_level_config_vdc_voltage_tmp_tx_fragmentation_offset >= 7) {
        return;
      }
    }
    error_level_config_vdc_voltage_tmp_tx_fragmentation_buffer[error_level_config_vdc_voltage_tmp_tx_fragmentation_offset] = msg.m_data;
    if (msg.m_header.m_eof == 0) {
      return;
    }
    error_level_config error_level_config_vdc_voltage_tmp;
    error_level_config_vdc_voltage_tmp.m_info_thresh = ((error_level_config_vdc_voltage_tmp_tx_fragmentation_buffer[0] & (0xFFFFFFFF >> (32 - 32)))) * 0.0000004656612874161595 + -1000;
    error_level_config_vdc_voltage_tmp.m_info_timeout = ((error_level_config_vdc_voltage_tmp_tx_fragmentation_buffer[1] & (0xFFFFFFFF >> (32 - 32)))) * 0.000000013969838622484784 + 0;
    error_level_config_vdc_voltage_tmp.m_warning_thresh = ((error_level_config_vdc_voltage_tmp_tx_fragmentation_buffer[2] & (0xFFFFFFFF >> (32 - 32)))) * 0.0000004656612874161595 + -1000;
    error_level_config_vdc_voltage_tmp.m_warning_timeout = ((error_level_config_vdc_voltage_tmp_tx_fragmentation_buffer[3] & (0xFFFFFFFF >> (32 - 32)))) * 0.000000013969838622484784 + 0;
    error_level_config_vdc_voltage_tmp.m_error_thresh = ((error_level_config_vdc_voltage_tmp_tx_fragmentation_buffer[4] & (0xFFFFFFFF >> (32 - 32)))) * 0.0000004656612874161595 + -1000;
    error_level_config_vdc_voltage_tmp.m_error_timeout = ((error_level_config_vdc_voltage_tmp_tx_fragmentation_buffer[5] & (0xFFFFFFFF >> (32 - 32)))) * 0.000000013969838622484784 + 0;
    error_level_config_vdc_voltage_tmp.m_ignore_info = ((bool_t)((error_level_config_vdc_voltage_tmp_tx_fragmentation_buffer[6] & (0xFFFFFFFF >> (32 - 1)))));
    error_level_config_vdc_voltage_tmp.m_ignore_warning = ((bool_t)((error_level_config_vdc_voltage_tmp_tx_fragmentation_buffer[6] >> 1) & (0xFFFFFFFF >> (32 - 1))));
    error_level_config_vdc_voltage_tmp.m_ignore_error = ((bool_t)((error_level_config_vdc_voltage_tmp_tx_fragmentation_buffer[6] >> 2) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_error_level_config_vdc_voltage(error_level_config_vdc_voltage_tmp);
    break;
  }
  case 54 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float board_temperature1_tmp;
    board_temperature1_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 8))) * 0.592156862745098 + -1);
    canzero_set_board_temperature1(board_temperature1_tmp);
    break;
  }
  case 55 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float board_temperature2_tmp;
    board_temperature2_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 8))) * 0.592156862745098 + -1);
    canzero_set_board_temperature2(board_temperature2_tmp);
    break;
  }
  case 56 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float board_temperature3_tmp;
    board_temperature3_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 8))) * 0.592156862745098 + -1);
    canzero_set_board_temperature3(board_temperature3_tmp);
    break;
  }
  case 57 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float board_avg_temperature_tmp;
    board_avg_temperature_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 8))) * 0.592156862745098 + -1);
    canzero_set_board_avg_temperature(board_avg_temperature_tmp);
    break;
  }
  case 58 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float board_max_temperature_tmp;
    board_max_temperature_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 8))) * 0.592156862745098 + -1);
    canzero_set_board_max_temperature(board_max_temperature_tmp);
    break;
  }
  case 59 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float board_min_temperature_tmp;
    board_min_temperature_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 8))) * 0.592156862745098 + -1);
    canzero_set_board_min_temperature(board_min_temperature_tmp);
    break;
  }
  case 60 : {
    if (msg.m_header.m_sof == 1) {
      if (msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 0) {
        return; //TODO proper error response frame!
      }
      error_level_config_board_temperature_tmp_tx_fragmentation_offset = 0;
    }else {
      error_level_config_board_temperature_tmp_tx_fragmentation_offset += 1;
      if (error_level_config_board_temperature_tmp_tx_fragmentation_offset >= 7) {
        return;
      }
    }
    error_level_config_board_temperature_tmp_tx_fragmentation_buffer[error_level_config_board_temperature_tmp_tx_fragmentation_offset] = msg.m_data;
    if (msg.m_header.m_eof == 0) {
      return;
    }
    error_level_config error_level_config_board_temperature_tmp;
    error_level_config_board_temperature_tmp.m_info_thresh = ((error_level_config_board_temperature_tmp_tx_fragmentation_buffer[0] & (0xFFFFFFFF >> (32 - 32)))) * 0.0000004656612874161595 + -1000;
    error_level_config_board_temperature_tmp.m_info_timeout = ((error_level_config_board_temperature_tmp_tx_fragmentation_buffer[1] & (0xFFFFFFFF >> (32 - 32)))) * 0.000000013969838622484784 + 0;
    error_level_config_board_temperature_tmp.m_warning_thresh = ((error_level_config_board_temperature_tmp_tx_fragmentation_buffer[2] & (0xFFFFFFFF >> (32 - 32)))) * 0.0000004656612874161595 + -1000;
    error_level_config_board_temperature_tmp.m_warning_timeout = ((error_level_config_board_temperature_tmp_tx_fragmentation_buffer[3] & (0xFFFFFFFF >> (32 - 32)))) * 0.000000013969838622484784 + 0;
    error_level_config_board_temperature_tmp.m_error_thresh = ((error_level_config_board_temperature_tmp_tx_fragmentation_buffer[4] & (0xFFFFFFFF >> (32 - 32)))) * 0.0000004656612874161595 + -1000;
    error_level_config_board_temperature_tmp.m_error_timeout = ((error_level_config_board_temperature_tmp_tx_fragmentation_buffer[5] & (0xFFFFFFFF >> (32 - 32)))) * 0.000000013969838622484784 + 0;
    error_level_config_board_temperature_tmp.m_ignore_info = ((bool_t)((error_level_config_board_temperature_tmp_tx_fragmentation_buffer[6] & (0xFFFFFFFF >> (32 - 1)))));
    error_level_config_board_temperature_tmp.m_ignore_warning = ((bool_t)((error_level_config_board_temperature_tmp_tx_fragmentation_buffer[6] >> 1) & (0xFFFFFFFF >> (32 - 1))));
    error_level_config_board_temperature_tmp.m_ignore_error = ((bool_t)((error_level_config_board_temperature_tmp_tx_fragmentation_buffer[6] >> 2) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_error_level_config_board_temperature(error_level_config_board_temperature_tmp);
    break;
  }
  case 61 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float mcu_temperature_tmp;
    mcu_temperature_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 8))) * 0.592156862745098 + -1);
    canzero_set_mcu_temperature(mcu_temperature_tmp);
    break;
  }
  case 62 : {
    if (msg.m_header.m_sof == 1) {
      if (msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 0) {
        return; //TODO proper error response frame!
      }
      error_level_config_mcu_temperature_tmp_tx_fragmentation_offset = 0;
    }else {
      error_level_config_mcu_temperature_tmp_tx_fragmentation_offset += 1;
      if (error_level_config_mcu_temperature_tmp_tx_fragmentation_offset >= 7) {
        return;
      }
    }
    error_level_config_mcu_temperature_tmp_tx_fragmentation_buffer[error_level_config_mcu_temperature_tmp_tx_fragmentation_offset] = msg.m_data;
    if (msg.m_header.m_eof == 0) {
      return;
    }
    error_level_config error_level_config_mcu_temperature_tmp;
    error_level_config_mcu_temperature_tmp.m_info_thresh = ((error_level_config_mcu_temperature_tmp_tx_fragmentation_buffer[0] & (0xFFFFFFFF >> (32 - 32)))) * 0.0000004656612874161595 + -1000;
    error_level_config_mcu_temperature_tmp.m_info_timeout = ((error_level_config_mcu_temperature_tmp_tx_fragmentation_buffer[1] & (0xFFFFFFFF >> (32 - 32)))) * 0.000000013969838622484784 + 0;
    error_level_config_mcu_temperature_tmp.m_warning_thresh = ((error_level_config_mcu_temperature_tmp_tx_fragmentation_buffer[2] & (0xFFFFFFFF >> (32 - 32)))) * 0.0000004656612874161595 + -1000;
    error_level_config_mcu_temperature_tmp.m_warning_timeout = ((error_level_config_mcu_temperature_tmp_tx_fragmentation_buffer[3] & (0xFFFFFFFF >> (32 - 32)))) * 0.000000013969838622484784 + 0;
    error_level_config_mcu_temperature_tmp.m_error_thresh = ((error_level_config_mcu_temperature_tmp_tx_fragmentation_buffer[4] & (0xFFFFFFFF >> (32 - 32)))) * 0.0000004656612874161595 + -1000;
    error_level_config_mcu_temperature_tmp.m_error_timeout = ((error_level_config_mcu_temperature_tmp_tx_fragmentation_buffer[5] & (0xFFFFFFFF >> (32 - 32)))) * 0.000000013969838622484784 + 0;
    error_level_config_mcu_temperature_tmp.m_ignore_info = ((bool_t)((error_level_config_mcu_temperature_tmp_tx_fragmentation_buffer[6] & (0xFFFFFFFF >> (32 - 1)))));
    error_level_config_mcu_temperature_tmp.m_ignore_warning = ((bool_t)((error_level_config_mcu_temperature_tmp_tx_fragmentation_buffer[6] >> 1) & (0xFFFFFFFF >> (32 - 1))));
    error_level_config_mcu_temperature_tmp.m_ignore_error = ((bool_t)((error_level_config_mcu_temperature_tmp_tx_fragmentation_buffer[6] >> 2) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_error_level_config_mcu_temperature(error_level_config_mcu_temperature_tmp);
    break;
  }
  case 63 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float lim_temperature1_tmp;
    lim_temperature1_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 8))) * 0.592156862745098 + -1);
    canzero_set_lim_temperature1(lim_temperature1_tmp);
    break;
  }
  case 64 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float lim_temperature2_tmp;
    lim_temperature2_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 8))) * 0.592156862745098 + -1);
    canzero_set_lim_temperature2(lim_temperature2_tmp);
    break;
  }
  case 65 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float lim_temperature3_tmp;
    lim_temperature3_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 8))) * 0.592156862745098 + -1);
    canzero_set_lim_temperature3(lim_temperature3_tmp);
    break;
  }
  case 66 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float lim_temperature4_tmp;
    lim_temperature4_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 8))) * 0.592156862745098 + -1);
    canzero_set_lim_temperature4(lim_temperature4_tmp);
    break;
  }
  case 67 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float lim_average_temperature_tmp;
    lim_average_temperature_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 8))) * 0.592156862745098 + -1);
    canzero_set_lim_average_temperature(lim_average_temperature_tmp);
    break;
  }
  case 68 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float lim_max_temperature_tmp;
    lim_max_temperature_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 8))) * 0.592156862745098 + -1);
    canzero_set_lim_max_temperature(lim_max_temperature_tmp);
    break;
  }
  case 69 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float lim_min_temperature_tmp;
    lim_min_temperature_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 8))) * 0.592156862745098 + -1);
    canzero_set_lim_min_temperature(lim_min_temperature_tmp);
    break;
  }
  case 70 : {
    if (msg.m_header.m_sof == 1) {
      if (msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 0) {
        return; //TODO proper error response frame!
      }
      error_level_config_lim_temperature_tmp_tx_fragmentation_offset = 0;
    }else {
      error_level_config_lim_temperature_tmp_tx_fragmentation_offset += 1;
      if (error_level_config_lim_temperature_tmp_tx_fragmentation_offset >= 7) {
        return;
      }
    }
    error_level_config_lim_temperature_tmp_tx_fragmentation_buffer[error_level_config_lim_temperature_tmp_tx_fragmentation_offset] = msg.m_data;
    if (msg.m_header.m_eof == 0) {
      return;
    }
    error_level_config error_level_config_lim_temperature_tmp;
    error_level_config_lim_temperature_tmp.m_info_thresh = ((error_level_config_lim_temperature_tmp_tx_fragmentation_buffer[0] & (0xFFFFFFFF >> (32 - 32)))) * 0.0000004656612874161595 + -1000;
    error_level_config_lim_temperature_tmp.m_info_timeout = ((error_level_config_lim_temperature_tmp_tx_fragmentation_buffer[1] & (0xFFFFFFFF >> (32 - 32)))) * 0.000000013969838622484784 + 0;
    error_level_config_lim_temperature_tmp.m_warning_thresh = ((error_level_config_lim_temperature_tmp_tx_fragmentation_buffer[2] & (0xFFFFFFFF >> (32 - 32)))) * 0.0000004656612874161595 + -1000;
    error_level_config_lim_temperature_tmp.m_warning_timeout = ((error_level_config_lim_temperature_tmp_tx_fragmentation_buffer[3] & (0xFFFFFFFF >> (32 - 32)))) * 0.000000013969838622484784 + 0;
    error_level_config_lim_temperature_tmp.m_error_thresh = ((error_level_config_lim_temperature_tmp_tx_fragmentation_buffer[4] & (0xFFFFFFFF >> (32 - 32)))) * 0.0000004656612874161595 + -1000;
    error_level_config_lim_temperature_tmp.m_error_timeout = ((error_level_config_lim_temperature_tmp_tx_fragmentation_buffer[5] & (0xFFFFFFFF >> (32 - 32)))) * 0.000000013969838622484784 + 0;
    error_level_config_lim_temperature_tmp.m_ignore_info = ((bool_t)((error_level_config_lim_temperature_tmp_tx_fragmentation_buffer[6] & (0xFFFFFFFF >> (32 - 1)))));
    error_level_config_lim_temperature_tmp.m_ignore_warning = ((bool_t)((error_level_config_lim_temperature_tmp_tx_fragmentation_buffer[6] >> 1) & (0xFFFFFFFF >> (32 - 1))));
    error_level_config_lim_temperature_tmp.m_ignore_error = ((bool_t)((error_level_config_lim_temperature_tmp_tx_fragmentation_buffer[6] >> 2) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_error_level_config_lim_temperature(error_level_config_lim_temperature_tmp);
    break;
  }
  case 71 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float loop_frequency_tmp;
    loop_frequency_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 32))) * 0.00000023283064370807974 + 0);
    canzero_set_loop_frequency(loop_frequency_tmp);
    break;
  }
  case 72 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    uint8_t last_node_missed_tmp;
    last_node_missed_tmp = ((uint8_t)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 8)))));
    canzero_set_last_node_missed(last_node_missed_tmp);
    break;
  }
  case 73 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    bool_t ignore_45v_tmp;
    ignore_45v_tmp = ((bool_t)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_ignore_45v(ignore_45v_tmp);
    break;
  }
  default:
    return;
  }
  resp.m_header.m_od_index = msg.m_header.m_od_index;
  resp.m_header.m_client_id = msg.m_header.m_client_id;
  resp.m_header.m_server_id = msg.m_header.m_server_id;
  resp.m_header.m_erno = set_resp_erno_Success;
  canzero_frame resp_frame;
  canzero_serialize_canzero_message_set_resp(&resp, &resp_frame);
  canzero_can1_send(&resp_frame);

}
static void canzero_handle_mother_board_stream_motor_command(canzero_frame* frame) {
  canzero_message_mother_board_stream_motor_command msg;
  canzero_deserialize_canzero_message_mother_board_stream_motor_command(frame, &msg);
  canzero_set_target_acceleration(msg.m_target_acceleration);
  canzero_set_command(msg.m_motor_driver_command);
}
static void canzero_handle_mother_board_stream_debug_settings(canzero_frame* frame) {
  canzero_message_mother_board_stream_debug_settings msg;
  canzero_deserialize_canzero_message_mother_board_stream_debug_settings(frame, &msg);
  canzero_set_ignore_45v(msg.m_ignore_45v);
}
 void canzero_handle_heartbeat_can0(canzero_frame* frame) {
  canzero_message_heartbeat_can0 msg;
  canzero_deserialize_canzero_message_heartbeat_can0(frame, &msg);

  if (msg.m_node_id < node_id_count) {   // static heartbeat
    if (msg.m_unregister != 0) {  // unregister only unregisters this bus
      heartbeat_wdg_job.job.wdg_job.can0_static_wdg_armed[msg.m_node_id] = 0;
    } else { // register registers for all buses
      heartbeat_wdg_job.job.wdg_job.can0_static_wdg_armed[msg.m_node_id] = 1;
      heartbeat_wdg_job.job.wdg_job.can1_static_wdg_armed[msg.m_node_id] = 1;
    }
    if (heartbeat_wdg_job.job.wdg_job.can0_static_tick_countdowns[msg.m_node_id] <= 0 &&
        msg.m_ticks_next > 0) {
      canzero_can0_wdg_recovered(msg.m_node_id);
    }
    heartbeat_wdg_job.job.wdg_job.can0_static_tick_countdowns[msg.m_node_id] = msg.m_ticks_next;
  } else {  // dynamic heartbeat
    if (msg.m_unregister != 0) { // unregister only unregisters this bus
      heartbeat_wdg_job.job.wdg_job.can0_dynamic_wdg_armed[msg.m_node_id - node_id_count] = 0;
    } else { // register registers all buses
      heartbeat_wdg_job.job.wdg_job.can0_dynamic_wdg_armed[msg.m_node_id - node_id_count] = 1;
      heartbeat_wdg_job.job.wdg_job.can1_dynamic_wdg_armed[msg.m_node_id - node_id_count] = 1;
    }
    if (heartbeat_wdg_job.job.wdg_job.can0_dynamic_tick_countdowns[msg.m_node_id - node_id_count] <= 0 
        && msg.m_ticks_next > 0) {
      canzero_can0_wdg_recovered(msg.m_node_id);
    }
    heartbeat_wdg_job.job.wdg_job.can0_dynamic_tick_countdowns[msg.m_node_id - node_id_count]
      = msg.m_ticks_next;
  }
}
 void canzero_handle_heartbeat_can1(canzero_frame* frame) {
  canzero_message_heartbeat_can1 msg;
  canzero_deserialize_canzero_message_heartbeat_can1(frame, &msg);

  if (msg.m_node_id < node_id_count) {   // static heartbeat
    if (msg.m_unregister != 0) {  // unregister only unregisters this bus
      heartbeat_wdg_job.job.wdg_job.can1_static_wdg_armed[msg.m_node_id] = 0;
    } else { // register registers for all buses
      heartbeat_wdg_job.job.wdg_job.can0_static_wdg_armed[msg.m_node_id] = 1;
      heartbeat_wdg_job.job.wdg_job.can1_static_wdg_armed[msg.m_node_id] = 1;
    }
    if (heartbeat_wdg_job.job.wdg_job.can1_static_tick_countdowns[msg.m_node_id] <= 0 &&
        msg.m_ticks_next > 0) {
      canzero_can1_wdg_recovered(msg.m_node_id);
    }
    heartbeat_wdg_job.job.wdg_job.can1_static_tick_countdowns[msg.m_node_id] = msg.m_ticks_next;
  } else {  // dynamic heartbeat
    if (msg.m_unregister != 0) { // unregister only unregisters this bus
      heartbeat_wdg_job.job.wdg_job.can1_dynamic_wdg_armed[msg.m_node_id - node_id_count] = 0;
    } else { // register registers all buses
      heartbeat_wdg_job.job.wdg_job.can0_dynamic_wdg_armed[msg.m_node_id - node_id_count] = 1;
      heartbeat_wdg_job.job.wdg_job.can1_dynamic_wdg_armed[msg.m_node_id - node_id_count] = 1;
    }
    if (heartbeat_wdg_job.job.wdg_job.can1_dynamic_tick_countdowns[msg.m_node_id - node_id_count] <= 0 
        && msg.m_ticks_next > 0) {
      canzero_can1_wdg_recovered(msg.m_node_id);
    }
    heartbeat_wdg_job.job.wdg_job.can1_dynamic_tick_countdowns[msg.m_node_id - node_id_count]
      = msg.m_ticks_next;
  }
}
void canzero_can0_poll() {
  canzero_frame frame;
  while (canzero_can0_recv(&frame)) {
    switch (frame.id) {
      case 0x1BE:
        canzero_handle_get_req(&frame);
        break;
      case 0x51:
        canzero_handle_mother_board_stream_debug_settings(&frame);
        break;
      case 0x1F4:
        canzero_handle_heartbeat_can0(&frame);
        break;
    }
  }
}
void canzero_can1_poll() {
  canzero_frame frame;
  while (canzero_can1_recv(&frame)) {
    switch (frame.id) {
      case 0x1DE:
        canzero_handle_set_req(&frame);
        break;
      case 0x4C:
        canzero_handle_mother_board_stream_motor_command(&frame);
        break;
      case 0x1F3:
        canzero_handle_heartbeat_can1(&frame);
        break;
    }
  }
}
uint32_t canzero_update_continue(uint32_t time){
  schedule_jobs(time);
  return scheduler_next_job_timeout();
}
#define COMPUTE_BUILD_YEAR \
    ( (__DATE__[ 7] - '0') * 1000 + \
        (__DATE__[ 8] - '0') *  100 + \
        (__DATE__[ 9] - '0') *   10 + \
        (__DATE__[10] - '0') \
    )
#define COMPUTE_BUILD_DAY \
    ( \
        ((__DATE__[4] >= '0') ? (__DATE__[4] - '0') * 10 : 0) + \
        (__DATE__[5] - '0') \
    )
#define BUILD_MONTH_IS_JAN (__DATE__[0] == 'J' && __DATE__[1] == 'a' && __DATE__[2] == 'n')
#define BUILD_MONTH_IS_FEB (__DATE__[0] == 'F')
#define BUILD_MONTH_IS_MAR (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r')
#define BUILD_MONTH_IS_APR (__DATE__[0] == 'A' && __DATE__[1] == 'p')
#define BUILD_MONTH_IS_MAY (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y')
#define BUILD_MONTH_IS_JUN (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n')
#define BUILD_MONTH_IS_JUL (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l')
#define BUILD_MONTH_IS_AUG (__DATE__[0] == 'A' && __DATE__[1] == 'u')
#define BUILD_MONTH_IS_SEP (__DATE__[0] == 'S')
#define BUILD_MONTH_IS_OCT (__DATE__[0] == 'O')
#define BUILD_MONTH_IS_NOV (__DATE__[0] == 'N')
#define BUILD_MONTH_IS_DEC (__DATE__[0] == 'D')
#define COMPUTE_BUILD_MONTH \
    ( \
        (BUILD_MONTH_IS_JAN) ?  1 : \
        (BUILD_MONTH_IS_FEB) ?  2 : \
        (BUILD_MONTH_IS_MAR) ?  3 : \
        (BUILD_MONTH_IS_APR) ?  4 : \
        (BUILD_MONTH_IS_MAY) ?  5 : \
        (BUILD_MONTH_IS_JUN) ?  6 : \
        (BUILD_MONTH_IS_JUL) ?  7 : \
        (BUILD_MONTH_IS_AUG) ?  8 : \
        (BUILD_MONTH_IS_SEP) ?  9 : \
        (BUILD_MONTH_IS_OCT) ? 10 : \
        (BUILD_MONTH_IS_NOV) ? 11 : \
        (BUILD_MONTH_IS_DEC) ? 12 : \
        /* error default */  99 \
    )
#define COMPUTE_BUILD_HOUR ((__TIME__[0] - '0') * 10 + __TIME__[1] - '0')
#define COMPUTE_BUILD_MIN  ((__TIME__[3] - '0') * 10 + __TIME__[4] - '0')
#define COMPUTE_BUILD_SEC  ((__TIME__[6] - '0') * 10 + __TIME__[7] - '0')
#define BUILD_DATE_IS_BAD (__DATE__[0] == '?')
#define BUILD_YEAR  ((BUILD_DATE_IS_BAD) ? 99 : COMPUTE_BUILD_YEAR)
#define BUILD_MONTH ((BUILD_DATE_IS_BAD) ? 99 : COMPUTE_BUILD_MONTH)
#define BUILD_DAY   ((BUILD_DATE_IS_BAD) ? 99 : COMPUTE_BUILD_DAY)
#define BUILD_TIME_IS_BAD (__TIME__[0] == '?')
#define BUILD_HOUR  ((BUILD_TIME_IS_BAD) ? 99 :  COMPUTE_BUILD_HOUR)
#define BUILD_MIN   ((BUILD_TIME_IS_BAD) ? 99 :  COMPUTE_BUILD_MIN)
#define BUILD_SEC   ((BUILD_TIME_IS_BAD) ? 99 :  COMPUTE_BUILD_SEC)
void canzero_init() {
  __oe_config_hash = 2876945102520670225ull;
  __oe_build_time = {
    .m_year = BUILD_YEAR,
    .m_month = BUILD_MONTH,
    .m_day = BUILD_DAY,
    .m_hour = BUILD_HOUR,
    .m_min = BUILD_MIN,
    .m_sec = BUILD_SEC
  };
  canzero_can0_setup(1000000, NULL, 0);
  canzero_can1_setup(1000000, NULL, 0);

  job_pool_allocator_init();
  scheduler.size = 0;
  schedule_heartbeat_job();
  schedule_heartbeat_wdg_job();
  schedule_state_interval_job();
  schedule_config_hash_interval_job();
  schedule_errors_interval_job();
  schedule_debug_interval_job();
  schedule_currents_u1v1w1_interval_job();
  schedule_current_u2v2w2_interval_job();
  schedule_control_info_interval_job();
  schedule_board_temperature_interval_job();
  schedule_lim_temperature_interval_job();

}
void canzero_set_config_hash(uint64_t value) {
  extern uint64_t __oe_config_hash;
  if (__oe_config_hash != value) {
    __oe_config_hash = value;
    if (config_hash_interval_job.climax > config_hash_interval_job.job.stream_job.last_schedule + 0) {
      config_hash_interval_job.climax = config_hash_interval_job.job.stream_job.last_schedule + 0;
      scheduler_promote_job(&config_hash_interval_job);
    }
  }
}
void canzero_set_state(motor_state value) {
  extern motor_state __oe_state;
  if (__oe_state != value) {
    __oe_state = value;
    if (state_interval_job.climax > state_interval_job.job.stream_job.last_schedule + 1) {
      state_interval_job.climax = state_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&state_interval_job);
    }
  }
}
void canzero_set_sdc_status(sdc_status value) {
  extern sdc_status __oe_sdc_status;
  if (__oe_sdc_status != value) {
    __oe_sdc_status = value;
    if (state_interval_job.climax > state_interval_job.job.stream_job.last_schedule + 1) {
      state_interval_job.climax = state_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&state_interval_job);
    }
  }
}
void canzero_set_assertion_fault(error_flag value) {
  extern error_flag __oe_assertion_fault;
  if (__oe_assertion_fault != value) {
    __oe_assertion_fault = value;
    if (errors_interval_job.climax > errors_interval_job.job.stream_job.last_schedule + 1) {
      errors_interval_job.climax = errors_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&errors_interval_job);
    }
  }
}
void canzero_set_error_arming_failed(error_flag value) {
  extern error_flag __oe_error_arming_failed;
  if (__oe_error_arming_failed != value) {
    __oe_error_arming_failed = value;
    if (errors_interval_job.climax > errors_interval_job.job.stream_job.last_schedule + 1) {
      errors_interval_job.climax = errors_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&errors_interval_job);
    }
  }
}
void canzero_set_error_precharge_failed(error_flag value) {
  extern error_flag __oe_error_precharge_failed;
  if (__oe_error_precharge_failed != value) {
    __oe_error_precharge_failed = value;
    if (errors_interval_job.climax > errors_interval_job.job.stream_job.last_schedule + 1) {
      errors_interval_job.climax = errors_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&errors_interval_job);
    }
  }
}
void canzero_set_error_acceleration_out_of_range(error_flag value) {
  extern error_flag __oe_error_acceleration_out_of_range;
  if (__oe_error_acceleration_out_of_range != value) {
    __oe_error_acceleration_out_of_range = value;
    if (errors_interval_job.climax > errors_interval_job.job.stream_job.last_schedule + 1) {
      errors_interval_job.climax = errors_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&errors_interval_job);
    }
  }
}
void canzero_set_error_acceleration_calibration_failed(error_flag value) {
  extern error_flag __oe_error_acceleration_calibration_failed;
  if (__oe_error_acceleration_calibration_failed != value) {
    __oe_error_acceleration_calibration_failed = value;
    if (errors_interval_job.climax > errors_interval_job.job.stream_job.last_schedule + 1) {
      errors_interval_job.climax = errors_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&errors_interval_job);
    }
  }
}
void canzero_set_error_invalid_target_acceleration(error_flag value) {
  extern error_flag __oe_error_invalid_target_acceleration;
  if (__oe_error_invalid_target_acceleration != value) {
    __oe_error_invalid_target_acceleration = value;
    if (errors_interval_job.climax > errors_interval_job.job.stream_job.last_schedule + 1) {
      errors_interval_job.climax = errors_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&errors_interval_job);
    }
  }
}
void canzero_set_error_board_temperature1_invalid(error_flag value) {
  extern error_flag __oe_error_board_temperature1_invalid;
  if (__oe_error_board_temperature1_invalid != value) {
    __oe_error_board_temperature1_invalid = value;
    if (errors_interval_job.climax > errors_interval_job.job.stream_job.last_schedule + 1) {
      errors_interval_job.climax = errors_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&errors_interval_job);
    }
  }
}
void canzero_set_error_board_temperature2_invalid(error_flag value) {
  extern error_flag __oe_error_board_temperature2_invalid;
  if (__oe_error_board_temperature2_invalid != value) {
    __oe_error_board_temperature2_invalid = value;
    if (errors_interval_job.climax > errors_interval_job.job.stream_job.last_schedule + 1) {
      errors_interval_job.climax = errors_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&errors_interval_job);
    }
  }
}
void canzero_set_error_board_temperature3_invalid(error_flag value) {
  extern error_flag __oe_error_board_temperature3_invalid;
  if (__oe_error_board_temperature3_invalid != value) {
    __oe_error_board_temperature3_invalid = value;
    if (errors_interval_job.climax > errors_interval_job.job.stream_job.last_schedule + 1) {
      errors_interval_job.climax = errors_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&errors_interval_job);
    }
  }
}
void canzero_set_error_mcu_temperature_invalid(error_flag value) {
  extern error_flag __oe_error_mcu_temperature_invalid;
  if (__oe_error_mcu_temperature_invalid != value) {
    __oe_error_mcu_temperature_invalid = value;
    if (errors_interval_job.climax > errors_interval_job.job.stream_job.last_schedule + 1) {
      errors_interval_job.climax = errors_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&errors_interval_job);
    }
  }
}
void canzero_set_error_lim_temperature1_invalid(error_flag value) {
  extern error_flag __oe_error_lim_temperature1_invalid;
  if (__oe_error_lim_temperature1_invalid != value) {
    __oe_error_lim_temperature1_invalid = value;
    if (errors_interval_job.climax > errors_interval_job.job.stream_job.last_schedule + 1) {
      errors_interval_job.climax = errors_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&errors_interval_job);
    }
  }
}
void canzero_set_error_lim_temperature2_invalid(error_flag value) {
  extern error_flag __oe_error_lim_temperature2_invalid;
  if (__oe_error_lim_temperature2_invalid != value) {
    __oe_error_lim_temperature2_invalid = value;
    if (errors_interval_job.climax > errors_interval_job.job.stream_job.last_schedule + 1) {
      errors_interval_job.climax = errors_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&errors_interval_job);
    }
  }
}
void canzero_set_error_lim_temperature3_invalid(error_flag value) {
  extern error_flag __oe_error_lim_temperature3_invalid;
  if (__oe_error_lim_temperature3_invalid != value) {
    __oe_error_lim_temperature3_invalid = value;
    if (errors_interval_job.climax > errors_interval_job.job.stream_job.last_schedule + 1) {
      errors_interval_job.climax = errors_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&errors_interval_job);
    }
  }
}
void canzero_set_error_lim_temperature4_invalid(error_flag value) {
  extern error_flag __oe_error_lim_temperature4_invalid;
  if (__oe_error_lim_temperature4_invalid != value) {
    __oe_error_lim_temperature4_invalid = value;
    if (errors_interval_job.climax > errors_interval_job.job.stream_job.last_schedule + 1) {
      errors_interval_job.climax = errors_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&errors_interval_job);
    }
  }
}
void canzero_set_error_heartbeat_miss(error_flag value) {
  extern error_flag __oe_error_heartbeat_miss;
  if (__oe_error_heartbeat_miss != value) {
    __oe_error_heartbeat_miss = value;
    if (errors_interval_job.climax > errors_interval_job.job.stream_job.last_schedule + 1) {
      errors_interval_job.climax = errors_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&errors_interval_job);
    }
  }
}
void canzero_set_error_level_current_u1(error_level value) {
  extern error_level __oe_error_level_current_u1;
  if (__oe_error_level_current_u1 != value) {
    __oe_error_level_current_u1 = value;
    if (errors_interval_job.climax > errors_interval_job.job.stream_job.last_schedule + 1) {
      errors_interval_job.climax = errors_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&errors_interval_job);
    }
  }
}
void canzero_set_error_level_current_v1(error_level value) {
  extern error_level __oe_error_level_current_v1;
  if (__oe_error_level_current_v1 != value) {
    __oe_error_level_current_v1 = value;
    if (errors_interval_job.climax > errors_interval_job.job.stream_job.last_schedule + 1) {
      errors_interval_job.climax = errors_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&errors_interval_job);
    }
  }
}
void canzero_set_error_level_current_w1(error_level value) {
  extern error_level __oe_error_level_current_w1;
  if (__oe_error_level_current_w1 != value) {
    __oe_error_level_current_w1 = value;
    if (errors_interval_job.climax > errors_interval_job.job.stream_job.last_schedule + 1) {
      errors_interval_job.climax = errors_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&errors_interval_job);
    }
  }
}
void canzero_set_error_level_current_u2(error_level value) {
  extern error_level __oe_error_level_current_u2;
  if (__oe_error_level_current_u2 != value) {
    __oe_error_level_current_u2 = value;
    if (errors_interval_job.climax > errors_interval_job.job.stream_job.last_schedule + 1) {
      errors_interval_job.climax = errors_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&errors_interval_job);
    }
  }
}
void canzero_set_error_level_current_v2(error_level value) {
  extern error_level __oe_error_level_current_v2;
  if (__oe_error_level_current_v2 != value) {
    __oe_error_level_current_v2 = value;
    if (errors_interval_job.climax > errors_interval_job.job.stream_job.last_schedule + 1) {
      errors_interval_job.climax = errors_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&errors_interval_job);
    }
  }
}
void canzero_set_error_level_current_w2(error_level value) {
  extern error_level __oe_error_level_current_w2;
  if (__oe_error_level_current_w2 != value) {
    __oe_error_level_current_w2 = value;
    if (errors_interval_job.climax > errors_interval_job.job.stream_job.last_schedule + 1) {
      errors_interval_job.climax = errors_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&errors_interval_job);
    }
  }
}
void canzero_set_error_level_vdc_voltage(error_level value) {
  extern error_level __oe_error_level_vdc_voltage;
  if (__oe_error_level_vdc_voltage != value) {
    __oe_error_level_vdc_voltage = value;
    if (errors_interval_job.climax > errors_interval_job.job.stream_job.last_schedule + 1) {
      errors_interval_job.climax = errors_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&errors_interval_job);
    }
  }
}
void canzero_set_error_level_board_temperature(error_level value) {
  extern error_level __oe_error_level_board_temperature;
  if (__oe_error_level_board_temperature != value) {
    __oe_error_level_board_temperature = value;
    if (errors_interval_job.climax > errors_interval_job.job.stream_job.last_schedule + 1) {
      errors_interval_job.climax = errors_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&errors_interval_job);
    }
  }
}
void canzero_set_error_level_mcu_temperature(error_level value) {
  extern error_level __oe_error_level_mcu_temperature;
  if (__oe_error_level_mcu_temperature != value) {
    __oe_error_level_mcu_temperature = value;
    if (errors_interval_job.climax > errors_interval_job.job.stream_job.last_schedule + 1) {
      errors_interval_job.climax = errors_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&errors_interval_job);
    }
  }
}
void canzero_set_error_level_lim_temperature(error_level value) {
  extern error_level __oe_error_level_lim_temperature;
  if (__oe_error_level_lim_temperature != value) {
    __oe_error_level_lim_temperature = value;
    if (errors_interval_job.climax > errors_interval_job.job.stream_job.last_schedule + 1) {
      errors_interval_job.climax = errors_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&errors_interval_job);
    }
  }
}
void canzero_set_precharge_status(sdc_status value) {
  extern sdc_status __oe_precharge_status;
  if (__oe_precharge_status != value) {
    __oe_precharge_status = value;
    if (state_interval_job.climax > state_interval_job.job.stream_job.last_schedule + 1) {
      state_interval_job.climax = state_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&state_interval_job);
    }
  }
}
void canzero_set_feedthrough_status(sdc_status value) {
  extern sdc_status __oe_feedthrough_status;
  if (__oe_feedthrough_status != value) {
    __oe_feedthrough_status = value;
    if (state_interval_job.climax > state_interval_job.job.stream_job.last_schedule + 1) {
      state_interval_job.climax = state_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&state_interval_job);
    }
  }
}
void canzero_set_command(motor_command value) {
  extern motor_command __oe_command;
  if (__oe_command != value) {
    __oe_command = value;
    if (state_interval_job.climax > state_interval_job.job.stream_job.last_schedule + 1) {
      state_interval_job.climax = state_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&state_interval_job);
    }
  }
}
void canzero_set_control_active(bool_t value) {
  extern bool_t __oe_control_active;
  if (__oe_control_active != value) {
    __oe_control_active = value;
    if (state_interval_job.climax > state_interval_job.job.stream_job.last_schedule + 1) {
      state_interval_job.climax = state_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&state_interval_job);
    }
  }
}
void canzero_set_error_vdc_voltage_invalid(error_flag value) {
  extern error_flag __oe_error_vdc_voltage_invalid;
  if (__oe_error_vdc_voltage_invalid != value) {
    __oe_error_vdc_voltage_invalid = value;
    if (errors_interval_job.climax > errors_interval_job.job.stream_job.last_schedule + 1) {
      errors_interval_job.climax = errors_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&errors_interval_job);
    }
  }
}
void canzero_set_last_node_missed(uint8_t value) {
  extern uint8_t __oe_last_node_missed;
  if (__oe_last_node_missed != value) {
    __oe_last_node_missed = value;
    if (errors_interval_job.climax > errors_interval_job.job.stream_job.last_schedule + 1) {
      errors_interval_job.climax = errors_interval_job.job.stream_job.last_schedule + 1;
      scheduler_promote_job(&errors_interval_job);
    }
  }
}
static uint32_t DMAMEM __oe_config_hash_send_fragmentation_buffer[2];
static uint32_t DMAMEM __oe_build_time_send_fragmentation_buffer[2];
static uint32_t DMAMEM __oe_error_level_config_current_phase_send_fragmentation_buffer[7];
static uint32_t DMAMEM __oe_acceleration_calibration_variance_send_fragmentation_buffer[2];
static uint32_t DMAMEM __oe_acceleration_calibration_offset_send_fragmentation_buffer[2];
static uint32_t DMAMEM __oe_error_level_config_vdc_voltage_send_fragmentation_buffer[7];
static uint32_t DMAMEM __oe_error_level_config_board_temperature_send_fragmentation_buffer[7];
static uint32_t DMAMEM __oe_error_level_config_mcu_temperature_send_fragmentation_buffer[7];
static uint32_t DMAMEM __oe_error_level_config_lim_temperature_send_fragmentation_buffer[7];
void canzero_send_config_hash() {
  canzero_message_get_resp msg;
  {
    uint64_t masked = (__oe_config_hash & (0xFFFFFFFFFFFFFFFF >> (64 - 64)));
    __oe_config_hash_send_fragmentation_buffer[0] = ((uint32_t*)&masked)[0];
    __oe_config_hash_send_fragmentation_buffer[1] = ((uint32_t*)&masked)[1];
  }
  msg.m_data = __oe_config_hash_send_fragmentation_buffer[0];
  msg.m_header.m_eof = 0;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 0;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
  schedule_get_resp_fragmentation_job(__oe_config_hash_send_fragmentation_buffer, 2, 0, 255);

}
void canzero_send_build_time() {
  canzero_message_get_resp msg;
  __oe_build_time_send_fragmentation_buffer[0] = (__oe_build_time.m_year & (0xFFFFFFFF >> (32 - 16)));
  __oe_build_time_send_fragmentation_buffer[0] |= ((__oe_build_time.m_month & (0xFFFFFFFF >> (32 - 8))) << 16);
  __oe_build_time_send_fragmentation_buffer[0] |= ((__oe_build_time.m_day & (0xFFFFFFFF >> (32 - 8))) << 24);
  __oe_build_time_send_fragmentation_buffer[1] = (__oe_build_time.m_hour & (0xFFFFFFFF >> (32 - 8)));
  __oe_build_time_send_fragmentation_buffer[1] |= ((__oe_build_time.m_min & (0xFFFFFFFF >> (32 - 8))) << 8);
  __oe_build_time_send_fragmentation_buffer[1] |= ((__oe_build_time.m_sec & (0xFFFFFFFF >> (32 - 8))) << 16);

  msg.m_data = __oe_build_time_send_fragmentation_buffer[0];
  msg.m_header.m_eof = 0;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 1;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
  schedule_get_resp_fragmentation_job(__oe_build_time_send_fragmentation_buffer, 2, 1, 255);

}
void canzero_send_state() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_state) & (0xFF >> (8 - 3)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 2;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_sdc_status() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_sdc_status) & (0xFF >> (8 - 1)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 3;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_assertion_fault() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_assertion_fault) & (0xFF >> (8 - 1)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 4;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_error_arming_failed() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_error_arming_failed) & (0xFF >> (8 - 1)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 5;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_error_precharge_failed() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_error_precharge_failed) & (0xFF >> (8 - 1)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 6;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_error_acceleration_out_of_range() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_error_acceleration_out_of_range) & (0xFF >> (8 - 1)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 7;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_error_acceleration_calibration_failed() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_error_acceleration_calibration_failed) & (0xFF >> (8 - 1)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 8;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_error_invalid_target_acceleration() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_error_invalid_target_acceleration) & (0xFF >> (8 - 1)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 9;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_error_board_temperature1_invalid() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_error_board_temperature1_invalid) & (0xFF >> (8 - 1)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 10;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_error_board_temperature2_invalid() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_error_board_temperature2_invalid) & (0xFF >> (8 - 1)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 11;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_error_board_temperature3_invalid() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_error_board_temperature3_invalid) & (0xFF >> (8 - 1)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 12;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_error_mcu_temperature_invalid() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_error_mcu_temperature_invalid) & (0xFF >> (8 - 1)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 13;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_error_lim_temperature1_invalid() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_error_lim_temperature1_invalid) & (0xFF >> (8 - 1)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 14;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_error_lim_temperature2_invalid() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_error_lim_temperature2_invalid) & (0xFF >> (8 - 1)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 15;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_error_lim_temperature3_invalid() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_error_lim_temperature3_invalid) & (0xFF >> (8 - 1)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 16;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_error_lim_temperature4_invalid() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_error_lim_temperature4_invalid) & (0xFF >> (8 - 1)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 17;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_error_heartbeat_miss() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_error_heartbeat_miss) & (0xFF >> (8 - 1)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 18;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_error_level_current_u1() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_error_level_current_u1) & (0xFF >> (8 - 2)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 19;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_error_level_current_v1() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_error_level_current_v1) & (0xFF >> (8 - 2)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 20;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_error_level_current_w1() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_error_level_current_w1) & (0xFF >> (8 - 2)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 21;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_error_level_current_u2() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_error_level_current_u2) & (0xFF >> (8 - 2)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 22;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_error_level_current_v2() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_error_level_current_v2) & (0xFF >> (8 - 2)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 23;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_error_level_current_w2() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_error_level_current_w2) & (0xFF >> (8 - 2)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 24;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_error_level_vdc_voltage() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_error_level_vdc_voltage) & (0xFF >> (8 - 2)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 25;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_error_level_board_temperature() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_error_level_board_temperature) & (0xFF >> (8 - 2)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 26;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_error_level_mcu_temperature() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_error_level_mcu_temperature) & (0xFF >> (8 - 2)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 27;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_error_level_lim_temperature() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_error_level_lim_temperature) & (0xFF >> (8 - 2)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 28;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_precharge_status() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_precharge_status) & (0xFF >> (8 - 1)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 29;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_feedthrough_status() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_feedthrough_status) & (0xFF >> (8 - 1)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 30;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_command() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_command) & (0xFF >> (8 - 3)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 31;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_control_active() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_control_active) & (0xFF >> (8 - 1)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 32;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_current_u1() {
  canzero_message_get_resp msg;
  msg.m_data |= min_u32((__oe_current_u1 - (-100)) / 0.0030518043793392844, 0xFFFF) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 33;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_current_v1() {
  canzero_message_get_resp msg;
  msg.m_data |= min_u32((__oe_current_v1 - (-100)) / 0.0030518043793392844, 0xFFFF) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 34;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_current_w1() {
  canzero_message_get_resp msg;
  msg.m_data |= min_u32((__oe_current_w1 - (-100)) / 0.0030518043793392844, 0xFFFF) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 35;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_current_u2() {
  canzero_message_get_resp msg;
  msg.m_data |= min_u32((__oe_current_u2 - (-100)) / 0.0030518043793392844, 0xFFFF) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 36;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_current_v2() {
  canzero_message_get_resp msg;
  msg.m_data |= min_u32((__oe_current_v2 - (-100)) / 0.0030518043793392844, 0xFFFF) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 37;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_current_w2() {
  canzero_message_get_resp msg;
  msg.m_data |= min_u32((__oe_current_w2 - (-100)) / 0.0030518043793392844, 0xFFFF) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 38;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_current_max() {
  canzero_message_get_resp msg;
  msg.m_data |= min_u32((__oe_current_max - (-100)) / 0.0030518043793392844, 0xFFFF) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 39;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_current_average() {
  canzero_message_get_resp msg;
  msg.m_data |= min_u32((__oe_current_average - (-100)) / 0.0030518043793392844, 0xFFFF) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 40;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_error_level_config_current_phase() {
  canzero_message_get_resp msg;
  __oe_error_level_config_current_phase_send_fragmentation_buffer[0] = (min_u32((__oe_error_level_config_current_phase.m_info_thresh - ((float)-1000)) / (float)0.0000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
  __oe_error_level_config_current_phase_send_fragmentation_buffer[1] = (min_u32((__oe_error_level_config_current_phase.m_info_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
  __oe_error_level_config_current_phase_send_fragmentation_buffer[2] = (min_u32((__oe_error_level_config_current_phase.m_warning_thresh - ((float)-1000)) / (float)0.0000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
  __oe_error_level_config_current_phase_send_fragmentation_buffer[3] = (min_u32((__oe_error_level_config_current_phase.m_warning_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
  __oe_error_level_config_current_phase_send_fragmentation_buffer[4] = (min_u32((__oe_error_level_config_current_phase.m_error_thresh - ((float)-1000)) / (float)0.0000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
  __oe_error_level_config_current_phase_send_fragmentation_buffer[5] = (min_u32((__oe_error_level_config_current_phase.m_error_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
  __oe_error_level_config_current_phase_send_fragmentation_buffer[6] = (__oe_error_level_config_current_phase.m_ignore_info & (0xFFFFFFFF >> (32 - 1)));
  __oe_error_level_config_current_phase_send_fragmentation_buffer[6] |= ((__oe_error_level_config_current_phase.m_ignore_warning & (0xFFFFFFFF >> (32 - 1))) << 1);
  __oe_error_level_config_current_phase_send_fragmentation_buffer[6] |= ((__oe_error_level_config_current_phase.m_ignore_error & (0xFFFFFFFF >> (32 - 1))) << 2);

  msg.m_data = __oe_error_level_config_current_phase_send_fragmentation_buffer[0];
  msg.m_header.m_eof = 0;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 41;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
  schedule_get_resp_fragmentation_job(__oe_error_level_config_current_phase_send_fragmentation_buffer, 7, 41, 255);

}
void canzero_send_frequency() {
  canzero_message_get_resp msg;
  msg.m_data |= min_u32((__oe_frequency - (0)) / 0.0007629510948348211, 0xFFFF) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 42;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_modulation_index() {
  canzero_message_get_resp msg;
  msg.m_data |= min_u32((__oe_modulation_index - (0)) / 0.000015259021896696422, 0xFFFF) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 43;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_acceleration() {
  canzero_message_get_resp msg;
  msg.m_data |= min_u32((__oe_acceleration - (-10)) / 0.00030518043793392844, 0xFFFF) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 44;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_acceleration_calibration_variance() {
  canzero_message_get_resp msg;
  {
    uint64_t masked = (min_u64((__oe_acceleration_calibration_variance - ((double)-1)) / (double)0.00000000000000000010842021724855044, 0xFFFFFFFFFFFFFFFFull) & (0xFFFFFFFFFFFFFFFF >> (64 - 64)));
    __oe_acceleration_calibration_variance_send_fragmentation_buffer[0] = ((uint32_t*)&masked)[0];
    __oe_acceleration_calibration_variance_send_fragmentation_buffer[1] = ((uint32_t*)&masked)[1];
  }
  msg.m_data = __oe_acceleration_calibration_variance_send_fragmentation_buffer[0];
  msg.m_header.m_eof = 0;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 45;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
  schedule_get_resp_fragmentation_job(__oe_acceleration_calibration_variance_send_fragmentation_buffer, 2, 45, 255);

}
void canzero_send_acceleration_calibration_offset() {
  canzero_message_get_resp msg;
  {
    uint64_t masked = (min_u64((__oe_acceleration_calibration_offset - ((double)-1)) / (double)0.00000000000000000010842021724855044, 0xFFFFFFFFFFFFFFFFull) & (0xFFFFFFFFFFFFFFFF >> (64 - 64)));
    __oe_acceleration_calibration_offset_send_fragmentation_buffer[0] = ((uint32_t*)&masked)[0];
    __oe_acceleration_calibration_offset_send_fragmentation_buffer[1] = ((uint32_t*)&masked)[1];
  }
  msg.m_data = __oe_acceleration_calibration_offset_send_fragmentation_buffer[0];
  msg.m_header.m_eof = 0;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 46;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
  schedule_get_resp_fragmentation_job(__oe_acceleration_calibration_offset_send_fragmentation_buffer, 2, 46, 255);

}
void canzero_send_target_acceleration() {
  canzero_message_get_resp msg;
  msg.m_data |= min_u32((__oe_target_acceleration - (-50)) / 0.000000023283064370807974, 0xFFFFFFFF) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 47;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_external_acceleration() {
  canzero_message_get_resp msg;
  msg.m_data |= min_u32((__oe_external_acceleration - (-10)) / 0.00030518043793392844, 0xFFFF) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 48;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_external_velocity() {
  canzero_message_get_resp msg;
  msg.m_data |= min_u32((__oe_external_velocity - (-10)) / 0.00030518043793392844, 0xFFFF) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 49;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_external_position() {
  canzero_message_get_resp msg;
  msg.m_data |= min_u32((__oe_external_position - (-50)) / 0.0015259021896696422, 0xFFFF) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 50;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_vdc_voltage() {
  canzero_message_get_resp msg;
  msg.m_data |= min_u32((__oe_vdc_voltage - (-100)) / 0.0030518043793392844, 0xFFFF) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 51;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_error_vdc_voltage_invalid() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_error_vdc_voltage_invalid) & (0xFF >> (8 - 1)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 52;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_error_level_config_vdc_voltage() {
  canzero_message_get_resp msg;
  __oe_error_level_config_vdc_voltage_send_fragmentation_buffer[0] = (min_u32((__oe_error_level_config_vdc_voltage.m_info_thresh - ((float)-1000)) / (float)0.0000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
  __oe_error_level_config_vdc_voltage_send_fragmentation_buffer[1] = (min_u32((__oe_error_level_config_vdc_voltage.m_info_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
  __oe_error_level_config_vdc_voltage_send_fragmentation_buffer[2] = (min_u32((__oe_error_level_config_vdc_voltage.m_warning_thresh - ((float)-1000)) / (float)0.0000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
  __oe_error_level_config_vdc_voltage_send_fragmentation_buffer[3] = (min_u32((__oe_error_level_config_vdc_voltage.m_warning_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
  __oe_error_level_config_vdc_voltage_send_fragmentation_buffer[4] = (min_u32((__oe_error_level_config_vdc_voltage.m_error_thresh - ((float)-1000)) / (float)0.0000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
  __oe_error_level_config_vdc_voltage_send_fragmentation_buffer[5] = (min_u32((__oe_error_level_config_vdc_voltage.m_error_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
  __oe_error_level_config_vdc_voltage_send_fragmentation_buffer[6] = (__oe_error_level_config_vdc_voltage.m_ignore_info & (0xFFFFFFFF >> (32 - 1)));
  __oe_error_level_config_vdc_voltage_send_fragmentation_buffer[6] |= ((__oe_error_level_config_vdc_voltage.m_ignore_warning & (0xFFFFFFFF >> (32 - 1))) << 1);
  __oe_error_level_config_vdc_voltage_send_fragmentation_buffer[6] |= ((__oe_error_level_config_vdc_voltage.m_ignore_error & (0xFFFFFFFF >> (32 - 1))) << 2);

  msg.m_data = __oe_error_level_config_vdc_voltage_send_fragmentation_buffer[0];
  msg.m_header.m_eof = 0;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 53;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
  schedule_get_resp_fragmentation_job(__oe_error_level_config_vdc_voltage_send_fragmentation_buffer, 7, 53, 255);

}
void canzero_send_board_temperature1() {
  canzero_message_get_resp msg;
  msg.m_data |= min_u32((__oe_board_temperature1 - (-1)) / 0.592156862745098, 0xFF) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 54;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_board_temperature2() {
  canzero_message_get_resp msg;
  msg.m_data |= min_u32((__oe_board_temperature2 - (-1)) / 0.592156862745098, 0xFF) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 55;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_board_temperature3() {
  canzero_message_get_resp msg;
  msg.m_data |= min_u32((__oe_board_temperature3 - (-1)) / 0.592156862745098, 0xFF) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 56;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_board_avg_temperature() {
  canzero_message_get_resp msg;
  msg.m_data |= min_u32((__oe_board_avg_temperature - (-1)) / 0.592156862745098, 0xFF) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 57;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_board_max_temperature() {
  canzero_message_get_resp msg;
  msg.m_data |= min_u32((__oe_board_max_temperature - (-1)) / 0.592156862745098, 0xFF) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 58;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_board_min_temperature() {
  canzero_message_get_resp msg;
  msg.m_data |= min_u32((__oe_board_min_temperature - (-1)) / 0.592156862745098, 0xFF) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 59;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_error_level_config_board_temperature() {
  canzero_message_get_resp msg;
  __oe_error_level_config_board_temperature_send_fragmentation_buffer[0] = (min_u32((__oe_error_level_config_board_temperature.m_info_thresh - ((float)-1000)) / (float)0.0000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
  __oe_error_level_config_board_temperature_send_fragmentation_buffer[1] = (min_u32((__oe_error_level_config_board_temperature.m_info_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
  __oe_error_level_config_board_temperature_send_fragmentation_buffer[2] = (min_u32((__oe_error_level_config_board_temperature.m_warning_thresh - ((float)-1000)) / (float)0.0000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
  __oe_error_level_config_board_temperature_send_fragmentation_buffer[3] = (min_u32((__oe_error_level_config_board_temperature.m_warning_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
  __oe_error_level_config_board_temperature_send_fragmentation_buffer[4] = (min_u32((__oe_error_level_config_board_temperature.m_error_thresh - ((float)-1000)) / (float)0.0000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
  __oe_error_level_config_board_temperature_send_fragmentation_buffer[5] = (min_u32((__oe_error_level_config_board_temperature.m_error_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
  __oe_error_level_config_board_temperature_send_fragmentation_buffer[6] = (__oe_error_level_config_board_temperature.m_ignore_info & (0xFFFFFFFF >> (32 - 1)));
  __oe_error_level_config_board_temperature_send_fragmentation_buffer[6] |= ((__oe_error_level_config_board_temperature.m_ignore_warning & (0xFFFFFFFF >> (32 - 1))) << 1);
  __oe_error_level_config_board_temperature_send_fragmentation_buffer[6] |= ((__oe_error_level_config_board_temperature.m_ignore_error & (0xFFFFFFFF >> (32 - 1))) << 2);

  msg.m_data = __oe_error_level_config_board_temperature_send_fragmentation_buffer[0];
  msg.m_header.m_eof = 0;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 60;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
  schedule_get_resp_fragmentation_job(__oe_error_level_config_board_temperature_send_fragmentation_buffer, 7, 60, 255);

}
void canzero_send_mcu_temperature() {
  canzero_message_get_resp msg;
  msg.m_data |= min_u32((__oe_mcu_temperature - (-1)) / 0.592156862745098, 0xFF) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 61;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_error_level_config_mcu_temperature() {
  canzero_message_get_resp msg;
  __oe_error_level_config_mcu_temperature_send_fragmentation_buffer[0] = (min_u32((__oe_error_level_config_mcu_temperature.m_info_thresh - ((float)-1000)) / (float)0.0000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
  __oe_error_level_config_mcu_temperature_send_fragmentation_buffer[1] = (min_u32((__oe_error_level_config_mcu_temperature.m_info_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
  __oe_error_level_config_mcu_temperature_send_fragmentation_buffer[2] = (min_u32((__oe_error_level_config_mcu_temperature.m_warning_thresh - ((float)-1000)) / (float)0.0000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
  __oe_error_level_config_mcu_temperature_send_fragmentation_buffer[3] = (min_u32((__oe_error_level_config_mcu_temperature.m_warning_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
  __oe_error_level_config_mcu_temperature_send_fragmentation_buffer[4] = (min_u32((__oe_error_level_config_mcu_temperature.m_error_thresh - ((float)-1000)) / (float)0.0000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
  __oe_error_level_config_mcu_temperature_send_fragmentation_buffer[5] = (min_u32((__oe_error_level_config_mcu_temperature.m_error_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
  __oe_error_level_config_mcu_temperature_send_fragmentation_buffer[6] = (__oe_error_level_config_mcu_temperature.m_ignore_info & (0xFFFFFFFF >> (32 - 1)));
  __oe_error_level_config_mcu_temperature_send_fragmentation_buffer[6] |= ((__oe_error_level_config_mcu_temperature.m_ignore_warning & (0xFFFFFFFF >> (32 - 1))) << 1);
  __oe_error_level_config_mcu_temperature_send_fragmentation_buffer[6] |= ((__oe_error_level_config_mcu_temperature.m_ignore_error & (0xFFFFFFFF >> (32 - 1))) << 2);

  msg.m_data = __oe_error_level_config_mcu_temperature_send_fragmentation_buffer[0];
  msg.m_header.m_eof = 0;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 62;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
  schedule_get_resp_fragmentation_job(__oe_error_level_config_mcu_temperature_send_fragmentation_buffer, 7, 62, 255);

}
void canzero_send_lim_temperature1() {
  canzero_message_get_resp msg;
  msg.m_data |= min_u32((__oe_lim_temperature1 - (-1)) / 0.592156862745098, 0xFF) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 63;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_lim_temperature2() {
  canzero_message_get_resp msg;
  msg.m_data |= min_u32((__oe_lim_temperature2 - (-1)) / 0.592156862745098, 0xFF) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 64;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_lim_temperature3() {
  canzero_message_get_resp msg;
  msg.m_data |= min_u32((__oe_lim_temperature3 - (-1)) / 0.592156862745098, 0xFF) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 65;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_lim_temperature4() {
  canzero_message_get_resp msg;
  msg.m_data |= min_u32((__oe_lim_temperature4 - (-1)) / 0.592156862745098, 0xFF) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 66;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_lim_average_temperature() {
  canzero_message_get_resp msg;
  msg.m_data |= min_u32((__oe_lim_average_temperature - (-1)) / 0.592156862745098, 0xFF) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 67;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_lim_max_temperature() {
  canzero_message_get_resp msg;
  msg.m_data |= min_u32((__oe_lim_max_temperature - (-1)) / 0.592156862745098, 0xFF) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 68;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_lim_min_temperature() {
  canzero_message_get_resp msg;
  msg.m_data |= min_u32((__oe_lim_min_temperature - (-1)) / 0.592156862745098, 0xFF) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 69;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_error_level_config_lim_temperature() {
  canzero_message_get_resp msg;
  __oe_error_level_config_lim_temperature_send_fragmentation_buffer[0] = (min_u32((__oe_error_level_config_lim_temperature.m_info_thresh - ((float)-1000)) / (float)0.0000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
  __oe_error_level_config_lim_temperature_send_fragmentation_buffer[1] = (min_u32((__oe_error_level_config_lim_temperature.m_info_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
  __oe_error_level_config_lim_temperature_send_fragmentation_buffer[2] = (min_u32((__oe_error_level_config_lim_temperature.m_warning_thresh - ((float)-1000)) / (float)0.0000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
  __oe_error_level_config_lim_temperature_send_fragmentation_buffer[3] = (min_u32((__oe_error_level_config_lim_temperature.m_warning_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
  __oe_error_level_config_lim_temperature_send_fragmentation_buffer[4] = (min_u32((__oe_error_level_config_lim_temperature.m_error_thresh - ((float)-1000)) / (float)0.0000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
  __oe_error_level_config_lim_temperature_send_fragmentation_buffer[5] = (min_u32((__oe_error_level_config_lim_temperature.m_error_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
  __oe_error_level_config_lim_temperature_send_fragmentation_buffer[6] = (__oe_error_level_config_lim_temperature.m_ignore_info & (0xFFFFFFFF >> (32 - 1)));
  __oe_error_level_config_lim_temperature_send_fragmentation_buffer[6] |= ((__oe_error_level_config_lim_temperature.m_ignore_warning & (0xFFFFFFFF >> (32 - 1))) << 1);
  __oe_error_level_config_lim_temperature_send_fragmentation_buffer[6] |= ((__oe_error_level_config_lim_temperature.m_ignore_error & (0xFFFFFFFF >> (32 - 1))) << 2);

  msg.m_data = __oe_error_level_config_lim_temperature_send_fragmentation_buffer[0];
  msg.m_header.m_eof = 0;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 70;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
  schedule_get_resp_fragmentation_job(__oe_error_level_config_lim_temperature_send_fragmentation_buffer, 7, 70, 255);

}
void canzero_send_loop_frequency() {
  canzero_message_get_resp msg;
  msg.m_data |= min_u32((__oe_loop_frequency - (0)) / 0.00000023283064370807974, 0xFFFFFFFF) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 71;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_last_node_missed() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(__oe_last_node_missed & (0xFF >> (8 - 8)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 72;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
void canzero_send_ignore_45v() {
  canzero_message_get_resp msg;
  msg.m_data |= ((uint32_t)(((uint8_t)__oe_ignore_45v) & (0xFF >> (8 - 1)))) << 0;
  msg.m_header.m_eof = 1;
  msg.m_header.m_sof = 1;
  msg.m_header.m_toggle = 0;
  msg.m_header.m_od_index = 73;
  msg.m_header.m_client_id = 255;
  msg.m_header.m_server_id = node_id_motor_driver;
  canzero_frame sender_frame;
  canzero_serialize_canzero_message_get_resp(&msg, &sender_frame);
  canzero_can0_send(&sender_frame);
}
