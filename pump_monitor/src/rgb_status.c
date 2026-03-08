#include "inc/rgb_status.h"
#include "pico/stdlib.h"

#define GPIO_R 13
#define GPIO_G 11
#define GPIO_B 12

void rgb_init(void) {
    gpio_init(GPIO_R);
    gpio_init(GPIO_G);
    gpio_init(GPIO_B);
    gpio_set_dir(GPIO_R, GPIO_OUT);
    gpio_set_dir(GPIO_G, GPIO_OUT);
    gpio_set_dir(GPIO_B, GPIO_OUT);
    rgb_apagar();
}

void rgb_set(bool r, bool g, bool b) {
    gpio_put(GPIO_R, r ? 1 : 0);
    gpio_put(GPIO_G, g ? 1 : 0);
    gpio_put(GPIO_B, b ? 1 : 0);
}

void rgb_azul(void) {
    rgb_set(false, false, true);
}

void rgb_verde(void) {
    rgb_set(false, true, false);
}

void rgb_vermelho(void) {
    rgb_set(true, false, false);
}

void rgb_apagar(void) {
    rgb_set(false, false, false);
}

void rgb_piscar_verde(void) {
    rgb_verde();
    sleep_ms(150);
    rgb_apagar();
}
