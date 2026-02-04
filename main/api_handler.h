#include <esp_http_server.h>
#include <dht/dht.h>

void set_respond_type(const char *TAG, httpd_req_t *req);

esp_err_t handle_sensor_request(const char *TAG, httpd_req_t *req, float *humidity, float *temperature);