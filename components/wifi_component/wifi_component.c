#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "lwip/dns.h"
#include "wifi_component.h"

#define WIFI_SSID "FiberHGW_HUX7CK"  // Bağlanılacak Wi-Fi SSID
#define WIFI_PASS "edpJDzux3d7H"    // Bağlanılacak Wi-Fi şifresi

//#define WIFI_SSID "mert"  // Bağlanılacak Wi-Fi SSID
//#define WIFI_PASS "mert123456"    // Bağlanılacak Wi-Fi şifresi

static const char *TAG = "WIFI_CONFIG";

// Wi-Fi olayları için event handler
static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT) {
        if (event_id == WIFI_EVENT_STA_CONNECTED) {
            ESP_LOGI(TAG, "Wi-Fi'ye bağlanıldı");
        } else if (event_id == WIFI_EVENT_STA_DISCONNECTED) {
            ESP_LOGE(TAG, "Wi-Fi'den bağlantı kesildi");
        }
    }
    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ESP_LOGI(TAG, "IP adresi alındı");
    }
}

// Wi-Fi bağlantısının durumunu kontrol eden fonksiyon
bool is_wifi_connected(void) {
    wifi_ap_record_t ap_info;
    esp_err_t err = esp_wifi_sta_get_ap_info(&ap_info);
    if (err == ESP_OK) {
        // Wi-Fi bağlıysa, AP bilgisi alınır
        ESP_LOGI(TAG, "Wi-Fi bağlı: %s", ap_info.ssid);
        return true;
    } else {
        ESP_LOGE(TAG, "Wi-Fi bağlı değil");
        return false;
    }
}

// Wi-Fi başlatma fonksiyonu
void wifi_init(void) {
    // NVS başlatma işlemi
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // ESP32 ağ altyapısını başlatma
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    // Wi-Fi başlatma
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Wi-Fi olay işleyicisini kaydetme
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS
        },
    };

    // ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));  // Wi-Fi modunu STA (client) olarak ayarla
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));  // Wi-Fi yapılandırmasını ayarla
    ESP_ERROR_CHECK(esp_wifi_start());  // Wi-Fi'yi başlat
    ESP_ERROR_CHECK(esp_wifi_connect());  // Wi-Fi'ye bağlan

    ESP_LOGI(TAG, "Wi-Fi'ye bağlanılıyor...");
}
