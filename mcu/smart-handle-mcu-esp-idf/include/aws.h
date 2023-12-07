#ifndef _AWS_H_
#define _AWS_H_

#include "aws_iot_config.h"
#include <AWS_IOT.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <String.h>

#define UUID_LEN 36
#define JSON_BUF_LEN AWS_IOT_MQTT_RX_BUF_LEN

// CHANGE THIS FOR DIFFERENT NETWORK (only 2.4 GHz supported on the esp32 RF module)
const char WIFI_SSID[] = "AndroidAP8168";
const char WIFI_PASSWORD[] = "aokk7001";

/*AWS IOT ENDPOINT DETAILS*/
const char HOST_ADDRESS[] = "av076j80qpn1a-ats.iot.us-east-2.amazonaws.com"; 
const char CLIENT_ID[] = "smart-handle";


/*AWS IOT SHADOW ENDPOINTs*/
const char SHADOW_GET[] = "$aws/things/smart-handle/shadow/get/accepted";
const char SENT_GET[] = "$aws/things/smart-handle/shadow/get";
const char SHADOW_UPDATE[] = "$aws/things/smart-handle/shadow/update";
const char SHADOW_DELTA[] = "$aws/things/smart-handle/shadow/update/delta";

extern AWS_IOT aws_iot;
extern String state_str;
extern char buffer[JSON_BUF_LEN];

// Initialize the underlying Hornbill AWS Library (this function registers a AWS callback queue handler task to FreeRTOS)
void aws_init();

// Sends the inputted JSON to the cloud (AWS IOT) via the SHADOW_UPDATE endpoint 
void aws_report(StaticJsonDocument<JSON_BUF_LEN>&);

// This function gets called by the AWS callback queue handler whenever it receives a payload from a subscribed shadow endpoint
void aws_callback(char* topicName, int payloadLen, char* payload);

#endif