// Bibliotecas padrão
#include <stdio.h>
#include <math.h>

// Pico SDK
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "pico/multicore.h"

// Configuração e módulos do projeto
#include "inc/pump_monitor.h"
#include "inc/config.h"
#include "inc/core1_task.h"
#include "inc/mpu6050.h"
#include "inc/fft.h"
#include "inc/inference.h"
#include "inc/wifi_connect.h"
#include "inc/mqtt_connect.h"
#include "inc/rgb_status.h"
#include "inc/buzzer.h"
#include "disp/display.h"

#define FIFO_SIGNAL_PROCESS 0x1234

// === Variáveis privadas ===
static mqtt_client_t *mqtt = NULL;
static char fft_json_buffer[MAX_JSON_LEN];
static volatile bool fft_ready_to_send = false;

// === Inicialização completa do sistema ===
void system_init(void) {
    rgb_init();
    rgb_azul();
    buzzer_init();
    display_init();
    display_mensagem("Inicializando...", 0);
    sleep_ms(1000);
    display_limpar_linha(0);

    // ---- WiFi ----
    display_mensagem("Conectando WiFi...", 3);
    if (!wifi_connect_sta(WIFI_SSID, WIFI_PASS)) {
        display_limpar_linha(3);
        display_mensagem("Falha!", 3);
        rgb_vermelho();
        while (1); // trava se falhar
    }
    display_limpar_linha(3);
    display_mensagem("WiFi OK", 3);
    sleep_ms(1000);

    // ---- MQTT ----
    display_mensagem("Conectando MQTT...", 5);
    if (!mqtt) {
        if (!mqtt_connect_broker(&mqtt, MQTT_BROKER_IP,  NULL)) {
            display_limpar_linha(5);
            display_mensagem("Falha!", 5);
            while (1);
        }
    }
    display_limpar_linha(5);
    display_mensagem("MQTT OK", 5);
    sleep_ms(1000);

    // ---- MPU6050 ----
    display_mensagem("Conectando MPU...", 7);
    i2c_init(i2c0, 400 * 1000);
    gpio_set_function(SDA_MPU, GPIO_FUNC_I2C);
    gpio_set_function(SCL_MPU, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_MPU);
    gpio_pull_up(SCL_MPU);

    if (!mpu6050_init()) {
        display_limpar_linha(7);
        display_mensagem("Falha!", 7);
        while (1);
    }
    display_limpar_linha(7);
    display_mensagem("MPU6050 OK", 7);
    sleep_ms(3000);

    display_limpar();
    display_mensagem("Sistema OK", 3);
    sleep_ms(2000);
    display_limpar();
    rgb_apagar();
}

// === Ciclo de aquisição, inferência e envio ===
void pump_process_cycle(void) {
    int16_t ax, ay, az;
    mpu6050_read_accel(&ax, &ay, &az);
    fft_add_sample(az / G_SCALE);

    if (fft_is_ready()) {
        multicore_fifo_push_blocking(FIFO_SIGNAL_PROCESS);
        int classe = multicore_fifo_pop_blocking();

        if (classe == 2) {
            buzzer_trigger_beeps(3);
        }

        const float* mags = fft_get_magnitude();
        fft_format_json(mags, PRINT_BINS, fft_json_buffer, MAX_JSON_LEN);
        fft_ready_to_send = true;

        display_exibir_classe(classe);

        if (mqtt_is_connected(mqtt)) {
            mqtt_publish_data(mqtt, MQTT_TOPIC_FFT, fft_json_buffer);
            fft_ready_to_send = false;
            rgb_piscar_verde();
        }
    }
}

// === Função principal ===
int main(void) {
    system_init();
    multicore_launch_core1(main_core1_tasks);
    fft_init();

    while (1) {
        pump_process_cycle();
        buzzer_update();
        sleep_us(1000000 / SAMPLE_RATE);
    }
}
