// in meters
#define WHEEL_DISTANCE_LR   0.132
#define WHEEL_CIRCUMFERENCE 0.2011

#define ENC_SLOT_COUNT  20

#define ARDUINO_PWM_RES 255

#define MOTOR_MAX_RPM   24

#define MOTOR_A_PWM     10
#define MOTOR_A_PINA    4
#define MOTOR_A_PINB    5

#define MOTOR_B_PWM     11
#define MOTOR_B_PINA    6
#define MOTOR_B_PINB    7

enum {CC, CCW};

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

struct Encoders {
  struct {
    uint16_t pulses;
    uint16_t cur_pulses;
    uint16_t last_pulses;
    int delta_pulses;
  } l;

  struct {
    uint16_t pulses;
    uint16_t cur_pulses;
    uint16_t last_pulses;
    int delta_pulses;
  } r;
};

struct Motor {
  int rpm;
  uint8_t  dir = CC;
};

struct MotorDriver {
  Motor l;
  Motor r;
};