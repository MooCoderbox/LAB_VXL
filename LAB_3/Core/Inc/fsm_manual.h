#ifndef INC_FSM_MANUAL_H_
#define INC_FSM_MANUAL_H_

#include <stdint.h>
#include "traffic_state.h"

/*
 * Debug values for STM32CubeIDE Expressions/Live Expressions.
 * In MAN_RED with MANUAL_BLINK_TOGGLE_MS = 500:
 *   blink_toggle_count increases about once every 500 ms;
 *   last_blink_toggle_tick_ms advances about 500 ms each time;
 *   traffic_output_update_count increases only when GPIO output is written.
 */
typedef struct {
    volatile uint32_t blink_toggle_count;
    volatile uint32_t traffic_output_update_count;
    volatile uint32_t last_blink_toggle_tick_ms;
    volatile uint8_t last_output_blink_state;
    volatile uint8_t last_output_mode;
} FSMManualDebug;

extern volatile FSMManualDebug g_fsm_manual_debug;

/* Called by the central dispatcher when entering/leaving a MAN state. */
void FSM_Manual_Enter(TrafficState state);
void FSM_Manual_Exit(void);

/* Processes BTN1/BTN2/BTN3 and manual-mode timer flags. */
void FSM_Manual_Update(uint8_t button1_event,
                       uint8_t button2_event,
                       uint8_t button3_event);

/* Returns 2, 3, or 4 according to the current shared status. */
uint8_t FSM_Manual_GetMode(void);

#endif /* INC_FSM_MANUAL_H_ */
