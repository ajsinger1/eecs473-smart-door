#include "deadbolt.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
#include <Arduino.h>

SemaphoreHandle_t deadbolt_semaphore = xSemaphoreCreateBinary();

bool unlocking = false;
bool locking = false;

enum DeadboltState deadbolt_state() {
  if (digitalRead(DEADBOLT_LOW_SWITCH) && digitalRead(DEADBOLT_HIGH_SWITCH)) {
    return DeadboltState::DEADBOLT_MIDDLE;
  } else if (digitalRead(DEADBOLT_HIGH_SWITCH)) {
    return DeadboltState::DEADBOLT_UNLOCKED;
  } else {
    return DeadboltState::DEADBOLT_LOCKED;
  }
}

void _motor_forward() {
  mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B);
  mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, MOTOR_DUTY_CYCLE);
  mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
}

void _motor_backward() {
  mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A);
  mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, MOTOR_DUTY_CYCLE);
  mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, MCPWM_DUTY_MODE_0);
}

void _motor_stop() {
  mcpwm_set_signal_high(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B);
  mcpwm_set_signal_high(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A);
}

void deadbolt_unlock() {
   xSemaphoreTake(deadbolt_semaphore, portMAX_DELAY);
  if (deadbolt_state() == DeadboltState::DEADBOLT_UNLOCKED) {
    Serial.println("DEADBOLT ALREADY UNLOCKED");
    xSemaphoreGive(deadbolt_semaphore);
    return;
  }
  Serial.println("UNLOCKING NOW");
  // unlocking = true;
  // locking = false;
  detachInterrupt(digitalPinToInterrupt(DEADBOLT_HIGH_SWITCH));
  attachInterrupt(digitalPinToInterrupt(DEADBOLT_LOW_SWITCH), ISR_lower_switch, FALLING);
  _motor_backward();
}

void deadbolt_lock() {
   xSemaphoreTake(deadbolt_semaphore, portMAX_DELAY);
  if (deadbolt_state() == DeadboltState::DEADBOLT_LOCKED) {
    Serial.println("DEADBOLT ALREADY LOCKED");
     xSemaphoreGive(deadbolt_semaphore);
    return;
  }
  Serial.println("LOCKING NOW");
  // unlocking = false;
  // locking = true;
  detachInterrupt(digitalPinToInterrupt(DEADBOLT_LOW_SWITCH));
  attachInterrupt(digitalPinToInterrupt(DEADBOLT_HIGH_SWITCH), ISR_higher_switch, FALLING);
  _motor_forward();
}

void ISR_higher_switch() { // MIGHT HAVE TO SWITCH THE INTERRUPTS
  // if (locking) {
  //   _motor_stop();
  //   locking = false;
  //   Serial.println("ISR HIGHER");
  // }
  _motor_stop();
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR(deadbolt_semaphore, &xHigherPriorityTaskWoken);
}

void ISR_lower_switch() {
  // if (unlocking) {
  //   _motor_stop();
  //   unlocking = false;
  //   Serial.println("ISR LOWER");
  // }
  _motor_stop();
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR(deadbolt_semaphore, &xHigherPriorityTaskWoken);
}

void deadbolt_init() {
  mcpwm_config_t pwm_config;

  pwm_config.frequency = 1000; // frequency = 500Hz,
  pwm_config.cmpr_a = 0;       // duty cycle of PWMxA = 0
  pwm_config.cmpr_b = 0;       // duty cycle of PWMxb = 0
  pwm_config.counter_mode = MCPWM_UP_COUNTER;
  pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, MOTOR_TERMINAL_1);
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, MOTOR_TERMINAL_2);
  //to get rid of resistors
  pinMode(DEADBOLT_HIGH_SWITCH, INPUT_PULLUP);
  pinMode(DEADBOLT_LOW_SWITCH, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(DEADBOLT_HIGH_SWITCH), ISR_higher_switch, FALLING);
  attachInterrupt(digitalPinToInterrupt(DEADBOLT_LOW_SWITCH), ISR_lower_switch, FALLING);

  xSemaphoreGive(deadbolt_semaphore);
}
