#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <dht/dht.h>
#include <esp_err.h>
#include <esp_http_server.h>

void get_dht_data_task(void *pvParameter) {
    float humidity = 0;
    float temperature = 0;

    ESP_ERROR_CHECK(dht_init(GPIO_NUM_16, false));
    
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    
    while (1) {
        if (dht_read_float_data(DHT_TYPE_DHT22, GPIO_NUM_16, &humidity, &temperature) == ESP_OK) {
            printf("Humidity: %.2f%%, Temperature: %.2fC\n", humidity, temperature);
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void app_main(void) {
    xTaskCreate(get_dht_data_task, "get_dht_data_task", 2048, NULL, 5, NULL);
}
