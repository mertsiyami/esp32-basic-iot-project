#ifndef WIFI_SERVICE_H
#define WIFI_SERVICE_H

#include <stdbool.h>

// Wi-Fi bağlantısını başlatan fonksiyon
void wifi_init(void);

// Wi-Fi'nin bağlı olup olmadığını kontrol eden fonksiyon
bool is_wifi_connected(void);

#endif // WIFI_SERVICE_H
