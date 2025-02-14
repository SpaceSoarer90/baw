#include "slinam_utils.h"

void encoder_left_interrupt() {
  enc.l.cur_pulses++;
}

void encoder_right_interrupt() {
  enc.r.cur_pulses++;
}

// returns the change of pulses since last check
// runs every 10ms
//
// funny story: we used to do delta_pulses here (current - last),
// but we realized that we don't actually need it in this case.
// if you're resetting the current pulses to 0, you're already getting
// the speed. (current pulses / time). so we kinda just overthought
// the problem HAHAHAHA
void encoder_calc_speed_interrupt() {
  struct Encoder* l = &enc.l;
  struct Encoder* r = &enc.r;

  // calculate change in pulses for each encoder
  // int16_t l_delta_pulses = l.cur_pulses - l.last_pulses;
  // int16_t r_delta_pulses = r.cur_pulses - r.last_pulses;

  // (convert pulses to rpm), then change direction depending on motor rpm
  l->rpm = ((l->cur_pulses * 10.0 / TICKS_PER_REV) * 60.0) * ((mot.l.rpm > 0) ? 1 : -1);
  r->rpm = ((r->cur_pulses * 10.0 / TICKS_PER_REV) * 60.0) * ((mot.r.rpm > 0) ? -1 : 1);

    // reset cur pulses to 0
  l->cur_pulses = 0;
  r->cur_pulses = 0;

  union rpm_but_sucks left_rpm;
  union rpm_but_sucks right_rpm;
  left_rpm.f = l->rpm;
  right_rpm.f = r->rpm;
  
  static uint8_t buf[8];

  // XXX.
  for (uint8_t i = 0; i < 4; i++) {
    buf[i] = (left_rpm.u) >> (8 * i);
    buf[i+4] = (right_rpm.u) >> (8 * i);
    // r_buf[i] = (right_rpm.u) >> (8 * i);
  }

   // Serial.write(l_buf, 4);
  // // Serial.write(r_buf, 4);
  can_frame.encode<uint8_t>({
                              buf[0],
                              buf[1],
                              buf[2],
                              buf[3],
                              buf[4],
                              buf[5],
                              buf[6],
                              buf[7]
                            });
  // std::vector<uint8_t> hahaha(buf, buf + sizeof(buf)/sizeof(buf[0]));
  // can_frame.encode<uint8_t>(hahaha);
  serialCan.send(&can_frame, timestamp);
}

