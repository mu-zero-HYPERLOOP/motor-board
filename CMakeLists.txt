######################################################################
# @author      : karl (karlsasssie@gmail.com)
# @file        : CMakeLists
# @created     : Friday Apr 07, 2023 15:39:05 CEST
######################################################################

cmake_minimum_required(VERSION 2.8)
project(motor-board CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} 
  mock/main.cpp
  mock/can.cpp
  mock/sdc.cpp
  mock/pwm.cpp
  src/canzero/canzero.cpp
  src/fsm/control_state.cpp
  src/fsm/error_handling.cpp
  src/fsm/error_state.cpp
  src/fsm/fsm.cpp
  src/fsm/idle_state.cpp
  src/fsm/init_state.cpp
  src/fsm/manual_state.cpp
  src/fsm/precharge_state.cpp
  src/fsm/ready_state.cpp
  src/fsm/accelerate_state.cpp
  src/fsm/decelerate_state.cpp
)

target_include_directories(${PROJECT_NAME}
  PRIVATE src/)

