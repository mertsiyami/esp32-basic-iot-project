// pwm_component.h
#ifndef PWM_COMPONENT_H
#define PWM_COMPONENT_H

#include "driver/ledc.h"
#include "esp_err.h"

// PWM component struct
typedef struct {
    int gpio_num;               // PWM çıkış pini
    ledc_channel_t channel;     // LEDC kanalı
    ledc_timer_t timer;         // LEDC timer
    ledc_mode_t speed_mode;     // Hız modu (high/low)
    uint32_t freq_hz;           // Frekans (Hz)
    ledc_timer_bit_t resolution; // Çözünürlük (bit)
} pwm_component_t;

// Fonksiyon prototipleri
esp_err_t pwm_init(pwm_component_t *pwm);
void pwm_set_duty(pwm_component_t *pwm, uint32_t duty);

#endif // PWM_COMPONENT_H