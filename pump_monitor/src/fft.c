#include "inc/fft.h"
#include <math.h>
#include <string.h>
#include <stdio.h> 

// Buffers para processamento
static float samples[FFT_SIZE];
static float magnitudes[FFT_SIZE / 2];
static uint16_t sample_index = 0;

// Lookup tables para otimização
static float cos_table[FFT_SIZE];
static float sin_table[FFT_SIZE];
static float hann_window[FFT_SIZE];
static bool tables_initialized = false;

// Inicializa as tabelas de lookup
static void init_tables() {
    if (!tables_initialized) {
        const float two_pi = 6.283185307179586f;
        
        for (int i = 0; i < FFT_SIZE; i++) {
            float angle = -two_pi * i / FFT_SIZE;
            cos_table[i] = cosf(angle);  // Função otimizada do SDK
            sin_table[i] = sinf(angle);   // Função otimizada do SDK
            hann_window[i] = 0.5f * (1.0f - cosf(two_pi * i / (FFT_SIZE - 1)));
        }
        tables_initialized = true;
    }
}

// Implementação iterativa da FFT
static void optimized_fft(float* real, float* imag, int n) {
    // Bit-reversal permutation
    int j = 0;
    for (int i = 0; i < n; i++) {
        if (j > i) {
            float temp_real = real[j];
            float temp_imag = imag[j];
            real[j] = real[i];
            imag[j] = imag[i];
            real[i] = temp_real;
            imag[i] = temp_imag;
        }
        
        int m = n >> 1;
        while (m >= 1 && j >= m) {
            j -= m;
            m >>= 1;
        }
        j += m;
    }

    // FFT computation
    for (int s = 1; s <= (int)log2f(n); s++) {
        int m = 1 << s;
        int m2 = m >> 1;
        
        for (int k = 0; k < n; k += m) {
            for (int j = 0; j < m2; j++) {
                int idx = j * n / m;
                float t_real = cos_table[idx] * real[k + j + m2] - sin_table[idx] * imag[k + j + m2];
                float t_imag = sin_table[idx] * real[k + j + m2] + cos_table[idx] * imag[k + j + m2];
                
                real[k + j + m2] = real[k + j] - t_real;
                imag[k + j + m2] = imag[k + j] - t_imag;
                real[k + j] += t_real;
                imag[k + j] += t_imag;
            }
        }
    }
}

void fft_init(void) {
    memset(samples, 0, sizeof(samples));
    memset(magnitudes, 0, sizeof(magnitudes));
    sample_index = 0;
    init_tables();  // Pré-computa as tabelas
}

void fft_add_sample(float sample) {
    if (sample_index < FFT_SIZE) {
        samples[sample_index++] = sample;
    }
}

bool fft_is_ready(void) {
    return (sample_index >= FFT_SIZE);
}

void fft_compute(void) {
    if (!fft_is_ready()) return;
    
    float real[FFT_SIZE], imag[FFT_SIZE] = {0};
    
    // Aplica janelamento
    for (int i = 0; i < FFT_SIZE; i++) {
        real[i] = samples[i] * hann_window[i];
    }
    
    optimized_fft(real, imag, FFT_SIZE);
    
    // Calcula magnitudes (apenas metade é simétrica)
    for (int i = 0; i < FFT_SIZE / 2; i++) {
        magnitudes[i] = sqrtf(real[i]*real[i] + imag[i]*imag[i]);
    }
    
    sample_index = 0;  // Prepara para nova coleta
}

const float* fft_get_magnitude(void) {
    return magnitudes;
}

float fft_get_frequency(uint16_t index, float sample_rate) {
    if (index >= FFT_SIZE / 2) return 0.0f;
    return index * (sample_rate / FFT_SIZE);
}

uint16_t fft_find_peak_frequency(uint16_t start_bin, uint16_t end_bin) {
    if (end_bin > FFT_SIZE / 2) end_bin = FFT_SIZE / 2;
    if (start_bin >= end_bin) return 0;
    
    float max_mag = 0;
    uint16_t peak_bin = start_bin;
    
    for (uint16_t i = start_bin; i < end_bin; i++) {
        if (magnitudes[i] > max_mag) {
            max_mag = magnitudes[i];
            peak_bin = i;
        }
    }
    
    return peak_bin;
}

// Converte magnitudes FFT em um JSON simples: [v1, v2, v3, ...]
void fft_format_json(const float* mags, int len, char* buffer, int buffer_size) {
    int offset = 0;
    offset += snprintf(buffer + offset, buffer_size - offset, "[");
    
    for (int i = 2; i <= len; i++) {  // pula os dois primeiros bins (DC e muito baixa freq)
        offset += snprintf(buffer + offset, buffer_size - offset,
                           "%.2f%s", mags[i], (i == len) ? "" : ", ");
    }

    snprintf(buffer + offset, buffer_size - offset, "]");
}