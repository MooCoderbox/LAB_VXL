#ifndef INC_TRAFFIC_STATE_H_
#define INC_TRAFFIC_STATE_H_

#include <stdint.h>

/*
 * One application-wide state machine.
 * Road A = East-West; Road B = North-South.
 */
typedef enum {
    TRAFFIC_STATE_INIT = 0,

    /* Automatic mode: first color belongs to Road A, second to Road B. */
    AUTO_RED_GREEN,
    AUTO_RED_YELLOW,
    AUTO_GREEN_RED,
    AUTO_YELLOW_RED,

    /* Manual setting modes. */
    MAN_RED,
    MAN_YELLOW,
    MAN_GREEN
} TrafficState;

/*
 * The single shared state variable.
 * Read it from any module, but change it only through TrafficFSM_TransitionTo().
 */
extern TrafficState status;

/* Initialize the dispatcher while all software timers are already initialized. */
void TrafficFSM_Init(void);

/*
 * Centralized state transition. Runs exit/entry actions and starts the timers
 * associated with the destination state.
 */
void TrafficFSM_TransitionTo(TrafficState next_state);

/* Dispatch one foreground update to the active Automatic or Manual FSM. */
void TrafficFSM_Update(uint8_t button1_event,
                       uint8_t button2_event,
                       uint8_t button3_event);

#endif /* INC_TRAFFIC_STATE_H_ */
