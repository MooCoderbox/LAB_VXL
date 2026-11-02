#ifndef INC_FSM_SETTING_H_
#define INC_FSM_SETTING_H_

#include <stdint.h>

typedef enum {
    SETTING_RED = 0,
    SETTING_YELLOW,
    SETTING_GREEN
} SettingColor;

void FSM_Setting_Init(void);
void FSM_Setting_BeginEdit(SettingColor color);
void FSM_Setting_Increment(void);
uint8_t FSM_Setting_Commit(void);
uint8_t FSM_Setting_GetEditValue(void);
uint8_t FSM_Setting_GetValue(SettingColor color);
void FSM_Setting_GetAll(uint8_t *red, uint8_t *yellow, uint8_t *green);

#endif /* INC_FSM_SETTING_H_ */
