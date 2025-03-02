#ifndef DHT11_H
#define DHT11_H

#include "esp_err.h"
#include "driver/gpio.h"
#include "driver/timer.h"

// DHT11 sensörünü temsil eden yapı
typedef struct {
    int gpio_num;         // GPIO numarası
    float temperature;    // Sıcaklık değeri
    float humidity;       // Nem değeri
} dht11_t;


esp_err_t dht11_init(dht11_t *sensor, int gpio_num);

// Mikro saniye cinsinden gecikme fonksiyonu
void delay_us(uint32_t us);

// DHT11 verisini okuma fonksiyonu
esp_err_t dht11_read(dht11_t *sensor);

#endif // DHT11_H
