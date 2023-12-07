#ifndef _STATE_H_
#define _STATE_H_
#include "aws.h"
#include <ArduinoJson.h>

enum Mode { SECURE, PROXIMITY };
enum LockState { UNLOCKED, LOCKED };
enum UserState { UNLOCK, DO_NOT_UNLOCK, LOCKOUT };

// User object
struct User {
  bool valid = false;
  bool ble_connected = false;
  uint16_t conn_id = 0;
  UserState state = UserState::LOCKOUT;
  String uuid = "";
};

// Mutex associated with state access. State can only be read/written by one thread at a time for consistency
extern SemaphoreHandle_t state_mutex;

class State {
private:
  enum Mode mode = PROXIMITY;
  // Deadbolt init unlocks the door by default
  enum LockState lock = UNLOCKED;
  User users[8];
  void serialize(JsonObject&);
  void report(bool desired);

  // RAII Locking for state
  class StateLock {
    StateLock() {
      while (xSemaphoreTake(state_mutex, portMAX_DELAY) != pdTRUE)
        ;
    }
    ~StateLock() { xSemaphoreGive(state_mutex); }
  };

// All of these functions are thread safe:
public:
  State();
  enum Mode getMode();
  enum LockState getLock();
  // Updates the mode
  void setMode(enum Mode);
  // Updates the lock state (and moves the deadbolt if needed)
  void setLock(bool, enum LockState);
  // Finds the UUID in the user table and marks it as connected (also storing the BLE connection ID). Returns false iff the UUID doesn't exist in the table.
  bool bleConnected(const String&, uint16_t);
  // Finds the BLE connection ID in the user table and marks it as not connected.
  void bleDisconnected(uint16_t);
  // Returns true iff the BLE connection ID is authenticated
  bool isBleConnIdConnected(uint16_t);
  // Returns true iff there is a BLE connected user in the unlock state (criteria for unlocking upon cap touch)
  bool isUnlockUserConnected();
  // Updates the state of all connected BLE users to do not unlock (behavior that should happen after cap touch triggers unlock). Update is sent to cloud.
  void setConnectedUsersDoNotUnlock();
  // Updates the state from a JSON payload. This is what is called within the aws_callback to update the state/make stuff happen over cloud. 
  void updateFromJson(StaticJsonDocument<JSON_BUF_LEN>&);
};

// Global state object
extern State state;

#endif