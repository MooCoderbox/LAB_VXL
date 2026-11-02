/*
 * timer.c
 *
 *  Created on: Nov 1, 2026
 *      Author: DELL
 */


#include "main.h"
#include "software_timer.h"

void SoftwareTimer_Init(void)
{
    /* HAL_Init() starts the 1 ms HAL tick. No TIM2-specific setup is needed. */
}

uint32_t SoftwareTimer_GetTick(void)
{
    return HAL_GetTick();
}

void SoftwareTimer_Start(SoftwareTimer *timer, uint32_t duration_ms)
{
    if (timer == 0) {
        return;
    }

    if (duration_ms == 0U) {
        duration_ms = 1U;
    }

    timer->deadline_ms = SoftwareTimer_GetTick() + duration_ms;
    timer->active = 1U;
}

void SoftwareTimer_Stop(SoftwareTimer *timer)
{
    if (timer != 0) {
        timer->active = 0U;
    }
}

bool SoftwareTimer_IsExpired(SoftwareTimer *timer)
{
    uint32_t now;

    if ((timer == 0) || (timer->active == 0U)) {
        return false;
    }

    now = SoftwareTimer_GetTick();
    /* Signed subtraction is safe across the 32-bit HAL tick wraparound. */
    if ((int32_t)(now - timer->deadline_ms) >= 0) {
        timer->active = 0U;
        return true;
    }

    return false;
}

uint32_t SoftwareTimer_RemainingMs(const SoftwareTimer *timer)
{
    uint32_t now;
    int32_t remaining_ms;

    if ((timer == 0) || (timer->active == 0U)) {
        return 0U;
    }

    now = SoftwareTimer_GetTick();
    remaining_ms = (int32_t)(timer->deadline_ms - now);

    if (remaining_ms <= 0) {
        return 0U;
    }

    return (uint32_t)remaining_ms;
}

