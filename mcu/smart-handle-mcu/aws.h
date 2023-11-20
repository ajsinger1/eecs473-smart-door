#ifndef _AWS_H_
#define _AWS_H_

#include "aws_iot_config.h"
#include <AWS_IOT.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <String.h>

#define UUID_LEN 36
#define JSON_BUF_LEN AWS_IOT_MQTT_RX_BUF_LEN

const char WIFI_SSID[] = "Ari’s iPhone";
const char WIFI_PASSWORD[] = "beefstik";
const char HOST_ADDRESS[] = "av076j80qpn1a-ats.iot.us-east-2.amazonaws.com"; // AWS IoT Custom Endpoint Address
const char CLIENT_ID[] = "smart-handle";
const char SHADOW_GET[] = "$aws/things/smart-handle/shadow/get/accepted";
const char SENT_GET[] = "$aws/things/smart-handle/shadow/get";
const char SHADOW_UPDATE[] = "$aws/things/smart-handle/shadow/update";
const char SHADOW_DELTA[] = "$aws/things/smart-handle/shadow/update/delta";

extern AWS_IOT aws_iot;
extern String state_str;
extern char buffer[JSON_BUF_LEN];

void aws_init();
void aws_report(StaticJsonDocument<JSON_BUF_LEN>&);
void aws_callback(char* topicName, int payloadLen, char* payload);

#endif