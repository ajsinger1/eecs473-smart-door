#ifndef _DEADBOLT_H_
#define _DEADBOLT_H_
#define MOTOR_TERMINAL_1 16
#define MOTOR_TERMINAL_2 4
#define DEADBOLT_LOW_SWITCH 15
#define DEADBOLT_HIGH_SWITCH 17
#define MOTOR_CYCLE_PERIOD_MS 100
#define MOTOR_DUTY_CYCLE 25
#define MOTOR_TIMEOUT 5000

enum DeadboltState { DEADBOLT_LOCKED, DEADBOLT_UNLOCKED, DEADBOLT_MIDDLE };

class Deadbolt {
private:
  static const int ON_DELAY = (int)(float(MOTOR_DUTY_CYCLE) / 100. * float(MOTOR_CYCLE_PERIOD_MS));
  static const int OFF_DELAY = (int)((1. - float(MOTOR_DUTY_CYCLE) / 100.) * float(MOTOR_CYCLE_PERIOD_MS));
  void motor_forward();
  void motor_backward();
  void motor_stop();
  void motor_coast();

public:
  void init();
  void unlock();
  void lock();
  enum DeadboltState get_state();
};

extern Deadbolt deadbolt;

#endif
