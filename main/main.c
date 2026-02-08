// ESP-IDF components
#include <esp_err.h>
#include <esp_event.h>
#include <esp_http_server.h>
#include <esp_log.h>
#include <esp_wifi.h>

// FreeRTOS
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// Third-party libraries
#include <dht/dht.h>

// Project headers
#include "nvs_helper.h"
#include "wifi_manager.h"
#include "api_handler.h"
#include "http_manager.h"
#include "led_handler.h"

static const char *TAG = "DHT22-API";

static const int ip[4] = {192, 168, 101, 50};
static const int netmask[4] = {255, 255, 255, 0};
static const int gateway[4] = {192, 168, 101, 1};

void setup_wifi(void) {
    initialize_wifi(TAG);

    set_wifi_mode_sta(TAG);
    set_static_ip(TAG, ip, netmask, gateway);

    wifi_config_t sta_config;

    configure_sta_config(TAG, &sta_config);
    set_sta_config(&sta_config);

    start_wifi(TAG);
}

esp_err_t dht_data_handler(httpd_req_t *req) {
    float humidity = 0;
    float temperature = 0;

    set_respond_type(TAG, req);

    return handle_sensor_request(TAG, req, &humidity, &temperature);
}

void start_http_server(void) {
    httpd_config_t config;
    httpd_handle_t server;
    configure_http_server(TAG, &config, &server);

    ESP_LOGI(TAG, "Starting HTTP server...");

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t dht_uri;
        configure_http_uri(TAG, &dht_uri, dht_data_handler);

        httpd_register_uri_handler(server, &dht_uri);

        ESP_LOGI(TAG, "HTTP server started. Access at http://192.168.101.50/sensor");
    } else {
        ESP_LOGE(TAG, "Failed to start HTTP server");
        esp_restart();
    }

    return;
}

void setup_led(void) {
    gpio_config_t io_conf;
    configure_led(TAG, &io_conf, GPIO_NUM_5);

    initialize_led(TAG, &io_conf, GPIO_NUM_5);
}

void app_main(void) {
    ESP_LOGI(TAG, "Starting DHT22 API application...");

    setup_led();

    handle_nvs_error(TAG);

    ESP_LOGI(TAG, "Initializing TCP/IP adapter...");
    tcpip_adapter_init();

    // This is required before initializing WiFi to handle connection events
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    ESP_LOGI(TAG, "Initializing DHT22 sensor on GPIO 16...");
    ESP_ERROR_CHECK(dht_init(GPIO_NUM_16, false));

    setup_wifi();

    // Wait for WiFi to be fully ready
    vTaskDelay(pdMS_TO_TICKS(2000));

    start_http_server();

    ESP_LOGI(TAG, "Application started successfully!");

    // to indicate application is running and wifi is connected
    turn_on_led(TAG, GPIO_NUM_5);

    xTaskCreate(&wifi_reconnect_task, "wifi_reconnect_task", 2048, (void *) TAG, 5, NULL);
}
