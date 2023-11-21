#include "aws.h"
#include "ble.h"
#include "driver/touch_pad.h"
#include "state.h"
#define CAP_FREQUENCY_MS 100
#define TOUCH_PIN                                                                                                      \
  TOUCH_PAD_NUM2 // GPIO 2 (see
                 // https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/touch_pad.html)
#define TOUCH_THRESHOLD 30
#define CAP_UNLOCK_USER_STATE_CHANGE_MS 5000

StackType_t stack[5120];
StaticTask_t xTaskBuffer;


SemaphoreHandle_t cap_unlock_semaphore = xSemaphoreCreateBinary();
State state;

void handle_touch_isr() {
  Serial.println("Cap Sensor Touched");
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR(cap_unlock_semaphore, &xHigherPriorityTaskWoken);
}

void cap_unlock_handler(void* _) {
  while (1) {
    while (xSemaphoreTake(cap_unlock_semaphore, portMAX_DELAY) != pdTRUE)
      ;
    touch_pad_intr_disable();
    Serial.println("Cap unlock handler unblocked");
    if (state.getMode() == Mode::PROXIMITY && state.isUnlockUserConnected()) {
      Serial.println("UNLOCKING");
      // TODO: Consider disabling interrupts here
      // TODO: Call Lock/Unlock Driver
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

void test(void* p) {
  while (1) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    // Serial.println("In ISR? " + String(xPortInIsrContext()));
    state.isUnlockUserConnected();
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  // aws_init();
  // ble_init();
  touchAttachInterrupt(TOUCH_PIN, handle_touch_isr, TOUCH_THRESHOLD);
  xTaskCreateStaticPinnedToCore(
      cap_unlock_handler, "cap_unlock_handler", 5120, NULL, configMAX_PRIORITIES - 1, stack, &xTaskBuffer, 1);
  // xTaskCreatePinnedToCore(test, "cap_unlock_handler", 80000, NULL, configMAX_PRIORITIES - 1, NULL, 1);
  Serial.println("Finish setup");
  // xTaskCreatePinnedToCore(ble_disconnect_task, "ble_disconnect_task", 5120, NULL, 1, NULL, 1);
}

void loop() {
  // Serial.println(String(touchRead(TOUCH_PIN)));
  // vTaskDelay(1000 / portTICK_PERIOD_MS);
  // // Serial.println("In ISR? " + String(xPortInIsrContext()));
  // state.isUnlockUserConnected();
}