#ifndef _DEADBOLT_H_
#define _DEADBOLT_H_
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
#include <Arduino.h>

#define MOTOR_TERMINAL_1 16
#define MOTOR_TERMINAL_2 4
#define DEADBOLT_HIGH_SWITCH 17
#define DEADBOLT_LOW_SWITCH 5
#define MOTOR_DUTY_CYCLE 60 // This number may need to be adjusted due to increased resistance upon attaching the deadbolt

// Taking this semaphore ensures that the deadbolt is in a constant state (i.e. it is not in motion)
extern SemaphoreHandle_t deadbolt_semaphore;

enum DeadboltState { DEADBOLT_LOCKED, DEADBOLT_UNLOCKED, DEADBOLT_MIDDLE };

// ISR handler for the higher switch (when this switch is low, deadbolt it locked)
void ISR_higher_switch();
// ISR handler for the lower switch (when this switch is low, deadbolt it unlockd)
void ISR_lower_switch();

// Moves the motor forward (unlocking direction)
void _motor_forward();

// Moves the motor backward (locking direction)
void _motor_backward();

// Stops the motor
void _motor_stop();

// Initializes the PWM configuration used by the driver/mcpwm.h and soc/mcpwm_periph.h. Registers motor terminals with the driver.
void deadbolt_init();

// Unlocks the deadbolt if it is not already unlocked
void deadbolt_unlock();

// Locks the deadbolt if it is not already locked
void deadbolt_lock();

// Gets the state of the deadbolt 
enum DeadboltState deadbolt_state();

#endif
