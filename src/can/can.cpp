#include "can/can.hpp"
#include "FlexCAN_T4.h"
#include "canzero/canzero.h"
#include "util/timestamp.h"
#include <assert.h>
#include <inttypes.h>

void canzero_can0_setup(uint32_t baudrate, canzero_can_filter *filters,
                        int filter_count) {
  CanBeginInfo beginInfo;
  if (baudrate == CAN_BAUDRATE_125Kbps) {
    beginInfo.baudrate = CAN_BAUDRATE_125Kbps;
  } else if (baudrate == CAN_BAUDRATE_250Kbps) {
    beginInfo.baudrate = CAN_BAUDRATE_250Kbps;
  } else if (baudrate == CAN_BAUDRATE_500Kbps) {
    beginInfo.baudrate = CAN_BAUDRATE_500Kbps;
  } else if (baudrate == CAN_BAUDRATE_1000Kbps) {
    beginInfo.baudrate = CAN_BAUDRATE_1000Kbps;
  } else {
    assert(false);
  }
  beginInfo.loopback = false;
  if (filter_count > 0) {
    beginInfo.filters = new CanFilter[filter_count];
    beginInfo.filter_count = filter_count;
    for (int i = 0; i < filter_count; ++i) {
      beginInfo.filters[i].id = filters->id;
      beginInfo.filters[i].mask = filters->mask;
      beginInfo.filters[i].ide = false;
    }
  }else {
    beginInfo.filters = nullptr;
    beginInfo.filter_count = 0;
  }
  Can3::begin(beginInfo);

  delete[] beginInfo.filters;
}
void canzero_can0_send(canzero_frame *frame) {
  CAN_message_t msg;
  msg.id = frame->id & 0x1FFFFFFF;
  msg.len = frame->dlc;
  msg.flags.remote = false;
  msg.flags.extended = frame->id & CANZERO_FRAME_IDE_BIT;
  for (int i = 0; i < 8; i++) {
    msg.buf[i] = frame->data[i];
  }
  Can3::send(msg);
}
int canzero_can0_recv(canzero_frame *frame) {
  CAN_message_t msg;
  int rx = Can3::recv(msg);
  if (rx) {
    frame->id = msg.id | (msg.flags.extended ? CANZERO_FRAME_IDE_BIT : 0) |
                (msg.flags.remote ? CANZERO_FRAME_RTR_BIT : 0);
    frame->dlc = msg.len;
    for (int i = 0; i < 8; i++) {
      frame->data[i] = msg.buf[i];
    }
  }
  return rx;
}
void canzero_can1_setup(uint32_t baudrate, canzero_can_filter *filters,
                        int filter_count) {
  CanBeginInfo beginInfo;
  if (baudrate == CAN_BAUDRATE_125Kbps) {
    beginInfo.baudrate = CAN_BAUDRATE_125Kbps;
  } else if (baudrate == CAN_BAUDRATE_250Kbps) {
    beginInfo.baudrate = CAN_BAUDRATE_250Kbps;
  } else if (baudrate == CAN_BAUDRATE_500Kbps) {
    beginInfo.baudrate = CAN_BAUDRATE_500Kbps;
  } else if (baudrate == CAN_BAUDRATE_1000Kbps) {
    beginInfo.baudrate = CAN_BAUDRATE_1000Kbps;
  } else {
    assert(false);
  }
  beginInfo.loopback = false;
  if (filter_count > 0) {
    beginInfo.filters = new CanFilter[filter_count];
    beginInfo.filter_count = filter_count;
    for (int i = 0; i < filter_count; ++i) {
      beginInfo.filters[i].id = filters->id;
      beginInfo.filters[i].mask = filters->mask;
      beginInfo.filters[i].ide = false;
    }
  }else {
    beginInfo.filters = nullptr;
    beginInfo.filter_count = 0;
  }
  Can2::begin(beginInfo);

  delete[] beginInfo.filters;
}
void canzero_can1_send(canzero_frame *frame) {
  CAN_message_t msg;
  msg.id = frame->id & 0x1FFFFFFF;
  msg.len = frame->dlc;
  msg.flags.remote = false;
  msg.flags.extended = frame->id & CANZERO_FRAME_IDE_BIT;
  for (int i = 0; i < 8; i++) {
    msg.buf[i] = frame->data[i];
  }
  Can2::send(msg);
}
int canzero_can1_recv(canzero_frame *frame) {
  CAN_message_t msg;
  int rx = Can2::recv(msg);
  if (rx) {
    frame->id = msg.id | (msg.flags.extended ? CANZERO_FRAME_IDE_BIT : 0) |
                (msg.flags.remote ? CANZERO_FRAME_RTR_BIT : 0);
    frame->dlc = msg.len;
    for (int i = 0; i < 8; i++) {
      frame->data[i] = msg.buf[i];
    }
  }
  return rx;
}
void canzero_request_update(uint32_t time) {
  // pass
}

static Timestamp g_start = Timestamp::now();

uint32_t canzero_get_time() { return (Timestamp::now() - g_start).as_ms(); }
void canzero_enter_critical() {
  // pass
}
void canzero_exit_critical() {
  // pass
}
