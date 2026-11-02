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

/* Variables exposed for live inspection in STM32CubeIDE's Expressions/Watch view. */
typedef struct {
    volatile uint32_t tim2_interrupt_count;
    volatile uint32_t system_tick_ms;
    volatile uint32_t software_timer_expiry_count;
} SoftwareTimerDebug;

extern volatile SoftwareTimerDebug g_software_timer_debug;

/* Call once before starting TIM2. Clears all software-timer channels. */
void SoftwareTimer_Init(void);

/* Call ONLY from the TIM2 update interrupt callback, once per TIM2 update. */
void SoftwareTimer_RunISR(void);

/* Start/restart a one-shot timer. Duration is expressed in milliseconds. */
void SoftwareTimer_Start(SoftwareTimerId id, uint32_t duration_ms);

/* Stop a timer and clear any pending expired flag. */
void SoftwareTimer_Stop(SoftwareTimerId id);

/*
 * Returns 1 once when a timer expires, then clears its expired flag.
 * This is intended to be polled from the main loop, not from an ISR.
 */
uint8_t SoftwareTimer_IsExpired(SoftwareTimerId id);

/* Snapshot remaining time in ms. Returns 0 if stopped or already expired. */
uint32_t SoftwareTimer_RemainingMs(SoftwareTimerId id);

/* Read the TIM2-driven application clock (milliseconds since initialization). */
uint32_t SoftwareTimer_GetTickMs(void);

#endif /* INC_SOFTWARE_TIMER_H_ */
