/*
This file provides a basic arduino sketch to use for BLE connection testing with the mobile app
*/
#include "ArduinoBLE.h"

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  BLE.begin();
  BLE.setDeviceName("Smart Handle");
  BLE.setLocalName("Smart Handle");
  BLE.advertise();
}

void loop() { digitalWrite(LED_BUILTIN, BLE.connected()); }
