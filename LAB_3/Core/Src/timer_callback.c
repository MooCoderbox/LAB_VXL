/*
 * timer_callback.c
 *
 *  Created on: Nov 2, 2026
 *      Author: DELL
 */


#include "main.h"
#include "button.h"

/*
 * Use this callback if the project does not already define it elsewhere.
 * Keep TIM2 at 10 ms for the button debounce sampling used by this lab.
 * The software timers and 2 Hz blink use HAL_GetTick(), not the TIM2 period.
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2) {
        Button_ScanISR();
    }
}
