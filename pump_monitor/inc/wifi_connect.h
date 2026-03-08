#ifndef WIFI_CONNECT_H
#define WIFI_CONNECT_H

#include <stdbool.h>

/**
 * @brief Inicializa e conecta ao Wi-Fi como STA.
 * 
 * @param ssid Nome da rede Wi-Fi (SSID)
 * @param password Senha da rede Wi-Fi
 * @return true Se conectar com sucesso
 * @return false Se falhar ao conectar
 */
bool wifi_connect_sta(const char* ssid, const char* password);

#endif // WIFI_CONNECT_H
