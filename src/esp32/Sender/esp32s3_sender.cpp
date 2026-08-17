// Original example credit below.
// I have made some adjustment using my helper class library.

/*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/  
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/

#include <Arduino.h>
#include <WiFi.h>
// #include <esp_wifi.h>
#include <esp_now.h>
#include <esp_now_manager.hpp>
#include <packets.h>

// Toggle low power for testing.
bool lowPower = true;

// The MAC address of a device as an array of bytes.
// Use this to send a packet directly to this device.
uint8_t cardputer_address[] = {0xc0, 0x4e, 0x30, 0x13, 0x05, 0x20};
uint8_t fat_esp8266_address[] = {0x50, 0x02, 0x91, 0xC3, 0x3A, 0x5D};
uint8_t all_address[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // Special MAC address used to send packets to all nearby devices.

// Create a struct. Should match on both the sender and receiver.
// I'm storing mine in a packets.h file.
struct_message myData;

unsigned long lastTime = 0;  
unsigned long timerDelay = 2000;  // send readings timer

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  delay(3000);
  Serial.println("STARTING SENDER");
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  // Not much difference between ESP32 and ESP8266.
  // The callback function parameters differ between the two, however.
  // The first parameter specifies if this is a sender (true) or a receiver (false).
  // At the moment this only affects which callback to be registered. Once I get around to two way communication, I'll adjust this. 
  esp_now_man.begin(true, 6, nullptr, OnDataSent);
  // For ESP32, register_peer requires mac, channel, and a bool for encrypted or not.
  esp_now_man.register_peer(cardputer_address, 6, false);
}
 
void loop() {
  Serial.println("Next message...");
  // Set values to send
  strcpy(myData.message, "THIS IS A CHAR");

  // Send message via ESP-NOW
  // cast your data struct into a byte array.
  // The receiver will use the struct definition to reconstruct the struct.
  esp_now_send(cardputer_address, (uint8_t *) &myData, sizeof(myData));
  delay(100); // let callback do it's thing

  // Trying out the esp32 light sleep mode.
  if (lowPower) {
    Serial.println("Going to sleep for 2 seconds...");
    esp_sleep_enable_timer_wakeup(1000 * 1000); // sleep for 2 seconds, in microseconds.
    esp_light_sleep_start();
  } else {
    delay(2000);
  }
}