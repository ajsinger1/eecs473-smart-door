#include "ble.h"
#include "state.h"
#include "touch.h"
#include <NimBLEDevice.h>

// UUIDs for service and characteristic
#define SERVICE_UUID "614842ae-f98b-4692-9dcb-0b2339edabe1"
#define CHARACTERISTIC_UUID "afbfe7a0-b023-4674-9960-2532a6edd778"
#define TOUCH_UUID "touch7a0-b023-4674-9960-2532a6edd778"

NimBLEServer* pServer;

void SmartHandleServerCallbacks::onConnect(NimBLEServer* pServer, ble_gap_conn_desc* desc) {
  Serial.println("Device connected");
  pServer->startAdvertising();
}

void SmartHandleServerCallbacks::onDisconnect(NimBLEServer* pServer, ble_gap_conn_desc* desc) {
  Serial.println("Device disconnected");
  // Remove device from connected devices map
  state.bleDisconnected(desc->conn_handle);
  pServer->startAdvertising();
}

void UUIDCharacteristicCallbacks::onWrite(NimBLECharacteristic* pCharacteristic, ble_gap_conn_desc* desc) {
  String uuid = pCharacteristic->getValue();
  if (state.bleConnected(uuid, desc->conn_handle)) {
    Serial.println("UUID:" + uuid + " recognized and connected");
  } else {
    Serial.println("UUID:" + uuid + " not recognized. Disconnecting...");
    NimBLEDevice::getServer()->disconnect(desc->conn_handle);
  }
}


void TouchCharacteristicCallbacks::onRead(NimBLECharacteristic* pCharacteristic, ble_gap_conn_desc* desc) {
  pCharacteristic->setValue(touchRead(TOUCH_PIN));
}

void ble_init() {
  NimBLEDevice::init("handle");
  NimBLEDevice::setPower(ESP_PWR_LVL_P9);
  pServer = NimBLEDevice::createServer();
  pServer->setCallbacks(new SmartHandleServerCallbacks());
  NimBLEService* pService = pServer->createService(SERVICE_UUID);
  NimBLECharacteristic* pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID, NIMBLE_PROPERTY::WRITE);
  NimBLECharacteristic* touch_characteristic = pService->createCharacteristic(TOUCH_UUID, NIMBLE_PROPERTY::READ);
  pCharacteristic->setCallbacks(new UUIDCharacteristicCallbacks());
  touch_characteristic->setCallbacks(new TouchCharacteristicCallbacks());
  touch_characteristic->setValue(touchRead(TOUCH_PIN));

  pService->start();

  // Start advertising
  NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  // pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
  // pAdvertising->setMinPreferred(0x12);
  NimBLEDevice::startAdvertising();
  Serial.println("Advertising...");
  
  //TODO: Enable BLE Disconnection task
  // xTaskCreatePinnedToCore(ble_disconnect_task, "ble_disconnect_task", 5120, NULL, 1, NULL, 1);

}

void ble_disconnect_task(void* _) {
  for (uint16_t& conn_id : pServer->getPeerDevices()) {
    if (!state.isBleConnIdConnected(conn_id))
      pServer->disconnect(conn_id);
  }
  vTaskDelay(10000 / portTICK_PERIOD_MS);
}
