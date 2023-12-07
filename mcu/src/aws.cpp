#include "aws.h"
#include "state.h"
#include <ArduinoJson.h>
#include <WiFi.h>

AWS_IOT aws_iot;
String state_str;
char buffer[JSON_BUF_LEN];

void aws_init() {
  int status = WL_IDLE_STATUS;
  status = WiFi.begin(
      WIFI_SSID, WIFI_PASSWORD); // Connect to WPA/WPA2 network. Change this line if using open or WEP network:

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected to wifi");

  if (aws_iot.connect(HOST_ADDRESS, CLIENT_ID) == 0) // Connect to AWS IoT COre
  {
    Serial.println("Connected to AWS");
    delay(1000);

    if (0 == aws_iot.subscribe(SHADOW_GET, aws_callback)) // Subscribe to Accepted GET Shadow Service
    {
      Serial.println("Subscribe Successfull");
    } else {
      Serial.println("Subscribe Failed, Check the Thing Name and Certificates");
      while (1)
        ;
    }
    if (0 == aws_iot.subscribe(SHADOW_DELTA, aws_callback)) // Subscribe to Accepted GET Shadow Service
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
  status = aws_iot.publish(SENT_GET, "{}");
  while (status) {
    Serial.println("Empty String Publish failed (" + String(status) + ")");
    status = aws_iot.publish(SENT_GET, "{}");
  }
  /*Sent Empty string to fetch Shadow desired state*/
  Serial.println("Empty String Published");
}

void aws_report(StaticJsonDocument<JSON_BUF_LEN>& doc) {
  serializeJsonPretty(doc, buffer, JSON_BUF_LEN); // DELETE ME LATER
  state_str = String(buffer);
  serializeJson(doc, buffer, JSON_BUF_LEN);
  while (aws_iot.publish(SHADOW_UPDATE, buffer) != 0) {
    Serial.println("Publish failed... retrying");
  }
  Serial.print("Publish success: ");
  Serial.println(buffer);
}

void aws_callback(char* topicName, int payloadLen, char* payload) {
  Serial.println("CALLBACK");
  Serial.println("Recv Payload (" + String(topicName) + "): " + String(payload));
  Serial.println("Recv Payload len: " + String(payloadLen));
  StaticJsonDocument<JSON_BUF_LEN> doc; // The size of this can be reduced if needed later
  deserializeJson(doc, payload);
  state.updateFromJson(doc);
  if (!strcmp(topicName, SHADOW_GET)) {
    Serial.println("Initial state retrived, unsubscribing from GET");
    while (aws_iot.unsubscribe(SHADOW_GET)) {
      Serial.println("Unsubscribing from GET failed, retrying");
      delay(1000);
    }
  }
}