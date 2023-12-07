#include "tests.h"
#include <Arduino.h>
#include "touch.h"
#include "deadbolt.h"

void touch_test() {
  Serial.println("Beginning touch test");
  while(1) {
    touch_value_t val = touchRead(TOUCH_PIN);
    Serial.println("Touch val: " + String(val));
    delay(100);
  }
}

void motor_test() {
  while (1) {
    deadbolt_unlock();
    while (deadbolt_state() != DeadboltState::DEADBOLT_UNLOCKED) {
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    Serial.println("Waiting 5 seconds before locking");
    delay(5000);
    deadbolt_lock();
    while (deadbolt_state() != DeadboltState::DEADBOLT_LOCKED) {
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    Serial.println("Waiting 5 seconds before unlocking");
    delay(5000);
  }
}