// in meters
#define WHEEL_DISTANCE_LR 0.132
#define WHEEL_CIRCUMFERENCE 0.2011

#define ENC_SLOT_COUNT 20

#define ARDUINO_PWM_RES 255

#define MOTOR_MAX_RPM 700

#define MOTOR_A_PWM 9
#define MOTOR_A_PINA 4
#define MOTOR_A_PINB 5

#define MOTOR_B_PWM 10
#define MOTOR_B_PINA 6
#define MOTOR_B_PINB 7

#define ENC_A_PIN 2
#define ENC_B_PIN 3

#define TICKS_PER_REV 40

struct Twist {
  union {
    float f;
    uint32_t u;
  } l_x;
  union {
    float f;
    uint32_t u;
  } a_z;
};

union rpm_but_sucks {
  float f;
  uint32_t u;
};

struct Encoder {
    uint16_t cur_pulses;
    float rpm;
};

struct Encoders {
  struct Encoder l;
  struct Encoder r;
};

struct Motor {
  int rpm;
  // uint8_t dir;
};

struct MotorDriver {
  Motor l;
  Motor r;
};
