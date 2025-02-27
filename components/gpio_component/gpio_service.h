#ifndef GPIO_SERVICE_H
#define GPIO_SERVICE_H

#include "driver/gpio.h"

// LED ve buzzer pin tanımları
#define LED_GPIO GPIO_NUM_2
#define BUZZER_GPIO GPIO_NUM_32

void gpio_init(void);

#endif
