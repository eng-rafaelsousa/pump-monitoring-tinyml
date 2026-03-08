#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>
#include <stdbool.h>

void buzzer_init(void);
void buzzer_trigger_beeps(uint8_t total_beeps);
void buzzer_update(void);

#endif
