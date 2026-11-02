/*
 * timer.h
 *
 *  Created on: Nov 1, 2026
 *      Author: DELL
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

/* One software timer per job. Add new ones before NO_OF_TIMERS. */
enum {
    TIMER_SEC,
    TIMER_BLINK,
    TIMER_TIMEOUT,
    TIMER_SCAN,
    NO_OF_TIMERS
};

void timer_init(void);                 /* start TIM2 with interrupt          */
void set_timer(int idx, int ms);       /* (re)start a timer, clears its flag */
int  is_timer_expired(int idx);        /* 1 when the time has elapsed        */
void timer_run(void);                  /* called from the TIM2 interrupt     */

#endif /* INC_TIMER_H_ */
