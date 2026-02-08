#include <driver/gpio.h>

void initialize_led(const char *TAG, gpio_num_t led_pin);

void turn_on_led(const char *TAG, gpio_num_t led_pin);

void turn_off_led(const char *TAG, gpio_num_t led_pin);

void blink_led(const char *TAG, gpio_num_t led_pin);