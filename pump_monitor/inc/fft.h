#ifndef FFT_H
#define FFT_H

#include <stdbool.h>
#include <stdint.h>

// Tamanho da FFT configurável (recomendado 128 ou 256 para o Pico W)
#define FFT_SIZE 256

/**
 * @brief Inicializa o sistema de FFT
 */
void fft_init(void);

/**
 * @brief Adiciona uma nova amostra para análise
 * @param sample Valor da amostra do sensor (normalizado)
 */
void fft_add_sample(float sample);

/**
 * @brief Verifica se há dados suficientes para processamento
 * @return true se FFT_SIZE amostras foram coletadas
 */
bool fft_is_ready(void);

/**
 * @brief Executa o cálculo da FFT
 * @note Automaticamente reinicia o buffer após o cálculo
 */
void fft_compute(void);

/**
 * @brief Obtém o espectro de magnitudes (apenas metade útil)
 * @return Ponteiro para o array de magnitudes (tamanho FFT_SIZE/2)
 * 
 * @note O array contém:
 * - Índice 0: Componente DC
 * - Índices 1 a N-1: Magnitudes das frequências (bin_width = sample_rate/FFT_SIZE)
 */
const float* fft_get_magnitude(void);

/**
 * @brief Obtém a frequência correspondente a cada bin
 * @param sample_rate Taxa de amostragem em Hz
 * @return Frequência em Hz para o índice especificado
 */
float fft_get_frequency(uint16_t index, float sample_rate);

/**
 * @brief Encontra o pico de frequência dominante
 * @param start_bin Primeiro bin a considerar (evitar DC)
 * @param end_bin Último bin a considerar
 * @return Índice do bin com maior magnitude
 */
uint16_t fft_find_peak_frequency(uint16_t start_bin, uint16_t end_bin);

void fft_format_json(const float* mags, int len, char* buffer, int buffer_size); // Converte magnitudes FFT em um JSON simples

#endif // FFT_H