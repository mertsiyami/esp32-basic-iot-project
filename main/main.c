#include "esp_log.h"          // ESP32 loglama
#include "esp_wifi.h"         // Wi-Fi işlemleri
#include "esp_event.h"        // Wi-Fi event yönetimi
#include "esp_system.h"       // ESP32 sistem yönetimi
#include "driver/gpio.h"      // GPIO kontrolü
#include "nvs_flash.h"        // NVS flash işlemleri (Wi-Fi için)
#include "driver/ledc.h"
#include "esp_task_wdt.h" 
#include "driver/adc.h"
#include "esp_adc_cal.h"


#include "http_component.h"   // HTTP istekleri için fonksiyonlar
#include "wifi_component.h"   // Wi-Fi bağlantısı için fonksiyonlar
#include "pwm_component.h"
#include "dht11_component.h"

#define FAN_PWM_PIN 25        // PWM çıkışı için GPIO 25
#define FAN_PWM_FREQ 50       // PWM frekansı (1 kHz)
#define FAN_PWM_RESOLUTION LEDC_TIMER_8_BIT // PWM çözünürlüğü (8-bit)

#define LED_PWM_PIN 33
#define LED_PWM_FREQ 50
#define LED_PWM_RESOLUTION LEDC_TIMER_8_BIT

#define SG90_PWM_PIN 14
#define SG90_PWM_FREQ 50
#define SG90_PWM_RESOLUTION LEDC_TIMER_8_BIT

#define ADC_CHANNEL ADC1_CHANNEL_6 // GPIO34
#define ADC_ATTEN ADC_ATTEN_DB_11
#define ADC_WIDTH ADC_WIDTH_BIT_12


bool wifi_status;

bool led_state = false; 
bool led_speed_state = false;

bool fan_state = false ;
bool fan_speed_state = false ;

bool sg90_state = false;
bool sg90_speed_state = false;

static const char *TAG = "MAIN";

// TOPRAK PROJEYE DAHIL OLDU 

void compareStates(pwm_component_t pwm_component, bool state, bool state_speed)
{
    if(state)
    {
        if(state_speed)
        {
            pwm_set_duty(&pwm_component, 255);            
        }
        else
        {
            pwm_set_duty(&pwm_component, 50);   
        }
    }
    else
    {
        pwm_set_duty(&pwm_component, 0);   
    }
}

void app_main(void) {

    dht11_t dht11;
    dht11_init(&dht11, GPIO_NUM_32);  // Timer parametreleri burada geçildi

    adc1_config_width(ADC_WIDTH);
    adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN);

    pwm_component_t fan  = {FAN_PWM_PIN, LEDC_CHANNEL_0, LEDC_TIMER_0, LEDC_HIGH_SPEED_MODE, FAN_PWM_FREQ, LED_PWM_RESOLUTION};
    pwm_component_t led  = {LED_PWM_PIN, LEDC_CHANNEL_1, LEDC_TIMER_1, LEDC_HIGH_SPEED_MODE, LED_PWM_FREQ, LED_PWM_RESOLUTION};
    pwm_component_t sg90 = {SG90_PWM_PIN, LEDC_CHANNEL_2, LEDC_TIMER_2, LEDC_HIGH_SPEED_MODE, SG90_PWM_FREQ, LED_PWM_RESOLUTION};

    pwm_init(&fan);

    pwm_init(&led);

    pwm_init(&sg90);

    wifi_init();  // Wi-Fi bağlantısını başlat

    while (1) {

        if (!is_wifi_connected()) 
        {
            ESP_LOGE(TAG, "Wi-Fi bağlantısı yok. Yeniden bağlanıyor...");
            wifi_init();  // Bağlantı yoksa yeniden bağlanmaya çalış
        }

        http_get_request();

        compareStates(led, led_state, led_speed_state);
        compareStates(fan, fan_state, fan_speed_state);

        if(sg90_state)
        {
            pwm_set_duty(&sg90, 26);            
        }
        else
        {
            pwm_set_duty(&sg90, 13);            
        }

        int adc_reading = adc1_get_raw(ADC_CHANNEL);
        ESP_LOGI(TAG, "ADC Değeri: %d", adc_reading);

        if (dht11_read(&dht11) == ESP_OK) 
        {
            ESP_LOGI(TAG, "Sıcaklık: %.1f°C, Nem: %.1f%%", dht11.temperature, dht11.humidity);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));  // 5 saniye bekle
    }
}

