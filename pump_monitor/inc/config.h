#ifndef CONFIG_H
#define CONFIG_H

// === Wi-Fi ===
#define WIFI_SSID     "SUA REDE"
#define WIFI_PASS     "SUA SENHA"

// === MQTT ===
#define MQTT_BROKER_IP      "IP DO BROKER"
#define MQTT_BROKER_PORT    1883
#define MQTT_CLIENT_ID      "pico_pump_monitor"
#define MQTT_TOPIC_FFT      "pump/fft"

// === FFT e Processamento ===
#define SAMPLE_RATE     200        // Hz (taxa de amostragem)
#define PRINT_BINS      11         // Número de bins a enviar
#define MAX_JSON_LEN    256        // Tamanho do buffer para JSON

// === MPU6050 ===
#define G_RANGE         2          // ±2g
#define G_SCALE         16384.0f   // Fator de escala (para ±2g)
#define SDA_MPU         0
#define SCL_MPU         1

// === Pinos de LED RGB ===
#define LED_R           13
#define LED_G           11
#define LED_B           12

// === Buzzer ===
#define BUZZER_PIN      10

#endif // CONFIG_H
