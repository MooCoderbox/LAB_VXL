#include "main.h"
#include "config.h"
#include "software_timer.h"

/*
 * Countdown state is shared by the TIM2 ISR and the foreground (main loop).
 * 'volatile' prevents the compiler from caching values modified by the ISR.
 */
static volatile uint32_t timer_counter_ticks[SOFT_TIMER_COUNT];
static volatile uint8_t timer_expired_flag[SOFT_TIMER_COUNT];
static volatile uint32_t system_tick_ms;

volatile SoftwareTimerDebug g_software_timer_debug;

/* Save/restore PRIMASK so a caller's previous interrupt state is preserved. */
static uint32_t irq_lock(void)
{
    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    return primask;
}

static void irq_unlock(uint32_t primask)
{
    __set_PRIMASK(primask);
}

/* Convert milliseconds to whole TIM2 ticks, rounding up (never early). */
static uint32_t milliseconds_to_ticks(uint32_t duration_ms)
{
    uint32_t ticks;

    if (duration_ms == 0U) {
        return 1U; /* A zero-duration timer expires on the next TIM2 interrupt. */
    }

    ticks = duration_ms / TIM2_TICK_PERIOD_MS;
    if ((duration_ms % TIM2_TICK_PERIOD_MS) != 0U) {
        ticks++;
    }

    return (ticks == 0U) ? 1U : ticks;
}

void SoftwareTimer_Init(void)
{
    uint32_t i;

    /* Call before HAL_TIM_Base_Start_IT(&htim2), while the timer ISR is inactive. */
    for (i = 0U; i < (uint32_t)SOFT_TIMER_COUNT; i++) {
        timer_counter_ticks[i] = 0U;
        timer_expired_flag[i] = 0U;
    }

    system_tick_ms = 0U;
    g_software_timer_debug.tim2_interrupt_count = 0U;
    g_software_timer_debug.system_tick_ms = 0U;
    g_software_timer_debug.software_timer_expiry_count = 0U;
}

void SoftwareTimer_RunISR(void)
{
    uint32_t i;

    /* This function is deliberately short: no HAL calls and no GPIO access here. */
    g_software_timer_debug.tim2_interrupt_count++;
    system_tick_ms += TIM2_TICK_PERIOD_MS;
    g_software_timer_debug.system_tick_ms = system_tick_ms;

    for (i = 0U; i < (uint32_t)SOFT_TIMER_COUNT; i++) {
        if (timer_counter_ticks[i] > 0U) {
            timer_counter_ticks[i]--;

            if (timer_counter_ticks[i] == 0U) {
                /* Leave the event pending until the corresponding main-loop code reads it. */
                timer_expired_flag[i] = 1U;
                g_software_timer_debug.software_timer_expiry_count++;
            }
        }
    }
}

void SoftwareTimer_Start(SoftwareTimerId id, uint32_t duration_ms)
{
    uint32_t primask;
    uint32_t ticks;

    if ((uint32_t)id >= (uint32_t)SOFT_TIMER_COUNT) {
        return; /* Invalid timer ID: ignore safely. */
    }

    ticks = milliseconds_to_ticks(duration_ms);

    /* The ISR may decrement this channel; update counter and flag atomically. */
    primask = irq_lock();
    timer_counter_ticks[id] = ticks;
    timer_expired_flag[id] = 0U;
    irq_unlock(primask);
}

void SoftwareTimer_Stop(SoftwareTimerId id)
{
    uint32_t primask;

    if ((uint32_t)id >= (uint32_t)SOFT_TIMER_COUNT) {
        return;
    }

    primask = irq_lock();
    timer_counter_ticks[id] = 0U;
    timer_expired_flag[id] = 0U;
    irq_unlock(primask);
}

uint8_t SoftwareTimer_IsExpired(SoftwareTimerId id)
{
    uint32_t primask;
    uint8_t expired;

    if ((uint32_t)id >= (uint32_t)SOFT_TIMER_COUNT) {
        return 0U;
    }

    /* Read-and-clear is atomic, so an ISR event cannot be lost during polling. */
    primask = irq_lock();
    expired = timer_expired_flag[id];
    timer_expired_flag[id] = 0U;
    irq_unlock(primask);

    return expired;
}

uint32_t SoftwareTimer_RemainingMs(SoftwareTimerId id)
{
    uint32_t primask;
    uint32_t remaining_ticks;

    if ((uint32_t)id >= (uint32_t)SOFT_TIMER_COUNT) {
        return 0U;
    }

    /* Take a consistent snapshot before converting ticks back to milliseconds. */
    primask = irq_lock();
    remaining_ticks = timer_counter_ticks[id];
    irq_unlock(primask);

    return remaining_ticks * TIM2_TICK_PERIOD_MS;
}

uint32_t SoftwareTimer_GetTickMs(void)
{
    uint32_t primask;
    uint32_t tick;

    /* 32-bit reads are naturally aligned, but lock for a clear ISR/foreground contract. */
    primask = irq_lock();
    tick = system_tick_ms;
    irq_unlock(primask);

    return tick;
}
