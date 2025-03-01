#include "esp_log.h"          // ESP32 loglama
#include "esp_wifi.h"         // Wi-Fi işlemleri
#include "esp_event.h"        // Wi-Fi event yönetimi
#include "esp_system.h"       // ESP32 sistem yönetimi
#include "driver/gpio.h"      // GPIO kontrolü
#include "nvs_flash.h"        // NVS flash işlemleri (Wi-Fi için)
#include "driver/ledc.h"

#include "http_component.h"  // HTTP istekleri için fonksiyonlar
#include "wifi_component.h"  // Wi-Fi bağlantısı için fonksiyonlar
#include "pwm_component.h"

#define PWM_PIN 25      // PWM çıkışı için GPIO 25
#define PWM_FREQ 50   // PWM frekansı (1 kHz)
#define PWM_RESOLUTION LEDC_TIMER_8_BIT // PWM çözünürlüğü (8-bit)

bool wifi_status;
bool led_state = false; 
bool buzzer_state = false;  
bool fan_state = false ;
bool fan_speed_state = false ;

static const char *TAG = "MAIN";

// TOPRAK PROJEYE DAHIL OLDU 

void app_main(void) {

    pwm_component_t pwm;
    pwm_init(&pwm, PWM_PIN, PWM_FREQ, PWM_RESOLUTION);


    gpio_set_level(GPIO_NUM_2, 0);
    gpio_set_direction(GPIO_NUM_2,GPIO_MODE_OUTPUT);
    
    wifi_init();  // Wi-Fi bağlantısını başlat
    vTaskDelay(pdMS_TO_TICKS(5000));  // Wi-Fi'ye bağlanabilmesi için yeterince bekleyin

    while (1) {
        if (is_wifi_connected()) {
            ESP_LOGI(TAG, "Wi-Fi bağlantısı başarıyla sağlandı.");
            http_get_request();
            
            if(led_state)
            {
                gpio_set_level(GPIO_NUM_2, 1);
            }
            else
            {
                gpio_set_level(GPIO_NUM_2, 0);
            }
            if(fan_state)
            {
                if(fan_speed_state)
                {
                    pwm_set_duty_cycle(&pwm, 255);
                }
                else
                {
                    pwm_set_duty_cycle(&pwm, 50);
                }

                    
            }
            else
            {
                pwm_set_duty_cycle(&pwm, 0);
            }


        } else {
            ESP_LOGE(TAG, "Wi-Fi bağlantısı sağlanamadı. Yeniden bağlanıyor...");
            wifi_init();  // Bağlantı yoksa yeniden bağlanmaya çalış
            vTaskDelay(pdMS_TO_TICKS(5000));  // Wi-Fi'ye bağlanması için bekleyin
        }

        vTaskDelay(pdMS_TO_TICKS(1000));  // 5 saniye bekle
    }
}

