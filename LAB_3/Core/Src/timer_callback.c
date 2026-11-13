#include "main.h"
#include "config.h"
#include "software_timer.h"
#include "button.h"
#include "global.h"
/*
 * TIM2 is the only hardware timer used as the application's time base.
 * This callback is called by HAL from TIM2's interrupt handler.
 * Keep it short: update software counters and sample buttons only.
 * Do not run FSM code, multiplex displays, or call HAL_Delay() here.
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2) {
        // static uint32_t button_scan_divider = 0U;

        software_timer_run();

        /* TIM2 tick is 10 ms; read active-low buttons every 10 ms. */
        getKeyInput();
    }
}
