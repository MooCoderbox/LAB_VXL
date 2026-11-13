#include "global.h"
#include "fsm_automatic.h"
#include "fsm_manual.h"

// static status active_auto_state;
// static uint8_t active_auto_state_valid;



void fsm_automatic_run(void)
{
    uint32_t phase_remaining_ms;
    uint32_t road_a_remaining_ms;
    uint32_t road_b_remaining_ms;

    /* Mode 1 -> Mode 2 when Button 1 is pressed; ignore Button 2/3 in Mode 1. */
    if ((status_fsm >= AUTO_RED_GREEN) && (status_fsm <= AUTO_YELLOW_RED)) {
        isButton2Pressed();
        isButton3Pressed();
        if (isButton1Pressed()) {
            fsm_manual_enter_red();
            return;
        }
    }

    switch (status_fsm) {
    case TRAFFIC_STATE_INIT:
        software_timer_set(SOFT_TIMER_AUTO_PHASE,
                (uint32_t)getTimeOfSettingColor(SETTING_GREEN) * 1000U);
        status_fsm = AUTO_RED_GREEN;
    case AUTO_RED_GREEN:
        task_AUTO_RED_GREEN();
        phase_remaining_ms =
            software_timer_get_remaining_ms(SOFT_TIMER_AUTO_PHASE);
        road_a_remaining_ms = phase_remaining_ms +
            (uint32_t)getTimeOfSettingColor(SETTING_YELLOW) * 1000U;
        road_b_remaining_ms = phase_remaining_ms;
        LED_Display_SetNumbers(
            milliseconds_to_display_seconds(road_a_remaining_ms),
            milliseconds_to_display_seconds(road_b_remaining_ms));

        if (software_timer_is_expired(SOFT_TIMER_AUTO_PHASE)) {
            software_timer_set(
                SOFT_TIMER_AUTO_PHASE,
                (uint32_t)getTimeOfSettingColor(SETTING_YELLOW) * 1000U);
            status_fsm = AUTO_RED_YELLOW;
        }
        break;

    case AUTO_RED_YELLOW:
        if (software_timer_is_expired(SOFT_TIMER_AUTO_PHASE)) {
            software_timer_set(
                SOFT_TIMER_AUTO_PHASE,
                (uint32_t)getTimeOfSettingColor(SETTING_GREEN) * 1000U);
            status_fsm = AUTO_GREEN_RED;
        }

        task_AUTO_RED_YELLOW();
        phase_remaining_ms =
            software_timer_get_remaining_ms(SOFT_TIMER_AUTO_PHASE);
        LED_Display_SetNumbers(
            milliseconds_to_display_seconds(phase_remaining_ms),
            milliseconds_to_display_seconds(phase_remaining_ms));
        break;

    case AUTO_GREEN_RED:
        if (software_timer_is_expired(SOFT_TIMER_AUTO_PHASE)) {
            software_timer_set(
                SOFT_TIMER_AUTO_PHASE,
                (uint32_t)getTimeOfSettingColor(SETTING_YELLOW) * 1000U);
            status_fsm = AUTO_YELLOW_RED;
        }

        task_AUTO_GREEN_RED();
        phase_remaining_ms =
            software_timer_get_remaining_ms(SOFT_TIMER_AUTO_PHASE);
        road_a_remaining_ms = phase_remaining_ms;
        road_b_remaining_ms = phase_remaining_ms +
            (uint32_t)getTimeOfSettingColor(SETTING_YELLOW) * 1000U;
        LED_Display_SetNumbers(
            milliseconds_to_display_seconds(road_a_remaining_ms),
            milliseconds_to_display_seconds(road_b_remaining_ms));
        break;

    case AUTO_YELLOW_RED:
        if (software_timer_is_expired(SOFT_TIMER_AUTO_PHASE)) {
            software_timer_set(
                SOFT_TIMER_AUTO_PHASE,
                (uint32_t)getTimeOfSettingColor(SETTING_GREEN) * 1000U);
            status_fsm = AUTO_RED_GREEN;
        }

        task_AUTO_YELLOW_RED();
        phase_remaining_ms =
            software_timer_get_remaining_ms(SOFT_TIMER_AUTO_PHASE);
        LED_Display_SetNumbers(
            milliseconds_to_display_seconds(phase_remaining_ms),
            milliseconds_to_display_seconds(phase_remaining_ms));
        break;

    default:
        break;
    }
}


