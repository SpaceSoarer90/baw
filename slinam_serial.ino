#include <SoftTimers.h>
#include <SerialCAN.h>
#include <cppQueue.h>

#include "slinam.h"

float average = 0;
int average_cnt = 1;
float rps = 0;
uint32_t last_time;

Encoders enc = { 0 };
MotorDriver mot;

char dbg_out[128];

cppQueue serial_buffer(sizeof(Twist), 128);
SoftTimer timer;

serial_can::SerialCAN serialCan{&Serial};
serial_can::Frame can_frame{0X00, 8, serial_can::Frame::no_crc};
uint64_t timestamp = 0;

	
void setup() {
  // Serial.begin(115200);
  serialCan.begin(115200);

  pinMode(MOTOR_A_PINA, OUTPUT);
  pinMode(MOTOR_A_PINB, OUTPUT);

  pinMode(MOTOR_B_PINA, OUTPUT);
  pinMode(MOTOR_B_PINB, OUTPUT);

  pinMode(ENC_A_PIN, INPUT);
  pinMode(ENC_B_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(ENC_A_PIN), encoder_left_interrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_B_PIN), encoder_right_interrupt, CHANGE);
  
  digitalWrite(MOTOR_A_PINA, LOW);
  digitalWrite(MOTOR_A_PINB, LOW);
  digitalWrite(MOTOR_B_PINA, LOW);
  digitalWrite(MOTOR_B_PINB, LOW);

  timer.setTimeOutTime(100);
  timer.reset();

  // 100 ms
  // Timer1.initialize(100000);
  // Timer1.attachInterrupt(encoder_calc_speed_interrupt);
}

void loop() {
  timestamp += 1;
  // only start reading when there are two floats (4 bytes)
  // in the serial buffer
  Twist msg = { 0 };
  if (Serial.available() >= 8) {
    // Initialize to zero (because the or-ing doesn't remove the bits)
    for (int i = 0; i < sizeof(float); i++) {
      uint32_t data = (uint32_t)Serial.read() << 8 * i;
      msg.l_x.u |= data;
    }
    for (int i = 0; i < sizeof(float); i++) {
      uint32_t data = (uint32_t)Serial.read() << 8 * i;
      msg.a_z.u |= data;
    }

    serial_buffer.push(&msg);
  }

  // change pwm when twist messages are received (velocity has changed)
  static Twist dbg_rec;
  Twist rec = { 0 };
  if (serial_buffer.pop(&rec)) {
    dbg_rec = rec;
    mot = twist_to_rpm(rec.l_x.f, rec.a_z.f);
    motor_change_state(mot);
  }

  if (timer.hasTimedOut()) {
    encoder_calc_speed_interrupt();
    timer.reset();
  }

  // static unsigned long dbg_millis;
  // #define PLOTTER_DELAY 100
  // if (millis() - dbg_millis >= PLOTTER_DELAY) {
  //   byte l_pwm, r_pwm = 0;
  //   // l_pwm = abs((float(mot.l.rpm) / MOTOR_MAX_RPM) * 255);
  //   // r_pwm = abs((float(mot.r.rpm) / MOTOR_MAX_RPM) * 255);
  //   l_pwm = rpm_to_pwm(mot.l.rpm);
  //   r_pwm = rpm_to_pwm(mot.r.rpm);
  //   // l_x, a_z, l_rpm, r_rpm, l_pwm, r_pwm;
  //   // sprintf(dbg_out, "%s,%s,%d,%d,%d,%d,%d",
  //   //         String(rec.l_x.f).c_str(), String(rec.a_z.f).c_str(),
  //   //         mot.l.rpm, mot.r.rpm, (int)l_pwm, (int)r_pwm);
  //   Serial.print(dbg_rec.l_x.f);
  //   Serial.print(" ");
  //   Serial.print(dbg_rec.a_z.f);
  //   Serial.print(" ");
  //   Serial.print(mot.l.rpm);
  //   Serial.print(" ");
  //   Serial.print(mot.r.rpm);
  //   Serial.print(" ");
  //   Serial.print(l_pwm, DEC);
  //   Serial.print(" ");
  //   Serial.print(r_pwm, DEC);
  //   Serial.println(); // end plotting
  //   dbg_millis = millis();
  // }

  
  // byte l_pwm, r_pwm = 0;
  // l_pwm = abs((float(mot.l.rpm) / MOTOR_MAX_RPM) * 255);
  // r_pwm = abs((float(mot.r.rpm) / MOTOR_MAX_RPM) * 255);
  // // l_x, a_z, l_rpm, r_rpm, l_pwm, r_pwm;
  // // sprintf(dbg_out, "%s,%s,%d,%d,%d,%d,%d",
  // //         String(rec.l_x.f).c_str(), String(rec.a_z.f).c_str(),
  // //         mot.l.rpm, mot.r.rpm, (int)l_pwm, (int)r_pwm);
  // Serial.print(rec.l_x.f);
  // Serial.print(" ");
  // Serial.print(rec.a_z.f);
  // Serial.print(" ");
  // Serial.print(mot.l.rpm);
  // Serial.print(" ");
  // Serial.print(mot.r.rpm);
  // Serial.print(" ");
  // Serial.print(l_pwm, DEC);
  // Serial.print(" ");
  // Serial.print(r_pwm, DEC);
  // Serial.println(); // end plotting
}
