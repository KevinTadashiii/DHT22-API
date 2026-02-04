#include <esp_wifi_types.h>

void initialize_wifi(const char *TAG);

void set_wifi_mode_apsta(const char *TAG);

void set_static_ip(const char *TAG);

void configure_apsta_config(const char *TAG, wifi_config_t *ap_config, wifi_config_t *sta_config);

void set_apsta_config(wifi_config_t *ap_config, wifi_config_t *sta_config);

void start_wifi(const char *TAG);

void wifi_reconnect_task(void *pvParameters);
