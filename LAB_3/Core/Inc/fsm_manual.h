/*
 * traffic_fsm.h
 *
 *  Created on: Nov 2, 2026
 *      Author: DELL
 */

#ifndef INC_FSM_MANUAL_H_
#define INC_FSM_MANUAL_H_

#include <stdint.h>

void FSM_Manual_Enter(void);
/*
 * Pass debounced one-shot button events from main().
 * Returns 1 when manual mode should end (BTN1 in mode 4 or 10 s timeout).
 */
uint8_t FSM_Manual_Update(uint8_t button1_event,
                          uint8_t button2_event,
                          uint8_t button3_event);
uint8_t FSM_Manual_GetMode(void);

#endif /* INC_FSM_MANUAL_H_ */

