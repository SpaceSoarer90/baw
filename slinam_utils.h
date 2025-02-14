#define DBG_PRINT(x) Serial.print(x)
#define DBG_PRINTLN(x) Serial.println(x)
#define PULSE_TO_RPM(delta_pulses) (delta_pulses * 10 / TICKS_PER_REV * 60)
