#ifndef ESP_NOW_MANAGER_H
#define ESP_NOW_MANAGER_H

#ifdef ESP32
    #include <esp_now.h>
#elif ESP8266
    #include <espnow.h>
#endif

#ifdef ESP32
    // typedef void (*send_callback)(const esp_now_send_info_t *tx_info, esp_now_send_status_t status);
    // typedef void (*receive_callback)(const esp_now_recv_info_t * esp_now_info, const uint8_t *data, int data_len);
    typedef void (*send_callback)(const uint8_t *mac_addr, esp_now_send_status_t status);
    typedef void (*receive_callback)(const uint8_t * mac_addr, const uint8_t *data, int data_len);
#elif ESP8266
    typedef void (*send_callback)(uint8_t *mac_addr, uint8_t sendStatus);
    typedef void (*receive_callback)(uint8_t * mac, uint8_t *incomingData, uint8_t len);
#endif

class esp_now_manager {
    public:
        esp_now_manager();
        bool begin(bool sender, uint8_t channel, receive_callback user_receive_callback, send_callback user_send_callback);
        #ifdef ESP32
        bool register_peer(uint8_t *mac_addr, uint8_t channel, bool encrypted);
        #elif ESP8266
        bool register_peer(uint8_t *mac_addr, uint8_t role, uint8_t channel);
        #endif
        void read_mac_address();
    private:
        bool _sender;
};

extern esp_now_manager esp_now_man;

#endif