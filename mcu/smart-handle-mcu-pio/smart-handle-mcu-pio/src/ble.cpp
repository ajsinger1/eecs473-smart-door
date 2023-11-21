#include "ble.h"
#include "state.h"
#include <NimBLEDevice.h>

// UUIDs for service and characteristic
#define SERVICE_UUID "614842ae-f98b-4692-9dcb-0b2339edabe1"
#define CHARACTERISTIC_UUID "afbfe7a0-b023-4674-9960-2532a6edd778"

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

void ble_init() {
  NimBLEDevice::init("Smart Handle");
  NimBLEDevice::setPower(ESP_PWR_LVL_P9);
  pServer = NimBLEDevice::createServer();
  pServer->setCallbacks(new SmartHandleServerCallbacks());
  NimBLEService* pService = pServer->createService(SERVICE_UUID);
  NimBLECharacteristic* pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID, NIMBLE_PROPERTY::WRITE);
  pCharacteristic->setCallbacks(new UUIDCharacteristicCallbacks());

  pService->start();

  // Start advertising
  NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  // pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
  // pAdvertising->setMinPreferred(0x12);
  NimBLEDevice::startAdvertising();
  Serial.println("Advertising...");
}

void ble_disconnect_task(void* _) {
  for (uint16_t& conn_id : pServer->getPeerDevices()) {
    if (!state.isBleConnIdConnected(conn_id))
      pServer->disconnect(conn_id);
  }
  vTaskDelay(10000 / portTICK_PERIOD_MS);
}
