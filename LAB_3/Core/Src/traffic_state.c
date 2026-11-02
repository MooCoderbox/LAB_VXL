/*
 * traffic_state.c
 *
 *  Created on: Nov 2, 2026
 *      Author: DELL
 */


#include "traffic_state.h"
#include "fsm_automatic.h"
#include "fsm_manual.h"

/* Define the global state exactly once. */
TrafficState status = INIT;

void FSM_TransitionTo(TrafficState next_state)
{
    /* Avoid unnecessary re-entry into the same state. */
    if (status == next_state) {
        return;
    }

    status = next_state;

    /*
     * Execute state-entry actions.
     * Each module initializes its own output and timer.
     */
    switch (status) {

    case AUTO_RED_GREEN:
    case AUTO_RED_YELLOW:
    case AUTO_GREEN_RED:
    case AUTO_YELLOW_RED:
        FSM_Automatic_Enter(status);
        break;

    case MAN_RED:
    case MAN_YELLOW:
    case MAN_GREEN:
        FSM_Manual_Enter(status);
        break;

    default:
        break;
    }
}
