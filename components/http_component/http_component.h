#ifndef HTTP_SERVICE_H
#define HTTP_SERVICE_H

#include "esp_http_client.h"
#include "esp_log.h"

// Global değişkenler için extern bildirimleri
extern bool led_state;  // LED durumu
extern bool buzzer_state;  // Buzzer durumu
extern bool fan_state;  // Buzzer durumu
extern bool fan_speed_state;

esp_err_t _http_event_handler(esp_http_client_event_t *evt);
esp_err_t http_get_request(void);

#endif // HTTP_CONFIG_H
