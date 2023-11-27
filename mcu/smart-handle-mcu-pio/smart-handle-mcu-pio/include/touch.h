#ifndef _TOUCH_H_
#define _TOUCH_H_
#define CAP_FREQUENCY_MS 100
#define TOUCH_PIN                                                                                                      \
  TOUCH_PAD_NUM2 // GPIO 2 (see
                 // https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/touch_pad.html)
#define TOUCH_THRESHOLD 30
#define CAP_UNLOCK_USER_STATE_CHANGE_MS 5000

void touch_isr();
void touch_handler(void*);
void touch_init();

#endif