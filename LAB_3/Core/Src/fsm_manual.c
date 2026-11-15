#include "fsm_manual.h"

#include "global.h"
#include "software_timer.h"
#include "fsm_setting.h"
#include "led_display.h"
#include "button.h"
#include "config.h"

void fsm_manual_run(void)
{
    switch (status_fsm) {
    case MAN_RED: /* Mode 2: blink RED, switch to Mode 3 on Button 1 */
        if (isButton1Pressed()) {
            fsm_setting_load(getTimeOfSettingColor(SETTING_YELLOW));
            LED_Display_BlinkStart(TRAFFIC_YELLOW);
            status_fsm = MAN_YELLOW;
            break;
        }
        task_MAN_RED();
        LED_Display_SetNumbers(fsm_setting_get_value(), 2U);
        break;

    case MAN_YELLOW: /* Mode 3: blink AMBER, switch to Mode 4 on Button 1 */
        if (isButton1Pressed()) {
            fsm_setting_load(getTimeOfSettingColor(SETTING_GREEN));
            LED_Display_BlinkStart(TRAFFIC_GREEN);
            status_fsm = MAN_GREEN;
            break;
        }
        task_MAN_YELLOW();
        LED_Display_SetNumbers(fsm_setting_get_value(), 3U);
        break;

    case MAN_GREEN: /* Mode 4: blink GREEN, return to Mode 1 on Button 1 */
        if (isButton1Pressed()) {
            status_fsm = TRAFFIC_STATE_INIT;
            break;
        }
        task_MAN_GREEN();
        LED_Display_SetNumbers(fsm_setting_get_value(), 4U);
        break;

    default:
        break;
    }
}



//static SettingColor setting_color_for_state(TrafficState state)
//{
//    switch (state) {
//    case MAN_RED:    return SETTING_RED;
//    case MAN_YELLOW: return SETTING_YELLOW;
//    case MAN_GREEN:  return SETTING_GREEN;
//    default:         return SETTING_RED;
//    }
//}
//
//static TrafficColor traffic_color_for_state(TrafficState state)
//{
//    switch (state) {
//    case MAN_RED:    return TRAFFIC_RED;
//    case MAN_YELLOW: return TRAFFIC_YELLOW;
//    case MAN_GREEN:  return TRAFFIC_GREEN;
//    default:         return TRAFFIC_OFF;
//    }
//}
//
//static void restart_inactivity_timer(void)
//{
//    SoftwareTimer_Start(SOFT_TIMER_MANUAL_TIMEOUT, MANUAL_IDLE_TIMEOUT_MS);
//}
//
///*
// * Apply the selected color to both roads only when a blink edge or state entry
// * changes the requested output. This prevents brief OFF pulses in the superloop.
// */
//static void apply_manual_traffic_output(void)
//{
//    LED_Display_BlinkBothRoads(traffic_color_for_state(status), blink_on);
//
//    g_fsm_manual_debug.traffic_output_update_count++;
//    g_fsm_manual_debug.last_output_blink_state = blink_on;
//    g_fsm_manual_debug.last_output_mode = mode_number_for_state(status);
//
//    traffic_output_dirty = 0U;
//}
//// button set flag
//
//static void update_manual_display(void)
//{
//    LED_Display_SetNumbers(FSM_Setting_GetEditValue(),
//                           mode_number_for_state(status));
//}
//
//void FSM_Manual_Enter(TrafficState state)
//{
//    if (is_manual_state(state) == 0U) {
//        return;
//    }
//
//    /* Every MAN state starts with a known OFF phase and a fresh edit buffer. */
//    blink_on = 0U;
//    traffic_output_dirty = 1U;
//
//    g_fsm_manual_debug.blink_toggle_count = 0U;
//    g_fsm_manual_debug.traffic_output_update_count = 0U;
//    g_fsm_manual_debug.last_blink_toggle_tick_ms = SoftwareTimer_GetTickMs();
//    g_fsm_manual_debug.last_output_blink_state = 0U;
//    g_fsm_manual_debug.last_output_mode = mode_number_for_state(state);
//
//    FSM_Setting_BeginEdit(setting_color_for_state(state));
//
//    SoftwareTimer_Start(SOFT_TIMER_MANUAL_BLINK, MANUAL_BLINK_TOGGLE_MS);
//    restart_inactivity_timer();
//
//    /* Apply OFF once on entry; the first ON edge occurs after blink interval. */
//    apply_manual_traffic_output();
//    update_manual_display();
//}
//
//void FSM_Manual_Exit(void)
//{
//    /* Do not turn the lamps OFF here: the destination state writes its own pattern. */
//    SoftwareTimer_Stop(SOFT_TIMER_MANUAL_BLINK);
//    SoftwareTimer_Stop(SOFT_TIMER_MANUAL_TIMEOUT);
//    traffic_output_dirty = 0U;
//}
//
//void FSM_Manual_Update(uint8_t button1_event,
//                       uint8_t button2_event,
//                       uint8_t button3_event)
//{
//    TrafficState next_state;
//
//    if (is_manual_state(status) == 0U) {
//        return;
//    }
//
//    /* BTN2 increments the currently selected value. */
//    if (button2_event != 0U) {
//        FSM_Setting_Increment();
//        restart_inactivity_timer();
//    }
//
//    /* BTN3 commits RED/YELLOW but returns to AUTO after confirming GREEN. */
//    if (button3_event != 0U) {
//        (void)FSM_Setting_Commit();
//        FSM_Setting_BeginEdit(setting_color_for_state(status));
//        restart_inactivity_timer();
//
//        if (status == MAN_GREEN) {
//            TrafficFSM_TransitionTo(AUTO_RED_GREEN);
//            return;
//        }
//    }
//
//    /* BTN1 selects the next setting mode; BTN1 in MAN_GREEN returns to AUTO. */
//    if (button1_event != 0U) {
//        switch (status) {
//        case MAN_RED:    next_state = MAN_YELLOW;     break;
//        case MAN_YELLOW: next_state = MAN_GREEN;      break;
//        case MAN_GREEN:  next_state = AUTO_RED_GREEN; break;
//        default:         return;
//        }
//
//        TrafficFSM_TransitionTo(next_state);
//        return;
//    }
//
//    /* Any accepted button action restarts the inactivity timeout above. */
//    if (SoftwareTimer_IsExpired(SOFT_TIMER_MANUAL_TIMEOUT) != 0U) {
//        TrafficFSM_TransitionTo(AUTO_RED_GREEN);
//        return;
//    }
//
//    /* Only the blink timer changes the ON/OFF phase during normal operation. */
//    if (SoftwareTimer_IsExpired(SOFT_TIMER_MANUAL_BLINK) != 0U) {
//        blink_on = (uint8_t)!blink_on;
//
//        g_fsm_manual_debug.blink_toggle_count++;
//        g_fsm_manual_debug.last_blink_toggle_tick_ms = SoftwareTimer_GetTickMs();
//
//        SoftwareTimer_Start(SOFT_TIMER_MANUAL_BLINK, MANUAL_BLINK_TOGGLE_MS);
//        traffic_output_dirty = 1U;
//    }
//
//    if (traffic_output_dirty != 0U) {
//        apply_manual_traffic_output();
//    }
//
//    update_manual_display();
//}
//
//uint8_t FSM_Manual_GetMode(void)
//{
//    return mode_number_for_state(status);
//}
