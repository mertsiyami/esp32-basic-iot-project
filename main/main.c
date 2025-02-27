#include "esp_log.h"          // ESP32 loglama
#include "esp_wifi.h"         // Wi-Fi işlemleri
#include "esp_event.h"        // Wi-Fi event yönetimi
#include "esp_system.h"       // ESP32 sistem yönetimi
#include "driver/gpio.h"      // GPIO kontrolü
#include "esp_http_client.h"  // HTTP istemcisi
#include "nvs_flash.h"        // NVS flash işlemleri (Wi-Fi için)

#include "http_service.h"  // HTTP istekleri için fonksiyonlar
#include "wifi_service.h"  // Wi-Fi bağlantısı için fonksiyonlar
#include "gpio_service.h"  // GPIO pinlerinin yapılandırılması için fonksiyonlar

bool wifi_status;
bool led_state = false; 
bool buzzer_state = false;  

static const char *TAG = "MAIN";

// TOPRAK PROJEYE DAHIL OLDU 

void app_main(void) {
    gpio_init();
    wifi_init();  // Wi-Fi bağlantısını başlat
    vTaskDelay(pdMS_TO_TICKS(5000));  // Wi-Fi'ye bağlanabilmesi için yeterince bekleyin

    while (1) {
        if (is_wifi_connected()) {
            ESP_LOGI(TAG, "Wi-Fi bağlantısı başarıyla sağlandı.");
            http_get_request();

            if(buzzer_state)
            {
                gpio_set_level(BUZZER_GPIO, 1);
            }
            else
            {
                gpio_set_level(BUZZER_GPIO, 0);
            }
            if(led_state)
            {
                gpio_set_level(LED_GPIO, 1);
            }
            else
            {
                gpio_set_level(LED_GPIO, 0);
            }
        } else {
            ESP_LOGE(TAG, "Wi-Fi bağlantısı sağlanamadı. Yeniden bağlanıyor...");
            wifi_init();  // Bağlantı yoksa yeniden bağlanmaya çalış
            vTaskDelay(pdMS_TO_TICKS(5000));  // Wi-Fi'ye bağlanması için bekleyin
        }

        vTaskDelay(pdMS_TO_TICKS(1000));  // 5 saniye bekle
    }
}

