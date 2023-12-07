#ifndef _BLE_H_
#define _BLE_H_
#include <NimBLEDevice.h>

extern NimBLEServer* pServer;

// Server Callbacks. Updates the state of connected users upon disconnection.
class SmartHandleServerCallbacks : public BLEServerCallbacks {
  void onConnect(NimBLEServer*, ble_gap_conn_desc*);
  void onDisconnect(NimBLEServer*, ble_gap_conn_desc*);
};

// UUID Characteristic Callback. Handles user authentication and updates the state of connected users upon auth.
class UUIDCharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onWrite(NimBLECharacteristic*, ble_gap_conn_desc*);
};

// Touch Threshold Callback. Used for debugging. Reading the characteristic gets the value of the capacitive touch. Writing the characteristic updates the ISR threshold.
class TouchCharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onRead(NimBLECharacteristic*, ble_gap_conn_desc*);
  void onWrite(NimBLECharacteristic*, ble_gap_conn_desc*);
};

// Initializes the NimBLE library by setting up the service, characteristics  registering the callbacks. Starts a FreeRTOS BLE callback queue handler task.
void ble_init();

// Disconnects unwanted connections that have connected but have not authenticated themselves.
void ble_disconnect_task(void*);

#endif