#include <driver/gpio.h>

void configure_led(const char *TAG, gpio_config_t *io_conf, gpio_num_t led_pin);

void initialize_led(const char *TAG, gpio_config_t *io_conf, gpio_num_t led_pin);

void turn_on_led(const char *TAG, gpio_num_t led_pin);

void turn_off_led(const char *TAG, gpio_num_t led_pin);

void blink_led(const char *TAG, gpio_num_t led_pin);