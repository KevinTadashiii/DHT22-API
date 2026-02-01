#include <stdio.h>
#include <string.h>

#include <dht/dht.h>

#include <esp_err.h>
#include <esp_event.h>
#include <esp_http_server.h>
#include <esp_log.h>
#include <esp_wifi.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <nvs_flash.h>

#include <tcpip_adapter.h>

static const char *TAG = "DHT22-API";

void setup_wifi(void) {
    ESP_LOGI(TAG, "Initializing WiFi...");

    wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&config));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));

    tcpip_adapter_ip_info_t ip_info;

    IP4_ADDR(&ip_info.ip,      192, 168, 101, 50);
    IP4_ADDR(&ip_info.gw,      192, 168, 101, 1);
    IP4_ADDR(&ip_info.netmask, 255, 255, 255, 0);

    tcpip_adapter_dhcpc_stop(TCPIP_ADAPTER_IF_STA);

    tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info);

    wifi_config_t ap_config = {
        .ap = {
            .ssid = "ESP8266_ABAY",
            .password = "Akbarkkmu",
            .authmode = WIFI_AUTH_WPA_WPA2_PSK,
            .max_connection = 4,
        }
    };

    wifi_config_t sta_config = {
        .sta = {
            .ssid = "Yudatachi",
            .password = "Kevinkanigara",
            .scan_method = WIFI_FAST_SCAN,
        }
    };

    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &ap_config));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &sta_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_connect());

    ESP_LOGI(TAG, "WiFi AP started. SSID: ESP8266_ABAY  Password: Akbarkkmu");
}

esp_err_t dht_data_handler(httpd_req_t *req) {
    float humidity = 0;
    float temperature = 0;

    httpd_resp_set_type(req, "application/json");

    if (dht_read_float_data(DHT_TYPE_DHT22, GPIO_NUM_16, &humidity, &temperature) == ESP_OK) {
        char response[100];
        snprintf(response, sizeof(response), "{\"humidity\": %.2f, \"temperature\": %.2f}", humidity, temperature);
        httpd_resp_send(req, response, -1);
        ESP_LOGI(TAG, "Sensor data sent: Temp=%.2f°C, Humidity=%.2f%%", temperature, humidity);
    } else {
        const char *error_response = "{\"error\": \"Failed to read sensor\"}";
        httpd_resp_send_500(req);
        httpd_resp_send(req, error_response, -1);
        ESP_LOGE(TAG, "Failed to read DHT22 sensor");
    }

    return ESP_OK;
}

void start_http_server(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    ESP_LOGI(TAG, "Starting HTTP server...");

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t dht_uri = {
            .uri       = "/sensor",
            .method    = HTTP_GET,
            .handler   = dht_data_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &dht_uri);
        ESP_LOGI(TAG, "HTTP server started. Access at http://192.168.101.50/sensor");
    } else {
        ESP_LOGE(TAG, "Failed to start HTTP server");
    }
}

void wifi_reconnect_task(void *pvParameters) {
    while (1) {
        wifi_ap_record_t ap_info;

        if (esp_wifi_sta_get_ap_info(&ap_info) != ESP_OK) {
            ESP_LOGW(TAG, "WiFi disconnected, attempting to reconnect...");
            ESP_ERROR_CHECK(esp_wifi_connect());
        }

        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void app_main(void) {
    ESP_LOGI(TAG, "Starting DHT22 API application...");

    if (nvs_flash_init() != ESP_OK) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    ESP_LOGI(TAG, "Initializing TCP/IP adapter...");
    tcpip_adapter_init();

    ESP_ERROR_CHECK(esp_event_loop_create_default());

    ESP_LOGI(TAG, "Initializing DHT22 sensor on GPIO 16...");
    ESP_ERROR_CHECK(dht_init(GPIO_NUM_16, false));

    setup_wifi();

    // Wait for WiFi to be fully ready
    vTaskDelay(pdMS_TO_TICKS(2000));

    start_http_server();

    ESP_LOGI(TAG, "Application started successfully!");

    xTaskCreate(&wifi_reconnect_task, "wifi_reconnect_task", 2048, NULL, 5, NULL);
}
