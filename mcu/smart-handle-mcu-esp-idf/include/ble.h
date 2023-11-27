#ifndef _BLE_H_
#define _BLE_H_
#include <NimBLEDevice.h>

extern NimBLEServer* pServer;

// Server Callbacks
class SmartHandleServerCallbacks : public BLEServerCallbacks {
  void onConnect(NimBLEServer*, ble_gap_conn_desc*);
  void onDisconnect(NimBLEServer*, ble_gap_conn_desc*);
};

// UUID Characteristic Callback
class UUIDCharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onWrite(NimBLECharacteristic*, ble_gap_conn_desc*);
};

class TouchCharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onRead(NimBLECharacteristic*, ble_gap_conn_desc*);
};

void ble_init();
void ble_disconnect_task(void*);

#endif