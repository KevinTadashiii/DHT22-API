#include "nvs_helper.h"
#include <esp_log.h>
#include <esp_err.h>
#include <nvs_flash.h>

void handle_nvs_error(const char *TAG) {
    if (nvs_flash_init() != ESP_OK) {
        ESP_LOGI(TAG, "NVS Flash init failed, erasing and retrying...");
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    return;
}