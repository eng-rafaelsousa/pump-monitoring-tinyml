#ifndef MQTT_CONNECT_H
#define MQTT_CONNECT_H

#include "lwip/apps/mqtt.h"
#include <stdbool.h>

typedef void (*mqtt_connection_cb_t)(mqtt_client_t *client, void *arg, mqtt_connection_status_t status);

bool mqtt_connect_broker(mqtt_client_t **client, const char *broker_ip, mqtt_connection_cb_t cb);
bool mqtt_is_connected(mqtt_client_t *client);
bool mqtt_publish_data(mqtt_client_t *client, const char *topic, const char *payload);

#endif // MQTT_CONNECT_H
