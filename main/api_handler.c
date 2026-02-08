#include <esp_http_server.h>
#include <dht/dht.h>
#include <esp_log.h>

#include "led_handler.h"

void set_respond_type(const char *TAG, httpd_req_t *req) {
    httpd_resp_set_type(req, "application/json");
    ESP_LOGI(TAG, "Response type set to application/json");

    return;
}

esp_err_t handle_sensor_request(const char *TAG, httpd_req_t *req, float *humidity, float *temperature) {
    if (dht_read_float_data(DHT_TYPE_DHT22, GPIO_NUM_16, humidity, temperature) == ESP_OK) {
        turn_on_led(TAG, GPIO_NUM_5);

        char response[100];
        snprintf(response, sizeof(response), "{\"humidity\": %.2f, \"temperature\": %.2f}", *humidity, *temperature);
        httpd_resp_send(req, response, -1);
        ESP_LOGI(TAG, "Sensor data sent: Temp=%.2f°C, Humidity=%.2f%%", *temperature, *humidity);

        blink_led(TAG, GPIO_NUM_5);
    } else {
        httpd_resp_send_500(req);
        ESP_LOGE(TAG, "Failed to read DHT22 sensor");
    }

    return ESP_OK;
}