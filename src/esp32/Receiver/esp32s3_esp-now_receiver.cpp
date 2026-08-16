/*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/  
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <esp_now_manager.hpp>
#include <packets.h>

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Char: ");
  Serial.println(myData.message);
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  delay(1000);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  
  esp_now_man.begin(false, 6, OnDataRecv, nullptr);
  esp_now_man.read_mac_address();
}
 
void loop() {
  delay(100);
}