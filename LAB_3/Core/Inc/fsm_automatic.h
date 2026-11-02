#ifndef INC_FSM_AUTOMATIC_H_
#define INC_FSM_AUTOMATIC_H_

#include <stdint.h>

/* Call Enter() at startup and whenever the application returns to mode 1. */
void FSM_Automatic_Enter(void);
void FSM_Automatic_Run(void);
void FSM_Automatic_GetRemaining(uint8_t *road_a_seconds,
                                uint8_t *road_b_seconds);

#endif /* INC_FSM_AUTOMATIC_H_ */
