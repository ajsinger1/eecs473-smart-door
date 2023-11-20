#include "aws.h"
#include "ble.h"
#include "driver/touch_pad.h"
#include "state.h"
#define CAP_FREQUENCY_MS 100
#define TOUCH_PIN                                                                                                      \
  TOUCH_PAD_NUM0 // GPIO 4 (see
                 // https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/touch_pad.html)
#define TOUCH_THRESHOLD 20
#define CAP_UNLOCK_USER_STATE_CHANGE_MS 15000

SemaphoreHandle_t cap_unlock_semaphore = xSemaphoreCreateBinary();

void handle_touch_isr(void* _) {
  Serial.println("Cap Sensor Touched");
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR(cap_unlock_semaphore, &xHigherPriorityTaskWoken);
}

void cap_unlock_handler(void* _) {
  while (1) {
    while (xSemaphoreTake(cap_unlock_semaphore, portMAX_DELAY) != pdTRUE)
      ;
    if (state.getMode() == Mode::SECURE)
      continue;
    if (state.isUnlockUserConnected()) {
      // TODO: Consider disabling interrupts here
      // TODO: Call Lock/Unlock Driver
      state.setLock(LockState::UNLOCKED);
      vTaskDelay(CAP_UNLOCK_USER_STATE_CHANGE_MS / portTICK_PERIOD_MS);
      state.setConnectedUsersDoNotUnlock();
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  aws_init();
  ble_init();
  touch_pad_init();
  touch_pad_config(TOUCH_PIN, TOUCH_THRESHOLD);
  touch_pad_isr_register(handle_touch_isr, NULL);
  xTaskCreatePinnedToCore(cap_unlock_handler, "cap_unlock_handler", 5120, NULL, configMAX_PRIORITIES - 1, NULL, 0);
  xTaskCreatePinnedToCore(ble_disconnect_task, "ble_disconnect_task", 5120, NULL, 1, NULL, 1);
}

void loop() {}