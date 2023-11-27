#ifndef _DEADBOLT_H_
#define _DEADBOLT_H_
#define MOTOR_TERMINAL_1 16
#define MOTOR_TERMINAL_2 4
#define DEADBOLT_HIGH_SWITCH 17
#define DEADBOLT_LOW_SWITCH 5
#define MOTOR_DUTY_CYCLE 60

enum DeadboltState { DEADBOLT_LOCKED, DEADBOLT_UNLOCKED, DEADBOLT_MIDDLE };

void ISR_higher_switch();
void ISR_lower_switch();

void _motor_forward();
void _motor_backward();
void _motor_stop();

void deadbolt_init();
void deadbolt_unlock();
void deadbolt_lock();
enum DeadboltState deadbolt_state();

#endif
