#include "pwm_component.h"
#include "esp_log.h"

// PWM Component'ini başlatma fonksiyonu
esp_err_t pwm_init(pwm_component_t *pwm, int pin, int frequency, int resolution) {
    if (!pwm) {
        return ESP_ERR_INVALID_ARG;
    }

    pwm->pwm_pin = pin;
    pwm->pwm_frequency = frequency;
    pwm->pwm_resolution = resolution;
    pwm->pwm_channel = LEDC_CHANNEL_0;  // Kanal numarasını belirleyin

    // PWM zamanlayıcısını yapılandırma
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = pwm->pwm_resolution,  // PWM çözünürlüğü
        .freq_hz = pwm->pwm_frequency,           // PWM frekansı
        .speed_mode = LEDC_HIGH_SPEED_MODE,      // Yüksek hız modu
        .timer_num = LEDC_TIMER_0               // Zamanlayıcı numarası
    };

    ledc_timer_config(&ledc_timer);

    // PWM kanalını yapılandırma
    ledc_channel_config_t ledc_channel = {
        .channel    = pwm->pwm_channel,    // PWM kanal numarası
        .gpio_num   = pwm->pwm_pin,        // PWM çıkışı pini
        .speed_mode = LEDC_HIGH_SPEED_MODE, // Yüksek hız modu
        .intr_type  = LEDC_INTR_DISABLE,    // Kesme tipini devre dışı bırak
        .timer_sel  = LEDC_TIMER_0,        // Zamanlayıcı seçimi
        .duty       = 0,                    // Başlangıçta PWM genişliği sıfır (yani, %0)
        .hpoint     = 0                     // Başlangıçta zamanlayıcı sıfır
    };

    ledc_channel_config(&ledc_channel);
    return ESP_OK;
}

// PWM duty cycle'ı ayarlama fonksiyonu
esp_err_t pwm_set_duty_cycle(pwm_component_t *pwm, int duty_cycle) {
    if (!pwm) {
        return ESP_ERR_INVALID_ARG;
    }

    // Duty cycle'ı 0-255 arası ayarlayın (8-bit çözünürlük)
    if (duty_cycle < 0 || duty_cycle > 255) {
        return ESP_ERR_INVALID_ARG;
    }

    // PWM darbe genişliğini ayarlama
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, pwm->pwm_channel, duty_cycle);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, pwm->pwm_channel);
    return ESP_OK;
}
