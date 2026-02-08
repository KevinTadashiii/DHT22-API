void configure_http_server(const char *TAG, httpd_config_t *config, httpd_handle_t *server);

void configure_http_uri(const char *TAG, httpd_uri_t *dht_uri, esp_err_t (*dht_data_handler)(httpd_req_t *req));