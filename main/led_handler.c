// FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// ESP-IDF drivers
#include <driver/gpio.h>

// ESP-IDF logging
#include <esp_log.h>

void turn_off_led(const char *TAG, gpio_num_t led_pin);

void configure_led(const char *TAG, gpio_config_t *io_conf, gpio_num_t led_pin) {
    io_conf->intr_type = GPIO_INTR_DISABLE;
    io_conf->mode = GPIO_MODE_OUTPUT;
    io_conf->pin_bit_mask = (1ULL << led_pin);
    io_conf->pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf->pull_up_en = GPIO_PULLUP_DISABLE;

    ESP_LOGI(TAG, "LED configuration set for GPIO %d", led_pin);

    return;
}

void initialize_led(const char *TAG, gpio_config_t *io_conf, gpio_num_t led_pin) {
    ESP_ERROR_CHECK(gpio_config(io_conf));
    
    // Ensure the LED is off at initialization
    turn_off_led(TAG, led_pin);
    ESP_LOGI(TAG, "LED initialized on GPIO %d", led_pin);

    return;
}

void turn_on_led(const char *TAG, gpio_num_t led_pin) {
    ESP_ERROR_CHECK(gpio_set_level(led_pin, 1));

    return;
}

void turn_off_led(const char *TAG, gpio_num_t led_pin) {
    ESP_ERROR_CHECK(gpio_set_level(led_pin, 0));

    return;
}

void blink_led(const char *TAG, gpio_num_t led_pin) {
    for (int i = 0; i < 3; i++) {
        turn_on_led(TAG, led_pin);
        vTaskDelay(pdMS_TO_TICKS(100));
        turn_off_led(TAG, led_pin);
        vTaskDelay(pdMS_TO_TICKS(100));
        turn_on_led(TAG, led_pin);
    }

    return;
}