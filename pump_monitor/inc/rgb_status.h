#ifndef RGB_STATUS_H
#define RGB_STATUS_H

#include <stdint.h>
#include <stdbool.h>

// Inicializa os pinos RGB como saída
void rgb_init(void);

// Liga/desliga individualmente cada cor
void rgb_set(bool r, bool g, bool b);

// Estados prontos
void rgb_azul(void);
void rgb_verde(void);
void rgb_vermelho(void);
void rgb_apagar(void);

// Piscar verde brevemente (envio MQTT OK)
void rgb_piscar_verde(void);

#endif
