#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

/*
 * Application timing configuration.
 * TIM2 must generate one update interrupt every TIM2_TICK_PERIOD_MS.
 * All software timers use this tick; they do not use HAL_GetTick().
 */
#define TIM2_TICK_PERIOD_MS             1U

/* Buttons are sampled slower than the 1 ms timer base to reduce CPU work. */
#define BUTTON_SCAN_PERIOD_MS           10U
#define BUTTON_DEBOUNCE_SAMPLES         2U
#define BUTTON_SCAN_DIVIDER_TICKS       \
    ((BUTTON_SCAN_PERIOD_MS + TIM2_TICK_PERIOD_MS - 1U) / TIM2_TICK_PERIOD_MS)

/* Default traffic-light durations. RED = GREEN + YELLOW for this 4-state FSM. */
#define DEFAULT_GREEN_SECONDS           3U
#define DEFAULT_YELLOW_SECONDS          2U
#define DEFAULT_RED_SECONDS             (DEFAULT_GREEN_SECONDS + DEFAULT_YELLOW_SECONDS)

/* Manual-mode timing. 2 Hz = 250 ms ON + 250 ms OFF. */
#define MANUAL_BLINK_TOGGLE_MS           250U
#define MANUAL_IDLE_TIMEOUT_MS           10000U

/* Multiplex one 7-segment digit every 2 ms (4 digits = 125 Hz full refresh). */
#define SEVENSEG_SCAN_INTERVAL_MS        2U

/* Set to 1 if digit-enable pins EN0..EN3 are active-low. */
#define SEVENSEG_ENABLE_ACTIVE_LOW       1

#endif /* INC_CONFIG_H_ */
