#include <ArduinoBLE.h>
#include <capacitive_sensor.h>
#include <Servo.h>
#include <mbed.h>

using namespace mbed;
using namespace rtos;
 
using namespace std::chrono_literals;

BLEService IdentificationService("1234");
BLEIntCharacteristic getIdentification("00002a05-0000-1000-8000-00805f9b34fb", BLERead | BLEWrite);

const int ledPin = LED_BUILTIN;
uint16_t value;

//EventQueue test;

Servo myservo;

CapacitiveSensor cs_4_3 = CapacitiveSensor(4,3);
 
Thread t1;
Thread t2;
 
void AdvertiseTask() {
  for(;;) {
    if(!BLE.connected()) {
      digitalWrite(5, 1);
      BLE.advertise();
      BLE.stopAdvertise();
      digitalWrite(5, 0);
    }
    ThisThread::sleep_for(500);
  }
}
 
void func2() {
  for(;;) {                         // Repeat forever
    ThisThread::sleep_for(250);      // Wait for 3 seconds
  }
}
 
void setup() {
  cs_4_3.set_CS_AutocaL_Millis(0xFFFFFFFF);
  Serial.begin(9600);

  myservo.attach(2);
  delay(1000);
  myservo.write(0);
  delay(100);
  myservo.detach();
  
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");
    while (1);
  }
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(5, OUTPUT);


  BLE.setLocalName("Smart Handle");

  IdentificationService.addCharacteristic(getIdentification);
  BLE.addService(IdentificationService);

  getIdentification.setValue(0);
 
  t1.start(AdvertiseTask);          // Pass func1 to t1 to start executing it as a independant thread
  t2.start(func2);                  // Pass func2 to t2 to start executing it as an independant thread
}
 
void loop() {                       // The loop function is executed in its own thread (left empty in this case)
}
