#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>

void display_init(void);

// Exibe texto na linha (0 a 7) do display OLED
void display_mensagem(const char* msg, int linha);

// Mostra status da bomba com base na classe inferida
void display_exibir_classe(int classe);

// Limpa tela
void display_limpar(void);

void display_limpar_linha(int linha);

#endif // DISPLAY_H
