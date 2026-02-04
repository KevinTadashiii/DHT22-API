#include <esp_http_server.h>
#include <esp_log.h>

void configure_http_server(const char *TAG, httpd_config_t *config, httpd_handle_t *server) {
    httpd_config_t default_config = HTTPD_DEFAULT_CONFIG();
    *config = default_config;
    *server = NULL;

    ESP_LOGI(TAG, "HTTP server configured");

    return;
}

void configure_http_uri(const char *TAG, httpd_uri_t *dht_uri, esp_err_t (*dht_data_handler)(httpd_req_t *req)) {
    dht_uri->uri      = "/sensor";
    dht_uri->method   = HTTP_GET;
    dht_uri->handler  = dht_data_handler;
    dht_uri->user_ctx = NULL;

    ESP_LOGI(TAG, "HTTP URI configured");

    return;
}