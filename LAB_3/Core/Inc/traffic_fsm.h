/*
 * traffic_fsm.h
 *
 *  Created on: Nov 2, 2026
 *      Author: DELL
 */

#ifndef INC_TRAFFIC_FSM_H_
#define INC_TRAFFIC_FSM_H_

void fsm_init(void);    // state = INIT
void fsm_run(void);     // one step, called every main loop

#endif /* INC_TRAFFIC_FSM_H_ */
