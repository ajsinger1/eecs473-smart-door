#include "aws.h"
#include "ble.h"
#include "deadbolt.h"
#include "touch.h"
#include "state.h"
#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  delay(1000);
  deadbolt.init();
  // aws_init();
  // ble_init();
  // touchAttachInterrupt(TOUCH_PIN, handle_touch_isr, TOUCH_THRESHOLD);
  // xTaskCreatePinnedToCore(cap_unlock_handler, "t1", 16384, NULL, configMAX_PRIORITIES - 1, NULL, 1);
  // Serial.println("Finish setup");
  // vTaskDelete(NULL);
  // xTaskCreatePinnedToCore(ble_disconnect_task, "ble_disconnect_task", 5120, NULL, 1, NULL, 1);

 
  


}

void loop() {
  // Serial.println(String(touchRead(TOUCH_PIN)));
  // vTaskDelay(1000 / portTICK_PERIOD_MS);
  // // Serial.println("In ISR? " + String(xPortInIsrContext()));
  // state.isUnlockUserConnected();
  deadbolt.unlock();
  
  deadbolt.lock();

}
