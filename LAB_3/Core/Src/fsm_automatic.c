#include "global.h"
#include "fsm_automatic.h"

// static status active_auto_state;
// static uint8_t active_auto_state_valid;



static uint8_t auto_handle_buttons(void)
{
    isButton2Pressed();
    isButton3Pressed();
    if (isButton1Pressed()) {
        fsm_setting_load(getTimeOfSettingColor(SETTING_RED));
        LED_Display_BlinkStart(TRAFFIC_RED);
        return 1U;
    }
    return 0U;
}

void fsm_automatic_run(void)
{
    uint32_t phase_remaining_ms;
    uint32_t road_a_remaining_ms;
    uint32_t road_b_remaining_ms;

    switch (status_fsm) {
    case TRAFFIC_STATE_INIT:
        software_timer_set(SOFT_TIMER_AUTO_PHASE,
                (uint32_t)getTimeOfSettingColor(SETTING_GREEN) * 1000U);
        status_fsm = AUTO_RED_GREEN;
        break;

    case AUTO_RED_GREEN:
        if (auto_handle_buttons()) {
            status_fsm = MAN_RED;
            break;
        }

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
        if (auto_handle_buttons()) {
            status_fsm = MAN_RED;
            break;
        }

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
        if (auto_handle_buttons()) {
            status_fsm = MAN_RED;
            break;
        }

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
        if (auto_handle_buttons()) {
            status_fsm = MAN_RED;
            break;
        }

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


