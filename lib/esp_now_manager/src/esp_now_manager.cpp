#include <Arduino.h>
#include "esp_now_manager.hpp"

#ifdef ESP32
    #include <esp_wifi.h>
    #include <esp_now.h>
#elif ESP8266
    #include <ESP8266WiFi.h>
    #include <espnow.h>
#endif

esp_now_manager esp_now_man;

esp_now_manager::esp_now_manager() {

}

bool esp_now_manager::begin(bool sender, uint8_t channel, receive_callback user_receive_callback, send_callback user_send_callback) {
    #ifdef ESP32

        // Init ESP-NOW
        if (esp_now_init() != 0) {
            Serial.println("Error initializing ESP-NOW");
            return false;
        }

        esp_wifi_set_promiscuous(true);
        esp_wifi_set_channel(6, WIFI_SECOND_CHAN_NONE);
        esp_wifi_set_promiscuous(false);

        if (sender) {
        esp_now_register_send_cb(user_send_callback);
        } else {
        // Once ESPNow is successfully Init, we will register for recv CB to
        // get recv packer info
        esp_now_register_recv_cb(user_receive_callback);
        }
    #elif ESP8266
        // Init ESP-NOW
        if (esp_now_init() != 0) {
            Serial.println("Error initializing ESP-NOW");
            return false;
        }

        wifi_promiscuous_enable(1);
        wifi_set_channel(6);
        wifi_promiscuous_enable(0);

        if (sender) {
            esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
            esp_now_register_send_cb(user_send_callback);
        } else {
            esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
            esp_now_register_recv_cb(user_receive_callback);
        }
    #endif
    return true;
}

#ifdef ESP32
bool esp_now_manager::register_peer(uint8_t *mac_addr, uint8_t channel, bool encrypted) {
    esp_now_peer_info_t peer_info = {};
    memcpy(peer_info.peer_addr, mac_addr, 6);
    peer_info.channel = channel;
    peer_info.encrypt = encrypted;

    if (esp_now_add_peer(&peer_info) != ESP_OK){
        Serial.println("Failed to add peer");
        return false;
    }
    return true;
}
#elif ESP8266
bool esp_now_manager::register_peer(uint8_t *mac_addr, uint8_t role, uint8_t channel) {
    int status = esp_now_add_peer(mac_addr, role, channel, NULL, 0);
    
    if (status != 0) {
        Serial.println("Failed to add peer");
        return false;
    }
    return true;
}
#endif

void esp_now_manager::read_mac_address() {
  #ifdef ESP32
  uint8_t base_mac[6];
  esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, base_mac);
  if (ret == ESP_OK) {
    Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
                  base_mac[0], base_mac[1], base_mac[2],
                  base_mac[3], base_mac[4], base_mac[5]);
  } else {
    Serial.println("Failed to read MAC address");
  }
  #elif ESP8266
    Serial.println(WiFi.macAddress());
  #endif
}