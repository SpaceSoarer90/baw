#include <TimerOne.h>
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

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing...");

  pinMode(MOTOR_A_PINA, OUTPUT);
  pinMode(MOTOR_A_PINB, OUTPUT);

  digitalWrite(MOTOR_A_PINA, LOW);
  digitalWrite(MOTOR_A_PINB, LOW);
}

void loop() {
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
  Twist rec;
  if (serial_buffer.pop(&rec)) {
    Serial.print(rec.l_x.f);
    Serial.print(",");
    Serial.println(rec.a_z.f);

    mot = twist_to_rpm(rec.l_x.f, msg.a_z.f);
    motor_change_state(mot);
  }
}
