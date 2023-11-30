#ifndef _STATE_H_
#define _STATE_H_
#include "aws.h"
#include <ArduinoJson.h>

enum Mode { SECURE, PROXIMITY };
enum LockState { UNLOCKED, LOCKED };
enum UserState { UNLOCK, DO_NOT_UNLOCK, LOCKOUT };

struct User {
  bool valid = false;
  bool ble_connected = false;
  uint16_t conn_id = 0;
  UserState state = UserState::LOCKOUT;
  String uuid = "";
};

// Mutex associated with state access
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

public:
  State();
  enum Mode getMode();
  enum LockState getLock();
  void setMode(enum Mode);
  void setLock(bool, enum LockState);
  bool bleConnected(const String&, uint16_t);
  void bleDisconnected(uint16_t);
  bool isBleConnIdConnected(uint16_t);
  bool isUnlockUserConnected();
  void setConnectedUsersDoNotUnlock();
  void updateFromJson(StaticJsonDocument<JSON_BUF_LEN>&);
};

extern State state;

#endif