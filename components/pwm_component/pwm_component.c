// pwm_component.c
#include "pwm_component.h"

// PWM başlatma fonksiyonu
esp_err_t pwm_init(pwm_component_t *pwm) {
    ledc_timer_config_t timer_config = {
        .speed_mode = pwm->speed_mode,
        .duty_resolution = pwm->resolution,
        .timer_num = pwm->timer,
        .freq_hz = pwm->freq_hz,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&timer_config));

    ledc_channel_config_t channel_config = {
        .gpio_num = pwm->gpio_num,
        .speed_mode = pwm->speed_mode,
        .channel = pwm->channel,
        .timer_sel = pwm->timer,
        .duty = 0,
        .hpoint = 0
    };
    return ledc_channel_config(&channel_config);
}

// Duty cycle ayarlama fonksiyonu
void pwm_set_duty(pwm_component_t *pwm, uint32_t duty) {
    ledc_set_duty(pwm->speed_mode, pwm->channel, duty);
    ledc_update_duty(pwm->speed_mode, pwm->channel);
}

