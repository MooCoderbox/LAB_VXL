#include "fsm_manual.h"
#include "software_timer.h"
#include "fsm_setting.h"
#include "led_display.h"
#include "config.h"

static uint8_t manual_mode = 2U; /* 2 = red, 3 = yellow, 4 = green. */
static uint8_t manual_active;
static uint8_t blink_on;

static SettingColor setting_color_for_mode(void)
{
    switch (manual_mode) {
    case 2U: return SETTING_RED;
    case 3U: return SETTING_YELLOW;
    case 4U: return SETTING_GREEN;
    default: return SETTING_RED;
    }
}

static TrafficColor traffic_color_for_mode(void)
{
    switch (manual_mode) {
    case 2U: return TRAFFIC_RED;
    case 3U: return TRAFFIC_YELLOW;
    case 4U: return TRAFFIC_GREEN;
    default: return TRAFFIC_OFF;
    }
}

static void restart_inactivity_timer(void)
{
    SoftwareTimer_Start(SOFT_TIMER_MANUAL_TIMEOUT, MANUAL_IDLE_TIMEOUT_MS);
}

void FSM_Manual_Enter(void)
{
    manual_mode = 2U;
    manual_active = 1U;
    blink_on = 0U;

    FSM_Setting_BeginEdit(SETTING_RED);
    SoftwareTimer_Start(SOFT_TIMER_MANUAL_BLINK, MANUAL_BLINK_TOGGLE_MS);
    restart_inactivity_timer();

    LED_Display_SetTraffic(TRAFFIC_OFF, TRAFFIC_OFF);
    LED_Display_SetNumbers(FSM_Setting_GetEditValue(), manual_mode);
}

uint8_t FSM_Manual_Update(uint8_t button1_event,
                          uint8_t button2_event,
                          uint8_t button3_event)
{
    if (manual_active == 0U) {
        return 1U;
    }

    if (button1_event != 0U) {
        restart_inactivity_timer();

        if (manual_mode == 4U) {
            manual_active = 0U;
            SoftwareTimer_Stop(SOFT_TIMER_MANUAL_BLINK);
            SoftwareTimer_Stop(SOFT_TIMER_MANUAL_TIMEOUT);
            LED_Display_SetTraffic(TRAFFIC_OFF, TRAFFIC_OFF);
            return 1U;
        }

        manual_mode++;
        FSM_Setting_BeginEdit(setting_color_for_mode());
    }

    if (button2_event != 0U) {
        FSM_Setting_Increment();
        restart_inactivity_timer();
    }

    if (button3_event != 0U) {
        (void)FSM_Setting_Commit();
        /* Keep the selected mode active and show the committed value. */
        FSM_Setting_BeginEdit(setting_color_for_mode());
        restart_inactivity_timer();
    }

    if (SoftwareTimer_IsExpired(SOFT_TIMER_MANUAL_TIMEOUT) != 0U) {
        manual_active = 0U;
        SoftwareTimer_Stop(SOFT_TIMER_MANUAL_BLINK);
        LED_Display_SetTraffic(TRAFFIC_OFF, TRAFFIC_OFF);
        return 1U;
    }

    if (SoftwareTimer_IsExpired(SOFT_TIMER_MANUAL_BLINK) != 0U) {
        blink_on = (uint8_t)!blink_on;
        SoftwareTimer_Start(SOFT_TIMER_MANUAL_BLINK, MANUAL_BLINK_TOGGLE_MS);
    }

    /* Both roads blink together; no delay is used in this state machine. */
    LED_Display_BlinkBothRoads(traffic_color_for_mode(), blink_on);
    LED_Display_SetNumbers(FSM_Setting_GetEditValue(), manual_mode);

    return 0U;
}

uint8_t FSM_Manual_GetMode(void)
{
    return manual_mode;
}
