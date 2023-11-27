#include "deadbolt.h"
#include <Arduino.h>

enum DeadboltState Deadbolt::get_state() {
  if (digitalRead(DEADBOLT_LOW_SWITCH) && digitalRead(DEADBOLT_HIGH_SWITCH)) {
    return DeadboltState::DEADBOLT_MIDDLE;
  } else if (digitalRead(DEADBOLT_LOW_SWITCH)) {
    return DeadboltState::DEADBOLT_UNLOCKED;
  } else {
    return DeadboltState::DEADBOLT_LOCKED;
  }
}

void Deadbolt::motor_forward() {
  digitalWrite(MOTOR_TERMINAL_1, HIGH);
  digitalWrite(MOTOR_TERMINAL_2, LOW);
}

void Deadbolt::motor_backward() {
  digitalWrite(MOTOR_TERMINAL_1, LOW);
  digitalWrite(MOTOR_TERMINAL_2, HIGH);
}

void Deadbolt::motor_stop() {
  digitalWrite(MOTOR_TERMINAL_1, HIGH);
  digitalWrite(MOTOR_TERMINAL_2, HIGH);
}

void Deadbolt::motor_coast() {
  digitalWrite(MOTOR_TERMINAL_1, LOW);
  digitalWrite(MOTOR_TERMINAL_2, LOW);
}

void Deadbolt::lock() {
  // taskDISABLE_INTERRUPTS();
  while (get_state() != DEADBOLT_LOCKED) {
    Serial.println("LOCKING (state is " + String(get_state()) + ")");
    motor_backward();
    delay(ON_DELAY);
    motor_coast();
    delay(OFF_DELAY);
  }
  motor_stop();
  // taskENABLE_INTERRUPTS();
}

void Deadbolt::unlock() {
  // taskDISABLE_INTERRUPTS();
  while (get_state() != DEADBOLT_UNLOCKED) {
    Serial.println("UNLOCKING (state is " + String(get_state()) + ")");
    motor_forward();
    delay(ON_DELAY);
    motor_coast();
    delay(OFF_DELAY);
  }
  motor_stop();
  // taskENABLE_INTERRUPTS();
}

void Deadbolt::init() {
  pinMode(DEADBOLT_HIGH_SWITCH, INPUT_PULLUP);
  pinMode(DEADBOLT_LOW_SWITCH, INPUT_PULLUP);
  pinMode(MOTOR_TERMINAL_1, OUTPUT);
  pinMode(MOTOR_TERMINAL_2, OUTPUT);
  unlock();
}
