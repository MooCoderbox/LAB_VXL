#ifndef INC_FSM_SETTING_H_
#define INC_FSM_SETTING_H_

#include <stdint.h>
#include "global.h"

typedef enum {
    SETTING_RED = 0,
    SETTING_YELLOW,
    SETTING_GREEN
} SettingColor;

extern uint8_t red_seconds;
extern uint8_t yellow_seconds;
extern uint8_t green_seconds;

uint8_t getTimeOfSettingColor(SettingColor t);
void fsm_setting_load(uint8_t initial_value);
uint8_t fsm_setting_get_value(void);
void fsm_setting_run(void);

#endif /* INC_FSM_SETTING_H_ */
