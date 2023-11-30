#include "aws.h"
#include "ble.h"
#include "deadbolt.h"
#include "state.h"
#include "touch.h"
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

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
  Serial.begin(115200);
  Serial.println("****INITIAL BOOT****");
  deadbolt_init();
  aws_init();
  ble_init();
  touch_init();
  xTaskCreatePinnedToCore(monitor_lock, "t2", 4096 * 2, NULL, 2, NULL, 1);
  Serial.println("Finish setup");
}

extern "C" void app_main() {
  // initialize arduino library before we start the tasks
  initArduino();
  setup();
}
