#include "inc/buzzer.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"

#define BUZZER 10  // GPIO do buzzer
#define BUZZER_FREQ_HZ 1000
#define BUZZ_ON_TIME_MS 150
#define BUZZ_OFF_TIME_MS 150

static uint slice;
static bool buzzer_on = false;
static uint8_t remaining_toggles = 0; // Cada beep = 2 toggles (on/off)
static absolute_time_t next_toggle_time;

void buzzer_init(void) {
    gpio_set_function(BUZZER, GPIO_FUNC_PWM);
    slice = pwm_gpio_to_slice_num(BUZZER);

    uint32_t clock_hz = 125000000;
    uint16_t wrap = clock_hz / BUZZER_FREQ_HZ - 1;

    pwm_config config = pwm_get_default_config();
    pwm_config_set_wrap(&config, wrap);
    pwm_init(slice, &config, true);
    pwm_set_gpio_level(BUZZER, 0);
}

void buzzer_trigger_beeps(uint8_t total_beeps) {
    remaining_toggles = total_beeps * 2; // ON + OFF por beep
    buzzer_on = false;
    next_toggle_time = get_absolute_time(); // Começa imediatamente
}

void buzzer_update(void) {
    if (remaining_toggles == 0) return;

    if (absolute_time_diff_us(get_absolute_time(), next_toggle_time) <= 0) {
        buzzer_on = !buzzer_on;
        pwm_set_gpio_level(BUZZER, buzzer_on ? 6250 : 0);
        next_toggle_time = delayed_by_ms(get_absolute_time(), buzzer_on ? BUZZ_ON_TIME_MS : BUZZ_OFF_TIME_MS);
        remaining_toggles--;
    }
}
