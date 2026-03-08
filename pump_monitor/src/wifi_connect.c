#include "inc/wifi_connect.h"
#include "pico/cyw43_arch.h"
#include <stdio.h>

bool wifi_connect_sta(const char* ssid, const char* password) {
    if (cyw43_arch_init()) {
        printf("[WiFi] Erro ao inicializar o driver CYW43.\n");
        return false;
    }

    cyw43_arch_enable_sta_mode();
    printf("[WiFi] Conectando à rede \"%s\"...\n", ssid);

    int result = cyw43_arch_wifi_connect_timeout_ms(ssid, password, CYW43_AUTH_WPA2_AES_PSK, 10000);
    if (result != 0) {
        printf("[WiFi] Falha ao conectar (código de erro: %d)\n", result);
        return false;
    }

    printf("[WiFi] Conectado com sucesso!\n");
    return true;
}
