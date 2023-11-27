#include "deadbolt.h"
#include <Arduino.h>
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"

bool unlocking = false;
bool locking = false;





enum DeadboltState deadbolt_state() {
  if (digitalRead(DEADBOLT_LOW_SWITCH) && digitalRead(DEADBOLT_HIGH_SWITCH)) {
    return DeadboltState::DEADBOLT_MIDDLE;
  } else if (digitalRead(DEADBOLT_LOW_SWITCH)) {
    return DeadboltState::DEADBOLT_UNLOCKED;
  } else {
    return DeadboltState::DEADBOLT_LOCKED;
  }
}

void Deadbolt::motor_forward() {

  mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B);
  mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 25.0);
  
}

void Deadbolt::motor_backward() {

  mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A);
  mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, 25.0);

}

void Deadbolt::motor_stop() {
  mcpwm_set_signal_high(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B);
  mcpwm_set_signal_high(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A);
}

void Deadbolt::motor_coast() {
  mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A);
  mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B);
}

void deadbolt_lock() {
  // taskDISABLE_INTERRUPTS();

  locking = true;
  detachInterrupt(digitalPinToInterrupt(DEADBOLT_HIGH_SWITCH));
  attachInterrupt(digitalPinToInterrupt(DEADBOLT_LOW_SWITCH), ISR_lower_switch, FALLING);
  motor_backward();
  
  // int start_time = millis();

  // while (get_state() != DEADBOLT_LOCKED && (millis() - start_time < MOTOR_TIMEOUT)){
  //   Serial.println("LOCKING (state is " + String(get_state()) + "), elapsed time: " + String(millis() - start_time));
  //   motor_backward();
    
  // }
  // motor_stop();
  //taskENABLE_INTERRUPTS();
}

void Deadbolt::unlock() {

  unlocking = true;
  detachInterrupt(digitalPinToInterrupt(DEADBOLT_LOW_SWITCH));
  attachInterrupt(digitalPinToInterrupt(DEADBOLT_HIGH_SWITCH), ISR_lower_switch, FALLING);
  motor_forward();

  // taskDISABLE_INTERRUPTS();
  // int start_time = millis();
  // while (get_state() != DEADBOLT_UNLOCKED && (millis() - start_time < MOTOR_TIMEOUT)) {
  //   Serial.println("UNLOCKING (state is " + String(get_state()) + "), elapsed time: " + String(millis() - start_time));
  //   motor_forward();
    
  // }
  // motor_stop();
  // taskENABLE_INTERRUPTS();
}


void ISR_higher_switch(){ //MIGHT HAVE TO SWITCH THE INTERRUPTS
  if(unlocking){
    motor_stop();
    unlocking = false;
  }
}


void ISR_lower_switch(){
  if(locking){
    motor_stop();
    locking = false;
  } 
}


void Deadbolt::init() {
  
  
  //interrupt init

  attachInterrupt(digitalPinToInterrupt(DEADBOLT_HIGH_SWITCH), ISR_higher_switch, FALLING);
  attachInterrupt(digitalPinToInterrupt(DEADBOLT_LOW_SWITCH), ISR_lower_switch, FALLING);

  //PWM INIT
  Serial.print("deadbolt init called\n");


  mcpwm_config_t pwm_config;

  pwm_config.frequency = 1000;    //frequency = 500Hz,
  pwm_config.cmpr_a = 0;    //duty cycle of PWMxA = 0
  pwm_config.cmpr_b = 0;    //duty cycle of PWMxb = 0
  pwm_config.counter_mode = MCPWM_UP_COUNTER;
  pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);
  
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, MOTOR_TERMINAL_1);
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, MOTOR_TERMINAL_2);


  
  
}
