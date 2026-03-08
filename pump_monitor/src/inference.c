#include "models\weights.h"
#include "models\fft_norm_params.h"

int neural_network_predict(const float input_raw[INPUT_SIZE]) {
    float input[INPUT_SIZE];
    float hidden[HIDDEN_SIZE];
    float output[OUTPUT_SIZE];

    // === NORMALIZAÇÃO ===
    for (int i = 0; i < INPUT_SIZE; i++) {
        input[i] = (input_raw[i + 2] - fft_means[i]) / fft_scales[i];
    }

    // === CAMADA OCULTA ===
    for (int i = 0; i < HIDDEN_SIZE; i++) {
        hidden[i] = hidden_biases[i];
        for (int j = 0; j < INPUT_SIZE; j++) {
            hidden[i] += hidden_weights[i][j] * input[j];
        }
        // ReLU
        if (hidden[i] < 0) hidden[i] = 0;
    }

    // === CAMADA DE SAÍDA ===
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        output[i] = output_biases[i];
        for (int j = 0; j < HIDDEN_SIZE; j++) {
            output[i] += output_weights[i][j] * hidden[j];
        }
    }

    // === CLASSIFICAÇÃO ===
    int class = 0;
    float max = output[0];
    for (int i = 1; i < OUTPUT_SIZE; i++) {
        if (output[i] > max) {
            max = output[i];
            class = i;
        }
    }
    return class;
}
