#ifndef PWM_COMPONENT_H
#define PWM_COMPONENT_H

#include "driver/ledc.h"

// PWM Component için yapı
typedef struct {
    int pwm_pin;            // PWM çıkışı için GPIO pin numarası
    int pwm_frequency;      // PWM frekansı
    int pwm_resolution;     // PWM çözünürlüğü
    int pwm_channel;        // PWM kanal numarası
} pwm_component_t;

// PWM Component'i başlatma fonksiyonu
esp_err_t pwm_init(pwm_component_t *pwm, int pin, int frequency, int resolution);

// PWM duty cycle ayarlama fonksiyonu
esp_err_t pwm_set_duty_cycle(pwm_component_t *pwm, int duty_cycle);

#endif // PWM_COMPONENT_H
