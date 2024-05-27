#ifndef CANZERO_H
#define CANZERO_H
#include <cinttypes>
#include <cstddef>
typedef struct {
  uint16_t m_od_index;
  uint8_t m_client_id;
  uint8_t m_server_id;
} get_req_header;
typedef struct {
  uint8_t m_sof;
  uint8_t m_eof;
  uint8_t m_toggle;
  uint16_t m_od_index;
  uint8_t m_client_id;
  uint8_t m_server_id;
} set_req_header;
typedef enum {
  motor_command_NONE = 0,
  motor_command_PRECHARGE = 1,
  motor_command_ACCELERATE = 2,
  motor_command_DECELERATE = 3,
  motor_command_DISCONNECT = 4,
  motor_command_MANUAL_CONTROL = 5,
} motor_command;
typedef enum {
  node_id_mother_board = 0,
  node_id_motor_driver = 1,
  node_id_guidance_board_front = 2,
  node_id_guidance_board_back = 3,
  node_id_levitation_board1 = 4,
  node_id_levitation_board2 = 5,
  node_id_levitation_board3 = 6,
  node_id_levitation_board4 = 7,
  node_id_levitation_board5 = 8,
  node_id_levitation_board6 = 9,
  node_id_input_board = 10,
  node_id_power_board12 = 11,
  node_id_power_board24 = 12,
  node_id_gamepad = 13,
  node_id_count = 14,
} node_id;
typedef struct {
  uint8_t m_sof;
  uint8_t m_eof;
  uint8_t m_toggle;
  uint16_t m_od_index;
  uint8_t m_client_id;
  uint8_t m_server_id;
} get_resp_header;
typedef enum {
  set_resp_erno_Success = 0,
  set_resp_erno_Error = 1,
} set_resp_erno;
typedef struct {
  uint16_t m_od_index;
  uint8_t m_client_id;
  uint8_t m_server_id;
  set_resp_erno m_erno;
} set_resp_header;
typedef enum {
  motor_state_INIT = 0,
  motor_state_IDLE = 1,
  motor_state_PRECHARGE = 2,
  motor_state_READY = 3,
  motor_state_ACCELERATE = 4,
  motor_state_CONTROL = 5,
  motor_state_DECELERATE = 6,
  motor_state_MANUAL = 7,
  motor_state_ERROR = 8,
} motor_state;
typedef enum {
  sdc_status_OPEN = 0,
  sdc_status_CLOSED = 1,
} sdc_status;
typedef enum {
  error_flag_OK = 0,
  error_flag_ERROR = 1,
} error_flag;
typedef enum {
  error_level_OK = 0,
  error_level_INFO = 1,
  error_level_WARNING = 2,
  error_level_ERROR = 3,
} error_level;
typedef struct {
  uint16_t m_year;
  uint8_t m_month;
  uint8_t m_day;
  uint8_t m_hour;
  uint8_t m_min;
  uint8_t m_sec;
} date_time;
typedef enum {
  bool_t_FALSE = 0,
  bool_t_TRUE = 1,
} bool_t;
typedef struct {
  float m_info_thresh;
  float m_info_timeout;
  float m_warning_thresh;
  float m_warning_timeout;
  float m_error_thresh;
  float m_error_timeout;
  bool_t m_ignore_info;
  bool_t m_ignore_warning;
  bool_t m_ignore_error;
} error_level_config;
typedef struct {
  uint32_t id;
  uint8_t dlc;
  uint8_t data[8];
} canzero_frame;
typedef enum : uint32_t {
  CANZERO_FRAME_IDE_BIT = 0x40000000, // 1 << 30
  CANZERO_FRAME_RTR_BIT = 0x80000000, // 1 << 31
} can_frame_id_bits;
typedef struct {
  uint32_t mask;
  uint32_t id;
} canzero_can_filter;
extern void canzero_can0_setup(uint32_t baudrate, canzero_can_filter* filters, int filter_count);
extern void canzero_can0_send(canzero_frame* frame);
extern int canzero_can0_recv(canzero_frame* frame);
extern void canzero_can1_setup(uint32_t baudrate, canzero_can_filter* filters, int filter_count);
extern void canzero_can1_send(canzero_frame* frame);
extern int canzero_can1_recv(canzero_frame* frame);
extern void canzero_request_update(uint32_t time);
extern uint32_t canzero_get_time();
extern void canzero_enter_critical();
extern void canzero_exit_critical();
static inline uint64_t canzero_get_config_hash() {
  extern uint64_t __oe_config_hash;
  return __oe_config_hash;
}
static inline date_time canzero_get_build_time() {
  extern date_time __oe_build_time;
  return __oe_build_time;
}
static inline motor_state canzero_get_state() {
  extern motor_state __oe_state;
  return __oe_state;
}
static inline sdc_status canzero_get_sdc_status() {
  extern sdc_status __oe_sdc_status;
  return __oe_sdc_status;
}
static inline motor_command canzero_get_command() {
  extern motor_command __oe_command;
  return __oe_command;
}
static inline float canzero_get_acceleration() {
  extern float __oe_acceleration;
  return __oe_acceleration;
}
static inline double canzero_get_acceleration_calibration_variance() {
  extern double __oe_acceleration_calibration_variance;
  return __oe_acceleration_calibration_variance;
}
static inline double canzero_get_acceleration_calibration_offset() {
  extern double __oe_acceleration_calibration_offset;
  return __oe_acceleration_calibration_offset;
}
static inline error_flag canzero_get_error_acceleration_out_of_range() {
  extern error_flag __oe_error_acceleration_out_of_range;
  return __oe_error_acceleration_out_of_range;
}
static inline error_flag canzero_get_error_acceleration_calibration_failed() {
  extern error_flag __oe_error_acceleration_calibration_failed;
  return __oe_error_acceleration_calibration_failed;
}
static inline float canzero_get_target_acceleration() {
  extern float __oe_target_acceleration;
  return __oe_target_acceleration;
}
static inline error_flag canzero_get_error_invalid_target_acceleration() {
  extern error_flag __oe_error_invalid_target_acceleration;
  return __oe_error_invalid_target_acceleration;
}
static inline float canzero_get_external_acceleration() {
  extern float __oe_external_acceleration;
  return __oe_external_acceleration;
}
static inline float canzero_get_external_velocity() {
  extern float __oe_external_velocity;
  return __oe_external_velocity;
}
static inline float canzero_get_external_position() {
  extern float __oe_external_position;
  return __oe_external_position;
}
static inline float canzero_get_vdc45() {
  extern float __oe_vdc45;
  return __oe_vdc45;
}
static inline error_level canzero_get_error_level_over_vdc45() {
  extern error_level __oe_error_level_over_vdc45;
  return __oe_error_level_over_vdc45;
}
static inline error_level_config canzero_get_error_level_config_over_vdc45() {
  extern error_level_config __oe_error_level_config_over_vdc45;
  return __oe_error_level_config_over_vdc45;
}
static inline error_level canzero_get_error_level_under_vdc45() {
  extern error_level __oe_error_level_under_vdc45;
  return __oe_error_level_under_vdc45;
}
static inline error_level_config canzero_get_error_level_config_under_vdc45() {
  extern error_level_config __oe_error_level_config_under_vdc45;
  return __oe_error_level_config_under_vdc45;
}
static inline float canzero_get_mcu_temperature() {
  extern float __oe_mcu_temperature;
  return __oe_mcu_temperature;
}
static inline error_flag canzero_get_error_mcu_temperature_invalid() {
  extern error_flag __oe_error_mcu_temperature_invalid;
  return __oe_error_mcu_temperature_invalid;
}
static inline error_level canzero_get_error_level_mcu_temperature() {
  extern error_level __oe_error_level_mcu_temperature;
  return __oe_error_level_mcu_temperature;
}
static inline error_level_config canzero_get_error_level_config_mcu_temperature() {
  extern error_level_config __oe_error_level_config_mcu_temperature;
  return __oe_error_level_config_mcu_temperature;
}
static inline error_flag canzero_get_assertion_fault() {
  extern error_flag __oe_assertion_fault;
  return __oe_assertion_fault;
}
typedef struct {
  get_resp_header m_header;
  uint32_t m_data;
} canzero_message_get_resp;
static const uint32_t canzero_message_get_resp_id = 0xD9;
typedef struct {
  set_resp_header m_header;
} canzero_message_set_resp;
static const uint32_t canzero_message_set_resp_id = 0xD8;
typedef struct {
  motor_state m_state;
  sdc_status m_sdc_status;
} canzero_message_motor_driver_stream_state;
static const uint32_t canzero_message_motor_driver_stream_state_id = 0xBD;
typedef struct {
  float m_mcu_temperature;
} canzero_message_motor_driver_stream_temperatures;
static const uint32_t canzero_message_motor_driver_stream_temperatures_id = 0x50;
typedef struct {
  error_flag m_error_acceleration_out_of_range;
  error_level m_error_level_over_vdc45;
  error_level m_error_level_under_vdc45;
  error_flag m_error_mcu_temperature_invalid;
  error_level m_error_level_mcu_temperature;
  error_flag m_assertion_fault;
} canzero_message_motor_driver_stream_errors;
static const uint32_t canzero_message_motor_driver_stream_errors_id = 0xB1;
typedef struct {
  node_id m_node_id;
  uint8_t m_unregister;
  uint8_t m_ticks_next;
} canzero_message_heartbeat_can0;
static const uint32_t canzero_message_heartbeat_can0_id = 0xED;
typedef struct {
  node_id m_node_id;
  uint8_t m_unregister;
  uint8_t m_ticks_next;
} canzero_message_heartbeat_can1;
static const uint32_t canzero_message_heartbeat_can1_id = 0xEC;
typedef struct {
  get_req_header m_header;
} canzero_message_get_req;
static const uint32_t canzero_message_get_req_id = 0xDB;
typedef struct {
  set_req_header m_header;
  uint32_t m_data;
} canzero_message_set_req;
static const uint32_t canzero_message_set_req_id = 0xDA;
typedef struct {
  float m_target_acceleration;
  motor_command m_motor_driver_command;
} canzero_message_mother_board_stream_motor_command;
static const uint32_t canzero_message_mother_board_stream_motor_command_id = 0x4B;
void canzero_can0_poll();
void canzero_can1_poll();
uint32_t canzero_update_continue(uint32_t delta_time);
void canzero_init();
static inline void canzero_set_config_hash(uint64_t value){
  extern uint64_t __oe_config_hash;
  __oe_config_hash = value;
}
static inline void canzero_set_build_time(date_time value){
  extern date_time __oe_build_time;
  __oe_build_time = value;
}
void canzero_set_state(motor_state value);
void canzero_set_sdc_status(sdc_status value);
static inline void canzero_set_command(motor_command value){
  extern motor_command __oe_command;
  __oe_command = value;
}
static inline void canzero_set_acceleration(float value){
  extern float __oe_acceleration;
  __oe_acceleration = value;
}
static inline void canzero_set_acceleration_calibration_variance(double value){
  extern double __oe_acceleration_calibration_variance;
  __oe_acceleration_calibration_variance = value;
}
static inline void canzero_set_acceleration_calibration_offset(double value){
  extern double __oe_acceleration_calibration_offset;
  __oe_acceleration_calibration_offset = value;
}
void canzero_set_error_acceleration_out_of_range(error_flag value);
static inline void canzero_set_error_acceleration_calibration_failed(error_flag value){
  extern error_flag __oe_error_acceleration_calibration_failed;
  __oe_error_acceleration_calibration_failed = value;
}
static inline void canzero_set_target_acceleration(float value){
  extern float __oe_target_acceleration;
  __oe_target_acceleration = value;
}
static inline void canzero_set_error_invalid_target_acceleration(error_flag value){
  extern error_flag __oe_error_invalid_target_acceleration;
  __oe_error_invalid_target_acceleration = value;
}
static inline void canzero_set_external_acceleration(float value){
  extern float __oe_external_acceleration;
  __oe_external_acceleration = value;
}
static inline void canzero_set_external_velocity(float value){
  extern float __oe_external_velocity;
  __oe_external_velocity = value;
}
static inline void canzero_set_external_position(float value){
  extern float __oe_external_position;
  __oe_external_position = value;
}
static inline void canzero_set_vdc45(float value){
  extern float __oe_vdc45;
  __oe_vdc45 = value;
}
void canzero_set_error_level_over_vdc45(error_level value);
static inline void canzero_set_error_level_config_over_vdc45(error_level_config value){
  extern error_level_config __oe_error_level_config_over_vdc45;
  __oe_error_level_config_over_vdc45 = value;
}
void canzero_set_error_level_under_vdc45(error_level value);
static inline void canzero_set_error_level_config_under_vdc45(error_level_config value){
  extern error_level_config __oe_error_level_config_under_vdc45;
  __oe_error_level_config_under_vdc45 = value;
}
static inline void canzero_set_mcu_temperature(float value){
  extern float __oe_mcu_temperature;
  __oe_mcu_temperature = value;
}
void canzero_set_error_mcu_temperature_invalid(error_flag value);
void canzero_set_error_level_mcu_temperature(error_level value);
static inline void canzero_set_error_level_config_mcu_temperature(error_level_config value){
  extern error_level_config __oe_error_level_config_mcu_temperature;
  __oe_error_level_config_mcu_temperature = value;
}
void canzero_set_assertion_fault(error_flag value);
#endif