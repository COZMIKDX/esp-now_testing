/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp8266-nodemcu-arduino-ide/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <ESP8266WiFi.h>
#include <espnow.h>
#include <esp_now_manager.hpp>
#include <packets.h>

// Create a struct_message called myData
struct_message myData;

bool newData = false;

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  newData = true;
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // Toggle the LED state
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  delay(2000);
  Serial.println("esp now basic receiver example");
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  esp_now_man.begin(false, 6, OnDataRecv, nullptr);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (newData) {
    Serial.print("Char: ");
    Serial.println(myData.message);
    newData = false;
  }
  delay(100);
}
