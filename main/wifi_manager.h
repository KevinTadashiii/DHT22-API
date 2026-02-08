#include <esp_wifi_types.h>

void initialize_wifi(const char *TAG);

void set_wifi_mode_sta(const char *TAG);

void set_static_ip(const char *TAG, const int ip[4], const int netmask[4], const int gateway[4]);

void configure_sta_config(const char *TAG, wifi_config_t *sta_config);

void set_sta_config(wifi_config_t *sta_config);

void start_wifi(const char *TAG);

void wifi_reconnect_task(void *pvParameters);
