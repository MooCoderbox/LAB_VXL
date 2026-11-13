/*
 * global.h
 *
 *  Created on: Nov 3, 2026
 *      Author: DELL
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include "config.h"
#include "software_timer.h"
#include "button.h"
#include "led_display.h"
#include "fsm_setting.h"

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
    MAN_GREEN,
} status;

status status_fsm;



#endif /* INC_GLOBAL_H_ */
