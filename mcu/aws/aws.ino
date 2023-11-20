#include <ArduinoJson.h>

#include "aws_iot_config.h"
#include <AWS_IOT.h>
#include <ArduinoJson.h> //Download latest ArduinoJson 6 Version library only by Benoît Blanchon from Manage Libraries
#include <WiFi.h>

AWS_IOT shadow;
#define ledPin 2 // LED Pin
#define UUID_LEN 36
#define JSON_BUF_LEN AWS_IOT_MQTT_RX_BUF_LEN

char buffer[JSON_BUF_LEN];
SemaphoreHandle_t jsonDocSemaphore = xSemaphoreCreateBinary();
String state_str = "Uninitialized State";
const char WIFI_SSID[] = "Ari’s iPhone";
const char WIFI_PASSWORD[] = "beefstik";
char HOST_ADDRESS[] = "av076j80qpn1a-ats.iot.us-east-2.amazonaws.com"; // AWS IoT Custom Endpoint Address
char CLIENT_ID[] = "smart-handle";
char SHADOW_GET[] = "$aws/things/smart-handle/shadow/get/accepted";
char SENT_GET[] = "$aws/things/smart-handle/shadow/get";
char SHADOW_UPDATE[] = "$aws/things/smart-handle/shadow/update";
char SHADOW_DELTA[] = "$aws/things/smart-handle/shadow/update/delta";
int status = WL_IDLE_STATUS;


struct User {
  bool valid = false;
  UserState state = LOCKOUT;
  String uuid = "";
};

class State {
private:
  void report();
  void toJsonDoc(StaticJsonDocument<JSON_BUF_LEN>&);

public:
  enum Mode mode = SECURE;
  enum LockState lock = LOCKED; // TODO: How do we initialize this? Need access to that sensor from driver.
  User users[8];
  void
  updateDeltaFromJson(StaticJsonDocument<JSON_BUF_LEN>&); // This function shoult take in an update/delta json string,
                                                          // adjust the values accordingly, and send a report
  void prettyPrint();
};

State state;

void callBackHandler(char* topicName, int payloadLen, char* payload) {
  Serial.println("CALLBACK");
  Serial.println("Recv Payload (" + String(topicName) + "): " + String(payload));
  Serial.println("Recv Payload len: " + String(payloadLen));
  StaticJsonDocument<JSON_BUF_LEN> doc; // The size of this can be reduced if needed later
  deserializeJson(doc, payload);
  state.updateDeltaFromJson(doc);
  // TODO: UNSUBSCRIBE FROM GET
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  delay(2000);
  status = WiFi.begin(
      WIFI_SSID, WIFI_PASSWORD); // Connect to WPA/WPA2 network. Change this line if using open or WEP network:

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected to wifi");

  if (shadow.connect(HOST_ADDRESS, CLIENT_ID) == 0) // Connect to AWS IoT COre
  {
    Serial.println("Connected to AWS");
    delay(1000);

    if (0 == shadow.subscribe(SHADOW_GET, callBackHandler)) // Subscribe to Accepted GET Shadow Service
    {
      Serial.println("Subscribe Successfull");
    } else {
      Serial.println("Subscribe Failed, Check the Thing Name and Certificates");
      while (1)
        ;
    }
    if (0 == shadow.subscribe(SHADOW_DELTA, callBackHandler)) // Subscribe to Accepted GET Shadow Service
    {
      Serial.println("Subscribe Successfull");
    } else {
      Serial.println("Subscribe Failed, Check the Thing Name and Certificates");
      while (1)
        ;
    }
  } else {
    Serial.println("AWS connection failed, Check the HOST Address");
    while (1)
      ;
  }

  delay(3000); /*Sent Empty string to fetch Shadow desired state*/
  int status = shadow.publish(SENT_GET, "{}");
  while (status) {
    Serial.println("Empty String Publish failed (" + String(status) + ")");
    status = shadow.publish(SENT_GET, "{}");
  }
  /*Sent Empty string to fetch Shadow desired state*/
  Serial.println("Empty String Published");
}

void loop() {
  Serial.println(state_str);
  vTaskDelay(5000 / portTICK_RATE_MS);
}

void State::updateDeltaFromJson(StaticJsonDocument<JSON_BUF_LEN>& stateDoc) {
  JsonObject jsonObj = stateDoc["state"].as<JsonObject>();
  if (jsonObj.containsKey("desired"))
    jsonObj = jsonObj["desired"].as<JsonObject>();

  if (jsonObj.containsKey("mode"))
    this->mode = jsonObj["mode"].as<Mode>();

  if (jsonObj.containsKey("lock")) {
    this->lock = jsonObj["lock"].as<LockState>();
    // TODO: Call LOCK/UNLOCK Driver
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

  report();
}

void State::report() {
  StaticJsonDocument<JSON_BUF_LEN> doc; // The size of this can be reduced if needed later
  toJsonDoc(doc);
  serializeJsonPretty(doc, buffer, JSON_BUF_LEN); // DELETE ME LATER
  state_str = String(buffer);
  serializeJson(doc, buffer, JSON_BUF_LEN);
  if (shadow.publish(SHADOW_UPDATE, buffer) == 0) {
    Serial.print("Publish Message:");
    Serial.println(buffer);
  } else {
    Serial.println("Publish failed");
    Serial.println(buffer);
  }
}

void State::toJsonDoc(StaticJsonDocument<JSON_BUF_LEN>& doc) {
  doc.clear();
  doc["state"]["reported"]["mode"] = this->mode;
  doc["state"]["reported"]["lock"] = this->lock;
  JsonArray userArr = doc["state"]["reported"].createNestedArray("users");
  for (User& user : this->users) {
    if (user.valid) {
      JsonObject userObj = userArr.createNestedObject();
      userObj["uuid"] = user.uuid;
      userObj["state"] = user.state;
    }
  }
}

void State::prettyPrint() {
  StaticJsonDocument<JSON_BUF_LEN> doc; // The size of this can be reduced if needed later
  toJsonDoc(doc);
  char buffer[JSON_BUF_LEN];
  serializeJsonPretty(doc, buffer, JSON_BUF_LEN);
  Serial.println(buffer);
}