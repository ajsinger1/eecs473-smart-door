#ifndef _TOUCH_H_
#define _TOUCH_H_
#define CAP_FREQUENCY_MS 100
#define TOUCH_PIN                                                                                                      \
  TOUCH_PAD_NUM2 // GPIO 2 (see
                 // https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/touch_pad.html)
#define TOUCH_THRESHOLD 5
#define CAP_UNLOCK_USER_STATE_CHANGE_MS 5000
#include <Arduino.h>

// Semaphore for deferred capacative touch handling
extern SemaphoreHandle_t cap_unlock_semaphore;

// Gives the semaphore upon the touch value threshold falling below the threshold
void touch_isr();

// Deferred execution of the touch ISR. Checks if the criteria is met for the door to unlock (a user in the correct state must be BLE connected, and mode must be proximity)
void touch_handler(void*);

// Registers the touch_isr at the defined threshold. 
void touch_init();

// Calling this function can change the threshold to trigger the ISR (see ble.h)
void set_touch_threshold(touch_value_t);

#endif