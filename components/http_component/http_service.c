#include "http_service.h"
#include "esp_http_client.h"
#include "cJSON.h"
#include "esp_log.h"
#include "lwip/dns.h"
#include "lwip/ip_addr.h"


static const char *TAG = "HTTP_CONFIG";

esp_err_t _http_event_handler(esp_http_client_event_t *evt) {
    switch (evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGE(TAG, "HTTP error occurred");
            break;

        case HTTP_EVENT_ON_DATA:
            if (evt->data_len > 0) {
                ESP_LOGI(TAG, "Received data: %.*s", evt->data_len, (char *)evt->data);

                // JSON verisini parse et
                cJSON *json = cJSON_Parse(evt->data);
                if (json == NULL) {
                    ESP_LOGE(TAG, "Failed to parse JSON");
                    break;
                }

                // "led" ve "buzzer" öğelerini al
                const cJSON *led = cJSON_GetObjectItem(json, "led");
                const cJSON *buzzer = cJSON_GetObjectItem(json, "buzzer");

                if (cJSON_IsBool(led)) {
                    led_state = cJSON_IsTrue(led);  // LED durumunu güncelle
                    ESP_LOGI(TAG, "LED state: %s", led_state ? "ON" : "OFF");
                }

                if (cJSON_IsBool(buzzer)) {
                    buzzer_state = cJSON_IsTrue(buzzer);  // Buzzer durumunu güncelle
                    ESP_LOGI(TAG, "Buzzer state: %s", buzzer_state ? "ON" : "OFF");
                }

                cJSON_Delete(json);
            }
            break;

        default:
            break;
    }
    return ESP_OK;
}

esp_err_t http_get_request(void) {

    esp_http_client_config_t config = {
        .url = "http://192.168.1.101:3000/data",  // JSON veri kaynağınızın URL'si
        .method = HTTP_METHOD_GET,
        .event_handler = _http_event_handler,  // Event handler
        .cert_pem = NULL,  // Sertifika olmadan HTTPS bağlantı yap
        .timeout_ms = 10000, // Timeout süresini artır
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_http_client_set_header(client, "Content-Type", "application/json");

    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "GET request successful, status code: %d", esp_http_client_get_status_code(client));
    } else {
        ESP_LOGE(TAG, "GET request failed, error code: %d", err);
    }

    esp_http_client_cleanup(client);

    return err;  // HTTP isteğinin durumuna göre hata kodunu döndür
}
