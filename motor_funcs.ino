MotorDriver twist_to_rpm(float l_x, float a_z) {
  static float l_x_min, a_z_min, tan_vel;
  static float x_rpm, tan_rpm;

  l_x_min = l_x * 60;
  a_z_min = a_z * 60;

  tan_vel = a_z_min * WHEEL_DISTANCE_LR;

  x_rpm = l_x_min / WHEEL_CIRCUMFERENCE;
  tan_rpm = tan_vel / WHEEL_CIRCUMFERENCE;

  MotorDriver m;

  m.l.rpm = x_rpm - tan_rpm;
  m.r.rpm = x_rpm + tan_rpm;

  return m;
}

// assumes 8-bit resolution for pwm
uint8_t rpm_to_pwm(int rpm) {
  // byte pwm = abs((float(rpm) / MOTOR_MAX_RPM) * 255);
  uint8_t pwm = map(abs(rpm), 0, MOTOR_MAX_RPM, 0, 255);
  return pwm;
}

void motor_change_state(MotorDriver md) {
  // enc.l.direction = (md.l.rpm > 0) ? 1 : -1;
  // enc.r.direction = (md.r.rpm > 0) ? 1 : -1;

  // Serial.println("rpms: ");
  // Serial.print(md.l.rpm);
  // Serial.print(" ");
  // Serial.println(md.r.rpm);
  // Serial.print("Setting the encoder directions: ");
  // Serial.print(enc.l.direction);
  // Serial.print(" ");
  // Serial.println(enc.r.direction);

  run_motors(md.l, MOTOR_A_PINA, MOTOR_A_PINB, MOTOR_A_PWM);
  // reversed the pins here para ok na yung movement niya
  // nakakatamad lol
  run_motors(md.r, MOTOR_B_PINB, MOTOR_B_PINA, MOTOR_B_PWM);
}

void run_motors(Motor m, uint8_t pinA, uint8_t pinB, uint8_t pinPWM) {

  /**
     pinA  | pinB 
       0   |   0    -- TURN OFF
       0   |   1    -- CLOCKWISE
       1   |   0    -- ANTI-CLOCKWISE
       1   |   1    -- SHORT BRAKE (unused)
  */
  // CLOCKWISE - (negative angles are clockwise)

  analogWrite(pinPWM, rpm_to_pwm(m.rpm));

  if (m.rpm < 0) {
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, HIGH);
    // ANTI-CLOCKWISE - (positive angles are anti-clockwise)
  } else if (m.rpm > 0) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, LOW);
  } else {
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, LOW);
  }
}
