#ifndef INFERENCE_H
#define INFERENCE_H

#include <stdint.h>

#define INPUT_SIZE 10
#define HIDDEN_SIZE 8
#define OUTPUT_SIZE 4

// Protótipo da função de inferência
// Recebe vetor de entrada com INPUT_SIZE floats e retorna a classe prevista (0..3)
int neural_network_predict(const float input[INPUT_SIZE]);
void normalizar_input(float input[INPUT_SIZE]);

#endif // INFERENCE_H
