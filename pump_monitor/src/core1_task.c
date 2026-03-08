#include "inc/core1_task.h"
#include "pico/multicore.h"
#include "inc\fft.h"
#include "inc\inference.h"
#include "inc\config.h"
#include "disp\display.h"
#include <stdio.h>
#include "hardware/gpio.h"


void main_core1_tasks(){
    while(true){
        // aguarda sinal do Core 0
        multicore_fifo_pop_blocking(); // bloqueia até receber sinal

        // Processa fft
        fft_compute();
        const float* mags = fft_get_magnitude(); 
        

        // Inferência
        int classe = neural_network_predict(mags);
        

        // envia retorno para core 0
        multicore_fifo_push_blocking(classe);
    }
}