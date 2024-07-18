#ifndef CANZERO_H
#define CANZERO_H
#include <cinttypes>
#include <cstddef>
#define MAX_DYN_HEARTBEATS 10
typedef enum {
  node_id_gamepad = 0,
  node_id_mother_board = 1,
  node_id_motor_driver = 2,
  node_id_guidance_board_front = 3,
  node_id_guidance_board_back = 4,
  node_id_levitation_board1 = 5,
  node_id_levitation_board2 = 6,
  node_id_levitation_board3 = 7,
  node_id_input_board = 8,
  node_id_power_board12 = 9,
  node_id_power_board24 = 10,
  node_id_led_board = 11,
  node_id_count = 12,
} node_id;
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
  motor_command_ARM45 = 1,
  motor_command_PRECHARGE = 2,
  motor_command_START = 3,
  motor_command_STOP = 4,
  motor_command_ABORT = 5,
  motor_command_DISARM45 = 6,
} motor_command;
typedef enum {
  bool_t_FALSE = 0,
  bool_t_TRUE = 1,
} bool_t;
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
  motor_state_ARMING45 = 2,
  motor_state_PRECHARGE = 3,
  motor_state_READY = 4,
  motor_state_CONTROL = 5,
  motor_state_DISARMING45 = 6,
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
static const node_id CANZERO_NODE_ID = node_id_motor_driver;
typedef struct {
  uint32_t id;
  uint8_t dlc;
  __attribute__((aligned(alignof(uint64_t)))) uint8_t data[8];
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
static inline error_flag canzero_get_assertion_fault() {
  extern error_flag __oe_assertion_fault;
  return __oe_assertion_fault;
}
static inline error_flag canzero_get_error_arming_failed() {
  extern error_flag __oe_error_arming_failed;
  return __oe_error_arming_failed;
}
static inline error_flag canzero_get_error_precharge_failed() {
  extern error_flag __oe_error_precharge_failed;
  return __oe_error_precharge_failed;
}
static inline error_flag canzero_get_error_acceleration_out_of_range() {
  extern error_flag __oe_error_acceleration_out_of_range;
  return __oe_error_acceleration_out_of_range;
}
static inline error_flag canzero_get_error_acceleration_calibration_failed() {
  extern error_flag __oe_error_acceleration_calibration_failed;
  return __oe_error_acceleration_calibration_failed;
}
static inline error_flag canzero_get_error_invalid_target_acceleration() {
  extern error_flag __oe_error_invalid_target_acceleration;
  return __oe_error_invalid_target_acceleration;
}
static inline error_flag canzero_get_error_vdc_voltage_invalid() {
  extern error_flag __oe_error_vdc_voltage_invalid;
  return __oe_error_vdc_voltage_invalid;
}
static inline error_flag canzero_get_error_board_temperature1_invalid() {
  extern error_flag __oe_error_board_temperature1_invalid;
  return __oe_error_board_temperature1_invalid;
}
static inline error_flag canzero_get_error_board_temperature2_invalid() {
  extern error_flag __oe_error_board_temperature2_invalid;
  return __oe_error_board_temperature2_invalid;
}
static inline error_flag canzero_get_error_board_temperature3_invalid() {
  extern error_flag __oe_error_board_temperature3_invalid;
  return __oe_error_board_temperature3_invalid;
}
static inline error_flag canzero_get_error_mcu_temperature_invalid() {
  extern error_flag __oe_error_mcu_temperature_invalid;
  return __oe_error_mcu_temperature_invalid;
}
static inline error_flag canzero_get_error_lim_temperature1_invalid() {
  extern error_flag __oe_error_lim_temperature1_invalid;
  return __oe_error_lim_temperature1_invalid;
}
static inline error_flag canzero_get_error_lim_temperature2_invalid() {
  extern error_flag __oe_error_lim_temperature2_invalid;
  return __oe_error_lim_temperature2_invalid;
}
static inline error_flag canzero_get_error_lim_temperature3_invalid() {
  extern error_flag __oe_error_lim_temperature3_invalid;
  return __oe_error_lim_temperature3_invalid;
}
static inline error_flag canzero_get_error_lim_temperature4_invalid() {
  extern error_flag __oe_error_lim_temperature4_invalid;
  return __oe_error_lim_temperature4_invalid;
}
static inline error_flag canzero_get_error_heartbeat_miss() {
  extern error_flag __oe_error_heartbeat_miss;
  return __oe_error_heartbeat_miss;
}
static inline error_flag canzero_get_error_phase_current_unexpected() {
  extern error_flag __oe_error_phase_current_unexpected;
  return __oe_error_phase_current_unexpected;
}
static inline error_flag canzero_get_error_sdc_brake() {
  extern error_flag __oe_error_sdc_brake;
  return __oe_error_sdc_brake;
}
static inline error_level canzero_get_error_level_current_u1() {
  extern error_level __oe_error_level_current_u1;
  return __oe_error_level_current_u1;
}
static inline error_level canzero_get_error_level_current_v1() {
  extern error_level __oe_error_level_current_v1;
  return __oe_error_level_current_v1;
}
static inline error_level canzero_get_error_level_current_w1() {
  extern error_level __oe_error_level_current_w1;
  return __oe_error_level_current_w1;
}
static inline error_level canzero_get_error_level_current_u2() {
  extern error_level __oe_error_level_current_u2;
  return __oe_error_level_current_u2;
}
static inline error_level canzero_get_error_level_current_v2() {
  extern error_level __oe_error_level_current_v2;
  return __oe_error_level_current_v2;
}
static inline error_level canzero_get_error_level_current_w2() {
  extern error_level __oe_error_level_current_w2;
  return __oe_error_level_current_w2;
}
static inline error_level canzero_get_error_level_vdc_voltage() {
  extern error_level __oe_error_level_vdc_voltage;
  return __oe_error_level_vdc_voltage;
}
static inline error_level canzero_get_error_level_board_temperature() {
  extern error_level __oe_error_level_board_temperature;
  return __oe_error_level_board_temperature;
}
static inline error_level canzero_get_error_level_mcu_temperature() {
  extern error_level __oe_error_level_mcu_temperature;
  return __oe_error_level_mcu_temperature;
}
static inline error_level canzero_get_error_level_lim_temperature() {
  extern error_level __oe_error_level_lim_temperature;
  return __oe_error_level_lim_temperature;
}
static inline error_flag canzero_get_phase_current_unexpected() {
  extern error_flag __oe_phase_current_unexpected;
  return __oe_phase_current_unexpected;
}
static inline sdc_status canzero_get_precharge_status() {
  extern sdc_status __oe_precharge_status;
  return __oe_precharge_status;
}
static inline sdc_status canzero_get_feedthrough_status() {
  extern sdc_status __oe_feedthrough_status;
  return __oe_feedthrough_status;
}
static inline motor_command canzero_get_command() {
  extern motor_command __oe_command;
  return __oe_command;
}
static inline bool_t canzero_get_control_active() {
  extern bool_t __oe_control_active;
  return __oe_control_active;
}
static inline float canzero_get_current_u1() {
  extern float __oe_current_u1;
  return __oe_current_u1;
}
static inline float canzero_get_current_v1() {
  extern float __oe_current_v1;
  return __oe_current_v1;
}
static inline float canzero_get_current_w1() {
  extern float __oe_current_w1;
  return __oe_current_w1;
}
static inline float canzero_get_current_u2() {
  extern float __oe_current_u2;
  return __oe_current_u2;
}
static inline float canzero_get_current_v2() {
  extern float __oe_current_v2;
  return __oe_current_v2;
}
static inline float canzero_get_current_w2() {
  extern float __oe_current_w2;
  return __oe_current_w2;
}
static inline float canzero_get_current_max() {
  extern float __oe_current_max;
  return __oe_current_max;
}
static inline float canzero_get_current_average() {
  extern float __oe_current_average;
  return __oe_current_average;
}
static inline error_level_config canzero_get_error_level_config_current_phase() {
  extern error_level_config __oe_error_level_config_current_phase;
  return __oe_error_level_config_current_phase;
}
static inline float canzero_get_frequency() {
  extern float __oe_frequency;
  return __oe_frequency;
}
static inline float canzero_get_modulation_index() {
  extern float __oe_modulation_index;
  return __oe_modulation_index;
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
static inline float canzero_get_target_acceleration() {
  extern float __oe_target_acceleration;
  return __oe_target_acceleration;
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
static inline float canzero_get_vdc_voltage() {
  extern float __oe_vdc_voltage;
  return __oe_vdc_voltage;
}
static inline error_level_config canzero_get_error_level_config_vdc_voltage() {
  extern error_level_config __oe_error_level_config_vdc_voltage;
  return __oe_error_level_config_vdc_voltage;
}
static inline float canzero_get_board_temperature1() {
  extern float __oe_board_temperature1;
  return __oe_board_temperature1;
}
static inline float canzero_get_board_temperature2() {
  extern float __oe_board_temperature2;
  return __oe_board_temperature2;
}
static inline float canzero_get_board_temperature3() {
  extern float __oe_board_temperature3;
  return __oe_board_temperature3;
}
static inline float canzero_get_board_avg_temperature() {
  extern float __oe_board_avg_temperature;
  return __oe_board_avg_temperature;
}
static inline float canzero_get_board_max_temperature() {
  extern float __oe_board_max_temperature;
  return __oe_board_max_temperature;
}
static inline float canzero_get_board_min_temperature() {
  extern float __oe_board_min_temperature;
  return __oe_board_min_temperature;
}
static inline error_level_config canzero_get_error_level_config_board_temperature() {
  extern error_level_config __oe_error_level_config_board_temperature;
  return __oe_error_level_config_board_temperature;
}
static inline float canzero_get_mcu_temperature() {
  extern float __oe_mcu_temperature;
  return __oe_mcu_temperature;
}
static inline error_level_config canzero_get_error_level_config_mcu_temperature() {
  extern error_level_config __oe_error_level_config_mcu_temperature;
  return __oe_error_level_config_mcu_temperature;
}
static inline float canzero_get_lim_temperature1() {
  extern float __oe_lim_temperature1;
  return __oe_lim_temperature1;
}
static inline float canzero_get_lim_temperature2() {
  extern float __oe_lim_temperature2;
  return __oe_lim_temperature2;
}
static inline float canzero_get_lim_temperature3() {
  extern float __oe_lim_temperature3;
  return __oe_lim_temperature3;
}
static inline float canzero_get_lim_temperature4() {
  extern float __oe_lim_temperature4;
  return __oe_lim_temperature4;
}
static inline float canzero_get_lim_average_temperature() {
  extern float __oe_lim_average_temperature;
  return __oe_lim_average_temperature;
}
static inline float canzero_get_lim_max_temperature() {
  extern float __oe_lim_max_temperature;
  return __oe_lim_max_temperature;
}
static inline float canzero_get_lim_min_temperature() {
  extern float __oe_lim_min_temperature;
  return __oe_lim_min_temperature;
}
static inline error_level_config canzero_get_error_level_config_lim_temperature() {
  extern error_level_config __oe_error_level_config_lim_temperature;
  return __oe_error_level_config_lim_temperature;
}
static inline float canzero_get_loop_frequency() {
  extern float __oe_loop_frequency;
  return __oe_loop_frequency;
}
static inline uint8_t canzero_get_last_node_missed() {
  extern uint8_t __oe_last_node_missed;
  return __oe_last_node_missed;
}
static inline bool_t canzero_get_ignore_45v() {
  extern bool_t __oe_ignore_45v;
  return __oe_ignore_45v;
}
typedef struct {
  get_resp_header m_header;
  uint32_t m_data;
} canzero_message_get_resp;
static const uint32_t canzero_message_get_resp_id = 0x1BD;
typedef struct {
  set_resp_header m_header;
} canzero_message_set_resp;
static const uint32_t canzero_message_set_resp_id = 0x1DD;
typedef struct {
  motor_state m_state;
  sdc_status m_sdc_status;
  motor_command m_command;
  bool_t m_control_active;
  sdc_status m_precharge_status;
  sdc_status m_feedthrough_status;
} canzero_message_motor_driver_stream_state;
static const uint32_t canzero_message_motor_driver_stream_state_id = 0xB6;
typedef struct {
  uint64_t m_config_hash;
} canzero_message_motor_driver_stream_config_hash;
static const uint32_t canzero_message_motor_driver_stream_config_hash_id = 0x76;
typedef struct {
  error_flag m_assertion_fault;
  error_flag m_error_arming_failed;
  error_flag m_error_precharge_failed;
  error_flag m_error_acceleration_out_of_range;
  error_flag m_error_acceleration_calibration_failed;
  error_flag m_error_invalid_target_acceleration;
  error_flag m_error_vdc_voltage_invalid;
  error_flag m_error_board_temperature1_invalid;
  error_flag m_error_board_temperature2_invalid;
  error_flag m_error_board_temperature3_invalid;
  error_flag m_error_mcu_temperature_invalid;
  error_flag m_error_lim_temperature1_invalid;
  error_flag m_error_lim_temperature2_invalid;
  error_flag m_error_lim_temperature3_invalid;
  error_flag m_error_lim_temperature4_invalid;
  error_flag m_error_heartbeat_miss;
  error_flag m_error_phase_current_unexpected;
  error_flag m_error_sdc_brake;
  error_level m_error_level_current_u1;
  error_level m_error_level_current_v1;
  error_level m_error_level_current_w1;
  error_level m_error_level_current_u2;
  error_level m_error_level_current_v2;
  error_level m_error_level_current_w2;
  error_level m_error_level_vdc_voltage;
  error_level m_error_level_board_temperature;
  error_level m_error_level_mcu_temperature;
  error_level m_error_level_lim_temperature;
  error_flag m_phase_current_unexpected;
  uint8_t m_last_node_missed;
} canzero_message_motor_driver_stream_errors;
static const uint32_t canzero_message_motor_driver_stream_errors_id = 0x96;
typedef struct {
  float m_loop_frequency;
} canzero_message_motor_driver_stream_debug;
static const uint32_t canzero_message_motor_driver_stream_debug_id = 0x79;
typedef struct {
  float m_current_u1;
  float m_current_v1;
  float m_current_w1;
  float m_current_max;
} canzero_message_motor_driver_stream_currents_u1v1w1;
static const uint32_t canzero_message_motor_driver_stream_currents_u1v1w1_id = 0x59;
typedef struct {
  float m_current_u2;
  float m_current_v2;
  float m_current_w2;
  float m_current_average;
} canzero_message_motor_driver_stream_current_u2v2w2;
static const uint32_t canzero_message_motor_driver_stream_current_u2v2w2_id = 0x19A;
typedef struct {
  float m_frequency;
  float m_modulation_index;
  float m_acceleration;
  float m_vdc_voltage;
} canzero_message_motor_driver_stream_control_info;
static const uint32_t canzero_message_motor_driver_stream_control_info_id = 0x17A;
typedef struct {
  float m_board_temperature1;
  float m_board_temperature2;
  float m_board_temperature3;
  float m_board_avg_temperature;
  float m_board_max_temperature;
  float m_board_min_temperature;
  float m_mcu_temperature;
} canzero_message_motor_driver_stream_board_temperature;
static const uint32_t canzero_message_motor_driver_stream_board_temperature_id = 0x15A;
typedef struct {
  float m_lim_temperature1;
  float m_lim_temperature2;
  float m_lim_temperature3;
  float m_lim_temperature4;
  float m_lim_average_temperature;
  float m_lim_max_temperature;
  float m_lim_min_temperature;
} canzero_message_motor_driver_stream_lim_temperature;
static const uint32_t canzero_message_motor_driver_stream_lim_temperature_id = 0x99;
typedef struct {
  uint8_t m_node_id;
  uint8_t m_unregister;
  uint8_t m_ticks_next;
} canzero_message_heartbeat_can0;
static const uint32_t canzero_message_heartbeat_can0_id = 0x1F5;
typedef struct {
  uint8_t m_node_id;
  uint8_t m_unregister;
  uint8_t m_ticks_next;
} canzero_message_heartbeat_can1;
static const uint32_t canzero_message_heartbeat_can1_id = 0x1F4;
typedef struct {
  get_req_header m_header;
} canzero_message_get_req;
static const uint32_t canzero_message_get_req_id = 0x1BE;
typedef struct {
  set_req_header m_header;
  uint32_t m_data;
} canzero_message_set_req;
static const uint32_t canzero_message_set_req_id = 0x1DE;
typedef struct {
  float m_target_acceleration;
  motor_command m_motor_driver_command;
  bool_t m_pod_grounded;
} canzero_message_mother_board_stream_motor_command;
static const uint32_t canzero_message_mother_board_stream_motor_command_id = 0x4D;
typedef struct {
  bool_t m_ignore_45v;
} canzero_message_mother_board_stream_debug_settings;
static const uint32_t canzero_message_mother_board_stream_debug_settings_id = 0x52;
void canzero_can0_poll();
void canzero_can1_poll();
uint32_t canzero_update_continue(uint32_t delta_time);
void canzero_init();
void canzero_set_config_hash(uint64_t value);

static inline void canzero_set_build_time(date_time value){
  extern date_time __oe_build_time;
  __oe_build_time = value;
}

void canzero_set_state(motor_state value);

void canzero_set_sdc_status(sdc_status value);

void canzero_set_assertion_fault(error_flag value);

void canzero_set_error_arming_failed(error_flag value);

void canzero_set_error_precharge_failed(error_flag value);

void canzero_set_error_acceleration_out_of_range(error_flag value);

void canzero_set_error_acceleration_calibration_failed(error_flag value);

void canzero_set_error_invalid_target_acceleration(error_flag value);

void canzero_set_error_vdc_voltage_invalid(error_flag value);

void canzero_set_error_board_temperature1_invalid(error_flag value);

void canzero_set_error_board_temperature2_invalid(error_flag value);

void canzero_set_error_board_temperature3_invalid(error_flag value);

void canzero_set_error_mcu_temperature_invalid(error_flag value);

void canzero_set_error_lim_temperature1_invalid(error_flag value);

void canzero_set_error_lim_temperature2_invalid(error_flag value);

void canzero_set_error_lim_temperature3_invalid(error_flag value);

void canzero_set_error_lim_temperature4_invalid(error_flag value);

void canzero_set_error_heartbeat_miss(error_flag value);

void canzero_set_error_phase_current_unexpected(error_flag value);

void canzero_set_error_sdc_brake(error_flag value);

void canzero_set_error_level_current_u1(error_level value);

void canzero_set_error_level_current_v1(error_level value);

void canzero_set_error_level_current_w1(error_level value);

void canzero_set_error_level_current_u2(error_level value);

void canzero_set_error_level_current_v2(error_level value);

void canzero_set_error_level_current_w2(error_level value);

void canzero_set_error_level_vdc_voltage(error_level value);

void canzero_set_error_level_board_temperature(error_level value);

void canzero_set_error_level_mcu_temperature(error_level value);

void canzero_set_error_level_lim_temperature(error_level value);

void canzero_set_phase_current_unexpected(error_flag value);

void canzero_set_precharge_status(sdc_status value);

void canzero_set_feedthrough_status(sdc_status value);

void canzero_set_command(motor_command value);

void canzero_set_control_active(bool_t value);

static inline void canzero_set_current_u1(float value){
  extern float __oe_current_u1;
  __oe_current_u1 = value;
}

static inline void canzero_set_current_v1(float value){
  extern float __oe_current_v1;
  __oe_current_v1 = value;
}

static inline void canzero_set_current_w1(float value){
  extern float __oe_current_w1;
  __oe_current_w1 = value;
}

static inline void canzero_set_current_u2(float value){
  extern float __oe_current_u2;
  __oe_current_u2 = value;
}

static inline void canzero_set_current_v2(float value){
  extern float __oe_current_v2;
  __oe_current_v2 = value;
}

static inline void canzero_set_current_w2(float value){
  extern float __oe_current_w2;
  __oe_current_w2 = value;
}

static inline void canzero_set_current_max(float value){
  extern float __oe_current_max;
  __oe_current_max = value;
}

static inline void canzero_set_current_average(float value){
  extern float __oe_current_average;
  __oe_current_average = value;
}

static inline void canzero_set_error_level_config_current_phase(error_level_config value){
  extern error_level_config __oe_error_level_config_current_phase;
  __oe_error_level_config_current_phase = value;
}

static inline void canzero_set_frequency(float value){
  extern float __oe_frequency;
  __oe_frequency = value;
}

static inline void canzero_set_modulation_index(float value){
  extern float __oe_modulation_index;
  __oe_modulation_index = value;
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

static inline void canzero_set_target_acceleration(float value){
  extern float __oe_target_acceleration;
  __oe_target_acceleration = value;
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

static inline void canzero_set_vdc_voltage(float value){
  extern float __oe_vdc_voltage;
  __oe_vdc_voltage = value;
}

static inline void canzero_set_error_level_config_vdc_voltage(error_level_config value){
  extern error_level_config __oe_error_level_config_vdc_voltage;
  __oe_error_level_config_vdc_voltage = value;
}

static inline void canzero_set_board_temperature1(float value){
  extern float __oe_board_temperature1;
  __oe_board_temperature1 = value;
}

static inline void canzero_set_board_temperature2(float value){
  extern float __oe_board_temperature2;
  __oe_board_temperature2 = value;
}

static inline void canzero_set_board_temperature3(float value){
  extern float __oe_board_temperature3;
  __oe_board_temperature3 = value;
}

static inline void canzero_set_board_avg_temperature(float value){
  extern float __oe_board_avg_temperature;
  __oe_board_avg_temperature = value;
}

static inline void canzero_set_board_max_temperature(float value){
  extern float __oe_board_max_temperature;
  __oe_board_max_temperature = value;
}

static inline void canzero_set_board_min_temperature(float value){
  extern float __oe_board_min_temperature;
  __oe_board_min_temperature = value;
}

static inline void canzero_set_error_level_config_board_temperature(error_level_config value){
  extern error_level_config __oe_error_level_config_board_temperature;
  __oe_error_level_config_board_temperature = value;
}

static inline void canzero_set_mcu_temperature(float value){
  extern float __oe_mcu_temperature;
  __oe_mcu_temperature = value;
}

static inline void canzero_set_error_level_config_mcu_temperature(error_level_config value){
  extern error_level_config __oe_error_level_config_mcu_temperature;
  __oe_error_level_config_mcu_temperature = value;
}

static inline void canzero_set_lim_temperature1(float value){
  extern float __oe_lim_temperature1;
  __oe_lim_temperature1 = value;
}

static inline void canzero_set_lim_temperature2(float value){
  extern float __oe_lim_temperature2;
  __oe_lim_temperature2 = value;
}

static inline void canzero_set_lim_temperature3(float value){
  extern float __oe_lim_temperature3;
  __oe_lim_temperature3 = value;
}

static inline void canzero_set_lim_temperature4(float value){
  extern float __oe_lim_temperature4;
  __oe_lim_temperature4 = value;
}

static inline void canzero_set_lim_average_temperature(float value){
  extern float __oe_lim_average_temperature;
  __oe_lim_average_temperature = value;
}

static inline void canzero_set_lim_max_temperature(float value){
  extern float __oe_lim_max_temperature;
  __oe_lim_max_temperature = value;
}

static inline void canzero_set_lim_min_temperature(float value){
  extern float __oe_lim_min_temperature;
  __oe_lim_min_temperature = value;
}

static inline void canzero_set_error_level_config_lim_temperature(error_level_config value){
  extern error_level_config __oe_error_level_config_lim_temperature;
  __oe_error_level_config_lim_temperature = value;
}

static inline void canzero_set_loop_frequency(float value){
  extern float __oe_loop_frequency;
  __oe_loop_frequency = value;
}

void canzero_set_last_node_missed(uint8_t value);

static inline void canzero_set_ignore_45v(bool_t value){
  extern bool_t __oe_ignore_45v;
  __oe_ignore_45v = value;
}

void canzero_send_config_hash();

void canzero_send_build_time();

void canzero_send_state();

void canzero_send_sdc_status();

void canzero_send_assertion_fault();

void canzero_send_error_arming_failed();

void canzero_send_error_precharge_failed();

void canzero_send_error_acceleration_out_of_range();

void canzero_send_error_acceleration_calibration_failed();

void canzero_send_error_invalid_target_acceleration();

void canzero_send_error_vdc_voltage_invalid();

void canzero_send_error_board_temperature1_invalid();

void canzero_send_error_board_temperature2_invalid();

void canzero_send_error_board_temperature3_invalid();

void canzero_send_error_mcu_temperature_invalid();

void canzero_send_error_lim_temperature1_invalid();

void canzero_send_error_lim_temperature2_invalid();

void canzero_send_error_lim_temperature3_invalid();

void canzero_send_error_lim_temperature4_invalid();

void canzero_send_error_heartbeat_miss();

void canzero_send_error_phase_current_unexpected();

void canzero_send_error_sdc_brake();

void canzero_send_error_level_current_u1();

void canzero_send_error_level_current_v1();

void canzero_send_error_level_current_w1();

void canzero_send_error_level_current_u2();

void canzero_send_error_level_current_v2();

void canzero_send_error_level_current_w2();

void canzero_send_error_level_vdc_voltage();

void canzero_send_error_level_board_temperature();

void canzero_send_error_level_mcu_temperature();

void canzero_send_error_level_lim_temperature();

void canzero_send_phase_current_unexpected();

void canzero_send_precharge_status();

void canzero_send_feedthrough_status();

void canzero_send_command();

void canzero_send_control_active();

void canzero_send_current_u1();

void canzero_send_current_v1();

void canzero_send_current_w1();

void canzero_send_current_u2();

void canzero_send_current_v2();

void canzero_send_current_w2();

void canzero_send_current_max();

void canzero_send_current_average();

void canzero_send_error_level_config_current_phase();

void canzero_send_frequency();

void canzero_send_modulation_index();

void canzero_send_acceleration();

void canzero_send_acceleration_calibration_variance();

void canzero_send_acceleration_calibration_offset();

void canzero_send_target_acceleration();

void canzero_send_external_acceleration();

void canzero_send_external_velocity();

void canzero_send_external_position();

void canzero_send_vdc_voltage();

void canzero_send_error_level_config_vdc_voltage();

void canzero_send_board_temperature1();

void canzero_send_board_temperature2();

void canzero_send_board_temperature3();

void canzero_send_board_avg_temperature();

void canzero_send_board_max_temperature();

void canzero_send_board_min_temperature();

void canzero_send_error_level_config_board_temperature();

void canzero_send_mcu_temperature();

void canzero_send_error_level_config_mcu_temperature();

void canzero_send_lim_temperature1();

void canzero_send_lim_temperature2();

void canzero_send_lim_temperature3();

void canzero_send_lim_temperature4();

void canzero_send_lim_average_temperature();

void canzero_send_lim_max_temperature();

void canzero_send_lim_min_temperature();

void canzero_send_error_level_config_lim_temperature();

void canzero_send_loop_frequency();

void canzero_send_last_node_missed();

void canzero_send_ignore_45v();

#endif