#include "dht11_component.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "driver/timer.h"
 
static const char *TAG = "DHT11";
 
// DHT11 başlatma fonksiyonu
esp_err_t dht11_init(dht11_t *sensor, int gpio_num) {
    if (!sensor) return ESP_ERR_INVALID_ARG;
    sensor->gpio_num = gpio_num;
    gpio_set_direction(gpio_num, GPIO_MODE_OUTPUT);
    return ESP_OK;
}
 
// Mikro saniye cinsinden gecikme
void delay_us(uint32_t us) {
    uint64_t start = esp_timer_get_time();
    while ((esp_timer_get_time() - start) < us);
}
 
// DHT11 verisini okuma fonksiyonu
esp_err_t dht11_read(dht11_t *sensor) {
    if (!sensor) return ESP_ERR_INVALID_ARG;
 
    uint8_t data[5] = {0};
 
    gpio_set_direction(sensor->gpio_num, GPIO_MODE_OUTPUT);
    // Başlatma sinyali
    gpio_set_level(sensor->gpio_num, 0);
    ESP_LOGI(TAG, "US ONCESI");
    delay_us(20000);
    ESP_LOGI(TAG, "US SONRASI");
    // Cihazın cevap vermesini bekle
    gpio_set_direction(sensor->gpio_num, GPIO_MODE_INPUT);
    delay_us(40);
 
    // Cevap sinyalini kontrol et
    if (!gpio_get_level(sensor->gpio_num)) {
        delay_us(80);
        if (gpio_get_level(sensor->gpio_num)) {
            delay_us(80);
 
            // 40 bitlik veri alımı
            for (int i = 0; i < 40; i++) {
                while (!gpio_get_level(sensor->gpio_num)); // 0 olmasını bekle
                delay_us(50);
                if (gpio_get_level(sensor->gpio_num)) {
                    data[i / 8] <<= 1;
                    data[i / 8] |= 1;
                } else {
                    data[i / 8] <<= 1;
                }
                while (gpio_get_level(sensor->gpio_num)); // 1 olmasını bekle
            }
 
            // Checksum kontrolü
            if ((data[0] + data[1] + data[2] + data[3]) == data[4]) {
                sensor->humidity = data[0] + data[1] * 0.1;
                sensor->temperature = data[2] + data[3] * 0.1;
                ESP_LOGI(TAG, "Nem: %.1f%%, Sıcaklık: %.1f°C", sensor->humidity, sensor->temperature);
                return ESP_OK;
            } else {
                ESP_LOGE(TAG, "Checksum hatası");
                return ESP_ERR_INVALID_RESPONSE;
            }
        }
    }
 
    ESP_LOGE(TAG, "DHT11 cevap vermedi");
    return ESP_ERR_TIMEOUT;
}