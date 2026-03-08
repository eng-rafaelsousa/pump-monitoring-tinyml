#include "inc/mqtt_connect.h"
#include "inc/config.h"
#include "lwip/ip_addr.h"
#include <stdio.h>
#include <string.h>

// === Callback padrão apenas para log ===
static void default_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status) {
    if (status == MQTT_CONNECT_ACCEPTED) {
        printf("MQTT conectado ao broker.\n");
    } else {
        printf("Falha na conexão MQTT: %d\n", status);
    }
}

// === Conecta ao broker MQTT ===
bool mqtt_connect_broker(mqtt_client_t **client, const char *broker_ip, mqtt_connection_cb_t cb) {
    if (!client) return false;

    *client = mqtt_client_new();
    if (!(*client)) {
        printf("Erro ao criar cliente MQTT.\n");
        return false;
    }

    ip_addr_t broker_addr;
    if (!ipaddr_aton(broker_ip, &broker_addr)) {
        printf("Endereço IP do broker inválido.\n");
        return false;
    }

    struct mqtt_connect_client_info_t ci = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = 60,
        .will_topic = NULL,
        .will_msg = NULL,
        .will_qos = 0,
        .will_retain = 0
    };

    mqtt_client_connect(*client, &broker_addr, MQTT_BROKER_PORT,
                        cb ? cb : default_connection_cb,
                        NULL, &ci);

    return true;
}

// === Verifica se o cliente está conectado ===
bool mqtt_is_connected(mqtt_client_t *client) {
    return client && mqtt_client_is_connected(client);
}

// === Callback de publicação ===
static void mqtt_pub_cb(void *arg, err_t result) {
    if (result == ERR_OK) {
        printf("Publicação MQTT enviada com sucesso!\n");
    } else {
        printf("Erro na publicação MQTT: %d\n", result);
    }
}

// === Publica dados no broker ===
bool mqtt_publish_data(mqtt_client_t *client, const char *topic, const char *payload) {
    if (!mqtt_is_connected(client)) {
        printf("MQTT desconectado, não foi possível publicar.\n");
        return false;
    }

    err_t res = mqtt_publish(client, topic, payload, strlen(payload), 0, 0, mqtt_pub_cb, NULL);
    if (res != ERR_OK) {
        printf("Erro ao publicar MQTT: %d\n", res);
        return false;
    }
    return true;
}
