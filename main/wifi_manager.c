#include <esp_wifi.h>
#include <esp_log.h>
#include <tcpip_adapter.h>

void initialize_wifi(const char *TAG) {
    ESP_LOGI(TAG, "Initializing WiFi...");

    wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&config));

    return;
}

void set_wifi_mode_apsta(const char *TAG) {
    ESP_LOGI(TAG, "Setting WiFi mode to APSTA...");
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));

    return;
}

void set_static_ip(const char *TAG) {
    tcpip_adapter_ip_info_t ip_info;

    IP4_ADDR(&ip_info.ip,      192, 168, 101, 50);
    IP4_ADDR(&ip_info.gw,      192, 168, 101, 1);
    IP4_ADDR(&ip_info.netmask, 255, 255, 255, 0);

    tcpip_adapter_dhcpc_stop(TCPIP_ADAPTER_IF_STA);

    tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info);

    ESP_LOGI(TAG, "Static IP set to 192.168.101.50");

    return;
}

void configure_apsta_config(const char *TAG, wifi_config_t *ap_config, wifi_config_t *sta_config) {
    *ap_config = (wifi_config_t){
        .ap = {
            .ssid = "ESP8266_ABAY",
            .password = "Akbarkkmu",
            .authmode = WIFI_AUTH_WPA_WPA2_PSK,
            .max_connection = 4,
        }
    };

    *sta_config = (wifi_config_t){
        .sta = {
            .ssid = "Yudatachi",
            .password = "Kevinkanigara",
            .scan_method = WIFI_FAST_SCAN,
        }
    };

    return;
}

void set_apsta_config(wifi_config_t *ap_config, wifi_config_t *sta_config) {
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, ap_config));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, sta_config));

    return;
}

void start_wifi(const char *TAG) {
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_connect());

    ESP_LOGI(TAG, "WiFi AP started. SSID: ESP8266_ABAY  Password: Akbarkkmu");

    return;
}

void wifi_reconnect_task(void *pvParameters) {
    const char *TAG = (const char *) pvParameters;

    while (1) {
        wifi_ap_record_t ap_info;

        if (esp_wifi_sta_get_ap_info(&ap_info) != ESP_OK) {
            ESP_LOGW(TAG, "WiFi disconnected, attempting to reconnect...");
            ESP_ERROR_CHECK(esp_wifi_connect());
        }

        vTaskDelay(pdMS_TO_TICKS(5000));
    }

    return;
}
