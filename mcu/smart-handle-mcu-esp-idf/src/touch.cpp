#include "touch.h"
#include "driver/touch_pad.h"
#include "state.h"
#include <Arduino.h>

SemaphoreHandle_t cap_unlock_semaphore = xSemaphoreCreateBinary();

void touch_isr() {
  Serial.println("Cap Sensor Touched");
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR(cap_unlock_semaphore, &xHigherPriorityTaskWoken);
  touch_pad_intr_disable();
}

void touch_handler(void* _) {
  while (1) {
    while (xSemaphoreTake(cap_unlock_semaphore, portMAX_DELAY) != pdTRUE)
      ;
    Serial.println("Cap unlock handler unblocked");
    if (state.getMode() == Mode::PROXIMITY && state.isUnlockUserConnected()) {
      Serial.println("UNLOCKING");
      state.setLock(LockState::UNLOCKED);
      vTaskDelay(CAP_UNLOCK_USER_STATE_CHANGE_MS / portTICK_PERIOD_MS);
      state.setConnectedUsersDoNotUnlock();
    } else {
      Serial.println("NOT UNLOCKING");
      vTaskDelay(CAP_UNLOCK_USER_STATE_CHANGE_MS / portTICK_PERIOD_MS);
    }
    touch_pad_intr_enable();
  }
}

void touch_init() {
  touchAttachInterrupt(TOUCH_PIN, touch_isr, TOUCH_THRESHOLD);
  xTaskCreatePinnedToCore(touch_handler, "t1", 4096*2, NULL, configMAX_PRIORITIES - 1, NULL, 1);
}