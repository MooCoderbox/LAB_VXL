/*
 * traffic_state.h
 *
 *  Created on: Nov 2, 2026
 *      Author: DELL
 */

#ifndef INC_TRAFFIC_STATE_H_
#define INC_TRAFFIC_STATE_H_

typedef enum {
    INIT,

    /* Automatic states */
    AUTO_RED_GREEN,
    AUTO_RED_YELLOW,
    AUTO_GREEN_RED,
    AUTO_YELLOW_RED,

    /* Manual states */
    MAN_RED,
    MAN_YELLOW,
    MAN_GREEN

} TrafficState;

/* One shared state variable for the entire application. */
extern TrafficState status;

/* All state changes should go through this function. */
void FSM_TransitionTo(TrafficState next_state);

#endif
