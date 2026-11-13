#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#include <stdint.h>

/* Each application function owns one independent software-timer channel. */
typedef enum {
    SOFT_TIMER_AUTO_PHASE = 0,       /* Time remaining in the current traffic phase. */
    SOFT_TIMER_MANUAL_BLINK,         /* 250 ms ON/OFF interval in modes 2..4. */
    SOFT_TIMER_MANUAL_TIMEOUT,       /* Return to automatic mode after 10 s idle. */
    SOFT_TIMER_DISPLAY_SCAN,         /* Multiplex refresh interval for one digit. */
    SOFT_TIMER_COUNT
} SoftwareTimerId;


/**
 * Initialize all software timers.
 * Call this before starting the TIM2 interrupt.
 */
void software_timer_init(void);

/**
 * Start/restart a one-shot software timer.
 *
 * timer_id:     Timer index in the range [0, SOFT_TIMER_COUNT - 1].
 * duration_ms:  Requested duration in milliseconds.
 *
 * A duration of 0 expires immediately. Durations that are not an exact
 * multiple of TIM2_TICK_PERIOD_MS are rounded up to the next tick.
 */
void software_timer_set(SoftwareTimerId timer_id, uint32_t duration_ms);

/**
 * Return 1 when the timer has expired, otherwise return 0.
 * The expired flag remains set until the timer is restarted.
 */
uint8_t software_timer_is_expired(SoftwareTimerId timer_id);

/**
 * Return the timer's remaining duration in milliseconds, rounded down to
 * the timer tick. Returns 0 for an invalid timer ID or an expired timer.
 */
uint32_t software_timer_get_remaining_ms(SoftwareTimerId timer_id);

/**
 * Update all software timers by one tick.
 * Call this function once from the TIM2 period-elapsed callback.
 */
void software_timer_run(void);

#endif /* INC_SOFTWARE_TIMER_H_ */
