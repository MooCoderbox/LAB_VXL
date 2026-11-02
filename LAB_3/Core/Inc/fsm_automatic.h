#ifndef INC_FSM_AUTOMATIC_H_
#define INC_FSM_AUTOMATIC_H_

#include <stdint.h>
#include "traffic_state.h"

/* Called by the central dispatcher when entering/leaving an AUTO state. */
void FSM_Automatic_Enter(TrafficState state);
void FSM_Automatic_Exit(void);

/* Processes BTN1 and the automatic phase timer while status is an AUTO state. */
void FSM_Automatic_Update(uint8_t button1_event);

/* Read the remaining time shown on the two road displays. */
void FSM_Automatic_GetRemaining(uint8_t *road_a_seconds,
                                uint8_t *road_b_seconds);

#endif /* INC_FSM_AUTOMATIC_H_ */
