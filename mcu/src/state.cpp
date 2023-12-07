#include "state.h"
#include "String.h"
#include "aws.h"
#include "deadbolt.h"

SemaphoreHandle_t state_mutex = xSemaphoreCreateMutex();
State state;

State::State() {
    for (int i = 0; i < 8; i++) {
        this->users[i] = User();
    }
    xSemaphoreGive(state_mutex);
}

void State::serialize(JsonObject& root) {
    root["mode"] = this->mode;
    root["lock"] = this->lock;
    JsonArray userArr = root.createNestedArray("users");
    for (User& user : this->users) {
        if (user.valid) {
            JsonObject userObj = userArr.createNestedObject();
            userObj["uuid"] = user.uuid;
            userObj["state"] = user.state;
        }
    }
}

void State::report(bool desired) {
    StaticJsonDocument<JSON_BUF_LEN> doc;
    JsonObject root;
    root = doc["state"].createNestedObject("reported");
    serialize(root);
    if (desired) {
        root = doc["state"].createNestedObject("desired");
        serialize(root);
    }
    aws_report(doc);
}

enum Mode State::getMode() {
    StateLock state_lock();
    return this->mode;
}

enum LockState State::getLock() {
    StateLock state_lock();
    return this->lock;
}

void State::setMode(enum Mode new_mode) {
    StateLock state_lock();
    if (this->mode == new_mode)
        return;
    this->mode = new_mode;
    report(true);
}

void State::setLock(bool move, enum LockState new_lock) {
    StateLock state_lock();
    if (this->lock == new_lock)
        return;
    this->lock = new_lock;
    Serial.println("LOCK STATE UPDATED TO " + String(new_lock));
    if (move) {
        Serial.println("MOVING DEADBOLT");
        if (new_lock == LockState::LOCKED) {
            deadbolt_lock();
        } else {
            deadbolt_unlock();
        }
    }
    report(true);
}

bool State::bleConnected(const String& uuid, uint16_t conn_id) {
    StateLock state_lock();
    for (User& user : this->users) {
        if (!user.valid)
            continue;
        if (user.uuid == uuid) {
            Serial.println("User connected (conn_id " + String(conn_id) + ")");
            user.ble_connected = true;
            user.conn_id = conn_id;
            return true;
        }
    }
    return false;
}

void State::bleDisconnected(uint16_t conn_id) {
    StateLock state_lock();
    Serial.println("Looking for conn_id " + String(conn_id) + " to disconnect");
    for (User& user : this->users) {
        if (!user.valid || !user.ble_connected)
            continue;
        if (user.conn_id == conn_id) {
            user.ble_connected = false;
            Serial.println("UUID " + user.uuid + " disconnected");
            return;
        }
    }
}

bool State::isBleConnIdConnected(uint16_t conn_id) {
    StateLock state_lock();
    for (User& user : this->users) {
        if (!user.valid)
            continue;
        if (user.conn_id == conn_id && user.ble_connected) {
            return true;
        }
    }
    return false;
}

bool State::isUnlockUserConnected() {
    StateLock state_lock();
    for (User& user : this->users) {
        if (user.valid && user.ble_connected && user.state == UserState::UNLOCK) {
            return true;
        }
    }
    return false;
}

void State::setConnectedUsersDoNotUnlock() {
    StateLock state_lock();
    bool change = false;
    for (User& user : this->users) {
        if (user.valid && user.ble_connected && user.state == UserState::UNLOCK) {
            user.state = UserState::DO_NOT_UNLOCK;
            change = true;
        }
    }
    if (change)
        report(true);
}

void State::updateFromJson(StaticJsonDocument<JSON_BUF_LEN>& stateDoc) {
    StateLock state_lock();
    JsonObject jsonObj = stateDoc["state"].as<JsonObject>();
    if (jsonObj.containsKey("desired"))
        jsonObj = jsonObj["desired"].as<JsonObject>();

    if (jsonObj.containsKey("mode"))
        this->mode = jsonObj["mode"].as<Mode>();

    if (jsonObj.containsKey("lock")) {
        LockState new_lock = jsonObj["lock"].as<LockState>();
        this->lock = new_lock;
        if (new_lock == LockState::LOCKED)
            deadbolt_lock();
        else
            deadbolt_unlock();
    }

    if (jsonObj.containsKey("users")) {
        JsonArray userArr = jsonObj["users"].as<JsonArray>();
        int i = 0;
        for (JsonVariant user : userArr) {
            this->users[i].valid = false; // Not sure if we need this
            this->users[i].state = user["state"].as<UserState>();
            this->users[i].uuid = user["uuid"].as<String>();
            this->users[i].valid = true;
            i++;
        }

        for (; i < 8; i++) {
            this->users[i].valid = false;
        }
    }

    report(false);
}
