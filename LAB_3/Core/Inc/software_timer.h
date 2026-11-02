/*
 * timer.h
 *
 *  Created on: Nov 1, 2026
 *      Author: DELL
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#include <stdint.h>
#include <stdbool.h>

/*
 * Millisecond software timer backed by HAL_GetTick() (the STM32 HAL SysTick).
 * It is deliberately independent of TIM2's period. Therefore changing TIM2
 * from 10 ms to 1 ms or 100 ms does not change the 2 Hz LED blink or timeouts.
 */
typedef struct {
    uint32_t deadline_ms;
    uint8_t active;
} SoftwareTimer;

void SoftwareTimer_Init(void);
uint32_t SoftwareTimer_GetTick(void);

void SoftwareTimer_Start(SoftwareTimer *timer, uint32_t duration_ms);
void SoftwareTimer_Stop(SoftwareTimer *timer);
bool SoftwareTimer_IsExpired(SoftwareTimer *timer);
uint32_t SoftwareTimer_RemainingMs(const SoftwareTimer *timer);

#endif /* INC_SOFTWARE_TIMER_H_ */

