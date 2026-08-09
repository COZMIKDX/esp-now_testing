#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>

bool lowPower = true;

// uint8_t broadcastAddress[] = {0xc0, 0x4e, 0x30, 0x13, 0x05, 0x20};
uint8_t broadcastAddress[] = {0x50, 0x02, 0x91, 0xC3, 0x3A, 0x5D}; // Replace with the MAC address of the receiver

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32];
} struct_message;

// Create a struct_message called myData
struct_message myData;
esp_now_peer_info_t peerInfo;

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
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(6, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_ps(WIFI_PS_MIN_MODEM);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 6;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  // Set values to send
  strcpy(myData.a, "THIS IS A CHAR");

  // Send message via ESP-NOW
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  delay(100); // let callback do it's thing

  if (lowPower) {
    Serial.println("Going to sleep for 2 seconds...");
    esp_sleep_enable_timer_wakeup(1000 * 1000); // sleep for 2 seconds, in microseconds.
    esp_light_sleep_start();
  } else {
    delay(2000);
  }
}