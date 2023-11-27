#include "aws.h"
#include "ble.h"
#include "deadbolt.h"
#include "state.h"
#include "touch.h"
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#define LED_PIN 32

// void touch_test() {
//   Serial.println("Beginning touch test");
//   while(1) {
//     touch_value_t val = touchRead(TOUCH_PIN);
//     Serial.println("Touch val: " + String(val));
//     delay(100);
//   }
// }

// void motor_test() {
//   while (1) {
//     deadbolt_unlock();
//     while (deadbolt_state() != DeadboltState::DEADBOLT_UNLOCKED) {
//       vTaskDelay(1000 / portTICK_PERIOD_MS);
//     }
//     Serial.println("Waiting 5 seconds before locking");
//     delay(5000);
//     deadbolt_lock();
//     while (deadbolt_state() != DeadboltState::DEADBOLT_LOCKED) {
//       vTaskDelay(1000 / portTICK_PERIOD_MS);
//     }
//     Serial.println("Waiting 5 seconds before unlocking");
//     delay(5000);
//   }
// }

void monitor_lock(void* _) {
  DeadboltState last_state = deadbolt_state();
  while (1) {
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    DeadboltState current_state = deadbolt_state();
    if (current_state != last_state && current_state != DeadboltState::DEADBOLT_MIDDLE) {
      last_state = current_state;
      state.setLock((current_state == DeadboltState::DEADBOLT_LOCKED) ? LockState::LOCKED : LockState::UNLOCKED);
    }
  }
}

void setup() {
  // pinMode(LED_PIN, OUTPUT);
  // digitalWrite(LED_PIN, HIGH);
  Serial.begin(115200);
  Serial.println("****INITIAL BOOT****");
  deadbolt_init();
  aws_init();
  // TODO: Enable BLE Disconnection task
  ble_init();
  touch_init();
  xTaskCreatePinnedToCore(monitor_lock, "t2", 4096 * 2, NULL, 2, NULL, 1);
  // motor_test();
  // touch_test();
  Serial.println("Finish setup");
  // digitalWrite(LED_PIN, LOW);
}

extern "C" void app_main() {
  // initialize arduino library before we start the tasks
  initArduino();
  setup();
}
