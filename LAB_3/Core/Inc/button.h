/*
 * input_processing.h
 *
 *  Created on: Nov 1, 2026
 *      Author: DELL
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include <stdint.h>

typedef enum {
    BUTTON_MODE = 0,       /* BTN1: select/change mode */
    BUTTON_INCREMENT,      /* BTN2: increase selected duration */
    BUTTON_CONFIRM,        /* BTN3: save selected duration */
    BUTTON_COUNT
} ButtonId;

void Button_Init(void);
/* Call this function from the TIM2 interrupt callback every 10 ms. */
void Button_ScanISR(void);
/* Returns 1 once for each debounced press; then clears the event. */
uint8_t Button_GetPressedEvent(ButtonId id);

#endif /* INC_BUTTON_H_ */
