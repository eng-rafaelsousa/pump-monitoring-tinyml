#include "disp/display.h"
#include "disp/ssd1306_i2c.h"
#include "disp/ssd1306.h"
#include "disp/ssd1306_font.h"
#include <stdio.h>

#define OLED_I2C i2c1
#define OLED_SDA 14
#define OLED_SCL 15

static uint8_t ssd[ssd1306_buffer_length] = {0};
static struct render_area area = {
    .start_column = 0,
    .end_column = ssd1306_width - 1,
    .start_page = 0,
    .end_page = ssd1306_n_pages - 1
};

void display_init(void) {
    i2c_init(OLED_I2C, 400 * 1000);
    gpio_set_function(OLED_SDA, GPIO_FUNC_I2C);
    gpio_set_function(OLED_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(OLED_SDA);
    gpio_pull_up(OLED_SCL);
    ssd1306_init();
    calculate_render_area_buffer_length(&area);
    ssd1306_clear_display(ssd);
    render_on_display(ssd, &area);
}

void display_mensagem(const char* msg, int linha) {
    if (linha < 0 || linha > 7) return; // Display tem 8 linhas (64px / 8px)

    ssd1306_draw_string(ssd, 0, linha * 8, (char*)msg);
    render_on_display(ssd, &area);
}

void display_exibir_classe(int classe) {
    const char *nomes[] = {"Desligada", "Ligada",  "Anomalia"};

    ssd1306_clear_display(ssd);
    display_mensagem("Status da bomba", 0);
    display_mensagem(nomes[classe], 3);
   // ssd1306_draw_string(ssd, 0, 0, "Status da bomba:");
    //ssd1306_draw_string(ssd, 0, 10, nomes[classe]);
    //render_on_display(ssd, &area);
}

void display_limpar(void) {
    ssd1306_clear_display(ssd);
    render_on_display(ssd, &area);
}

void display_limpar_linha(int linha) {
    if (linha < 0 || linha >= ssd1306_n_pages) return;

    // Cada "linha" é uma page de 8 pixels verticais
    int start = linha * ssd1306_width;

    for (int i = 0; i < ssd1306_width; i++) {
        ssd[start + i] = 0x00;
    }

    struct render_area linha_area = {
        .start_column = 0,
        .end_column = ssd1306_width - 1,
        .start_page = linha,
        .end_page = linha
    };

    calculate_render_area_buffer_length(&linha_area);
    render_on_display(ssd, &linha_area);
}

