/*
 * config.h
 *
 *  Created on: Nov 2, 2026
 *      Author: DELL
 */

#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

#define TIM_PRESCALER       7999
#define TIM_PERIOD          9

/* Fixed system constant: TIM2 input clock (8 MHz with the default HSI).
 * Change it only if you change the clock tree in CubeMX. */
#define TIM_CLOCK_HZ        8000000ULL

/* ---- derived at compile time, never edit ---- */
#define TIMER_CYCLE_US      (((TIM_PRESCALER) + 1ULL) * ((TIM_PERIOD) + 1ULL) \
                             * 1000000ULL / (TIM_CLOCK_HZ))
#define TIMER_CYCLE_MS      (TIMER_CYCLE_US / 1000ULL)
#define MS_TO_TICKS(ms)     (((ms) * 1000ULL + TIMER_CYCLE_US / 2ULL) / TIMER_CYCLE_US)

/* Periods used by the application */
#define SEC_MS              1000    /* countdown tick           */
#define BLINK_MS            250     /* toggle period, 2 Hz blink */
#define TIMEOUT_MS          10000   /* manual mode timeout      */
#define SCAN_MS             10      /* 7-segment multiplex step */



#endif /* INC_CONFIG_H_ */
