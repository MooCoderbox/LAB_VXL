#include "software_timer.h"
#include "config.h"


/* These variables are shared between the main context and the TIM2 ISR. */
static volatile uint32_t timer_counter[SOFT_TIMER_COUNT];
static volatile uint8_t timer_flag[SOFT_TIMER_COUNT];

void software_timer_init(void)
{
    uint32_t i;

    for (i = 0U; i < SOFT_TIMER_COUNT; i++)
    {
        timer_counter[i] = 0U;
        timer_flag[i] = 0U;
    }
}

void software_timer_set(SoftwareTimerId timer_id, uint32_t duration_ms)
{
    uint32_t ticks;

    if (timer_id >= SOFT_TIMER_COUNT) {
        return;
    }

    ticks = duration_ms / TIM2_TICK_PERIOD_MS;
    if ((duration_ms % TIM2_TICK_PERIOD_MS) != 0U) {
        ticks++;
    }

    timer_counter[timer_id] = ticks;
    timer_flag[timer_id] = (ticks == 0U) ? 1U : 0U;
}

uint8_t software_timer_is_expired(SoftwareTimerId timer_id)
{
    if (timer_id >= SOFT_TIMER_COUNT)
    {
        return 0U;
    }
    // Auto reset flag prevent ERROR
    if (timer_flag[timer_id]) {
        timer_flag[timer_id] = 0;
        return 1;
    }
    return timer_flag[timer_id];
}

uint32_t software_timer_get_remaining_ms(SoftwareTimerId timer_id)
{
    if (timer_id >= SOFT_TIMER_COUNT) {
        return 0U;
    }
    return timer_counter[timer_id] * TIM2_TICK_PERIOD_MS;
}

void software_timer_run(void)
{
    SoftwareTimerId i;

    for (i = 0U; i < SOFT_TIMER_COUNT; i++)
    {
        if (timer_counter[i] > 0U)
        {
            timer_counter[i]--;

            if (timer_counter[i] == 0U)
            {
                timer_flag[i] = 1U;
            }
        }
    }
}


