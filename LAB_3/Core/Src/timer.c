/*
 * timer.c
 *
 *  Created on: Nov 1, 2026
 *      Author: DELL
 */


#include "main.h"
#include "config.h"
#include "timer.h"
#include "input_reading.h"

extern TIM_HandleTypeDef htim2;

/* written by the interrupt, read by the main loop */
static volatile int     timer_counter[NO_OF_TIMERS];
static volatile uint8_t timer_flag[NO_OF_TIMERS];

void timer_init(void) {
    HAL_TIM_Base_Start_IT(&htim2);
}

void set_timer(int idx, int ms) {
    int ticks;
    if (idx < 0 || idx >= NO_OF_TIMERS) return;
    ticks = MS_TO_TICKS(ms);
    if (ticks < 1) ticks = 1;          /* never create a timer that cannot fire */

    __disable_irq();                   /* counter and flag must change together */
    timer_counter[idx] = ticks;
    timer_flag[idx] = 0;
    __enable_irq();
}

int is_timer_expired(int idx) {
    if (idx < 0 || idx >= NO_OF_TIMERS) return 0;
    return timer_flag[idx];
}

void timer_run(void) {
    for (int i = 0; i < NO_OF_TIMERS; i++) {
        if (timer_counter[i] > 0) {
            timer_counter[i]--;
            if (timer_counter[i] == 0) {
                timer_flag[i] = 1;
            }
        }
    }
}

/* Called by HAL every time TIM2 overflows (every TIMER_CYCLE_MS) */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
        timer_run();
        // button_reading();
    }
}
