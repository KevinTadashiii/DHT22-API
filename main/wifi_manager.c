#include <esp_wifi.h>
#include <esp_log.h>
#include <tcpip_adapter.h>

#include "led_handler.h"

void initialize_wifi(const char *TAG) {
    ESP_LOGI(TAG, "Initializing WiFi...");

    wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&config));

    return;
}

void set_wifi_mode_sta(const char *TAG) {
    ESP_LOGI(TAG, "Setting WiFi mode to STA...");
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    return;
}

void set_static_ip(const char *TAG, const int ip[4], const int netmask[4], const int gateway[4]) {
    tcpip_adapter_ip_info_t ip_info;

    IP4_ADDR(&ip_info.ip,      ip[0], ip[1], ip[2], ip[3]);
    IP4_ADDR(&ip_info.gw,      gateway[0], gateway[1], gateway[2], gateway[3]);
    IP4_ADDR(&ip_info.netmask, netmask[0], netmask[1], netmask[2], netmask[3]);

    tcpip_adapter_dhcpc_stop(TCPIP_ADAPTER_IF_STA);

    tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info);

    ESP_LOGI(TAG, "Static IP set to %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);

    return;
}

void configure_sta_config(const char *TAG, wifi_config_t *sta_config) {
    *sta_config = (wifi_config_t){
        .sta = {
            .ssid = "Yudatachi",
            .password = "Kevinkanigara",
            .scan_method = WIFI_FAST_SCAN,
        }
    };

    return;
}

void set_sta_config(wifi_config_t *sta_config) {
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, sta_config));

    return;
}

void start_wifi(const char *TAG) {
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_connect());

    ESP_LOGI(TAG, "WiFi STA started.");

    return;
}

void wifi_reconnect_task(void *pvParameters) {
    const char *TAG = (const char *) pvParameters;

    while (1) {
        wifi_ap_record_t ap_info;

        if (esp_wifi_sta_get_ap_info(&ap_info) != ESP_OK) {
            turn_off_led(TAG, GPIO_NUM_5);

            ESP_LOGW(TAG, "WiFi disconnected, attempting to reconnect...");
            ESP_ERROR_CHECK(esp_wifi_connect());
        } else {
            turn_on_led(TAG, GPIO_NUM_5);
        }

        vTaskDelay(pdMS_TO_TICKS(15000));
    }

    return;
}
