#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <dht/dht.h>
#include <esp_err.h>
#include <esp_http_server.h>

void http_server_task(void *pvParameter) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t dht_uri = {
            .uri       = "/sensor",
            .method    = HTTP_GET,
            .handler   = dht_data_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &dht_uri);
    }

    while (1) {
        vTaskDelay(portMAX_DELAY);
    }
}

esp_err_t dht_data_handler(httpd_req_t *req) {
    float humidity = 0;
    float temperature = 0;

    ESP_ERROR_CHECK(dht_init(GPIO_NUM_16, false));

    httpd_resp_set_type(req, "application/json");

    if (dht_read_float_data(DHT_TYPE_DHT22, GPIO_NUM_16, &humidity, &temperature) == ESP_OK) {
        char *response = "{\"humidity\": %.2f, \"temperature\": %.2f}", humidity, temperature;
        httpd_resp_send(req, response, -1);
    }

    return ESP_OK;
}

void app_main(void) {
    xTaskCreate(http_server_task, "http_server_task", 4096, NULL, 5, NULL);
}
