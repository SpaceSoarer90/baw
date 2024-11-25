
enum { ENC_LEFT, ENC_RIGHT };

void encoder_left_interrupt() {
  enc.l.pulses++;
}

void encoder_right_interrupt() {
  enc.r.pulses++;
}

// returns the change of pulses since last check
// runs every 10ms
void encoder_delta_pulses() {
  Timer1.detachInterrupt();

  enc.l.cur_pulses = enc.l.pulses;
  enc.r.cur_pulses = enc.r.pulses;
  
  enc.l.delta_pulses = enc.l.cur_pulses - enc.l.last_pulses;
  enc.r.delta_pulses = enc.r.cur_pulses - enc.l.last_pulses;

  enc.l.last_pulses = enc.l.cur_pulses;
  enc.r.last_pulses = enc.r.cur_pulses;

  Timer1.attachInterrupt(encoder_delta_pulses);
}

