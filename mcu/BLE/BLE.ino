#include <ArduinoBLE.h>
//#include <Arduino_FreeRTOS.h>
//#include <semphr.h>

BLEService IdentificationService("1234");
BLEIntCharacteristic getIdentification("00002a05-0000-1000-8000-00805f9b34fb", BLERead | BLEWrite);

//void AdvertiseTask( void *pvParameters);

const int ledPin = LED_BUILTIN;
uint16_t value;

void setup() {

  Serial.begin(9600);
  
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

void AdvertiseInterval() {
  BLE.advertise();
  delay(10);
  BLE.stopAdvertise();
  delay(490);
}

void readValue() {
  while(BLE.connected()) {
    getIdentification.readValue(value);
    Serial.println(value);
    if(value == 52) {
      BLE.disconnect();
      Serial.println("Disconnected");
      value = 0;
      getIdentification.writeValue(value);
    }
  }
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
