#include <ArduinoBLE.h>
#include <capacitive_sensor.h>
#include <Servo.h>
#include <mbed.h>

BLEService IdentificationService("1234");
BLEIntCharacteristic getIdentification("00002a05-0000-1000-8000-00805f9b34fb", BLERead | BLEWrite);

const int ledPin = LED_BUILTIN;
uint16_t value;

Servo myservo;

CapacitiveSensor cs_4_3 = CapacitiveSensor(4,3);

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


  BLE.setLocalName("Smart Handle");

  IdentificationService.addCharacteristic(getIdentification);
  BLE.addService(IdentificationService);

  getIdentification.setValue(0);

}

void loop() {
  BLEDevice peripheral = BLE.available();

  digitalWrite(LED_BUILTIN, LOW);

  while(!BLE.connected()) {
    AdvertiseInterval();
  }

  if (BLE.connected()) {
     digitalWrite(LED_BUILTIN, HIGH);
     Serial.println("Connected");

     readValue();
  }
}

void AdvertiseInterval() {
  BLE.advertise();
  delay(10);
  BLE.stopAdvertise();
  delay(490);
}

void readValue() {
  uint16_t prevVal = 0;
  while(BLE.connected()) {
    getIdentification.readValue(value);
    Serial.println(value);
    if(value == 52) {
      cap();
      turnServo();
      BLE.disconnect();
      Serial.println("Disconnected");
      value = 0;
      getIdentification.writeValue(value);
    }
  }
}

void cap() {
  while(cs_4_3.capacitiveSensor(30) < 6000);
}

void turnServo() {
  myservo.attach(2);
  delay(1000);
  myservo.write(180);
  delay(100);
  myservo.detach();
  delay(1000);
  myservo.attach(2);
  delay(1000);
  myservo.write(0);
  delay(100);
  myservo.detach();
}
