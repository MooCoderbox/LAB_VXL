#include "fsm_automatic.h"

#include "traffic_state.h"
#include "software_timer.h"
#include "fsm_setting.h"
#include "led_display.h"

static uint8_t milliseconds_to_seconds_ceil(uint32_t milliseconds)
{
    uint32_t seconds = (milliseconds + 999U) / 1000U;
    return (uint8_t)((seconds > 99U) ? 99U : seconds);
}

static uint32_t duration_for_state_ms(TrafficState state)
{
    switch (state) {
    case AUTO_RED_GREEN:
    case AUTO_GREEN_RED:
        return (uint32_t)FSM_Setting_GetValue(SETTING_GREEN) * 1000U;

    case AUTO_RED_YELLOW:
    case AUTO_YELLOW_RED:
        return (uint32_t)FSM_Setting_GetValue(SETTING_YELLOW) * 1000U;

    default:
        return 0U;
    }
}

void FSM_Automatic_Enter(TrafficState state)
{
    /* Entry action: write the traffic-light pattern once and arm its phase timer. */
    switch (state) {
    case AUTO_RED_GREEN:
        LED_Display_SetTraffic(TRAFFIC_RED, TRAFFIC_GREEN);
        break;

    case AUTO_RED_YELLOW:
        LED_Display_SetTraffic(TRAFFIC_RED, TRAFFIC_YELLOW);
        break;

    case AUTO_GREEN_RED:
        LED_Display_SetTraffic(TRAFFIC_GREEN, TRAFFIC_RED);
        break;

    case AUTO_YELLOW_RED:
        LED_Display_SetTraffic(TRAFFIC_YELLOW, TRAFFIC_RED);
        break;

    default:
        return;
    }

    SoftwareTimer_Start(SOFT_TIMER_AUTO_PHASE, duration_for_state_ms(state));
}

void FSM_Automatic_Exit(void)
{
    /* The shared transition manager calls this before entering another mode. */
    SoftwareTimer_Stop(SOFT_TIMER_AUTO_PHASE);
}

void FSM_Automatic_Update(uint8_t button1_event)
{
    TrafficState next_state;

    /* BTN1 has priority over a phase timeout if both arrive in the same loop. */
    if (button1_event != 0U) {
        TrafficFSM_TransitionTo(MAN_RED);
        return;
    }

    if (SoftwareTimer_IsExpired(SOFT_TIMER_AUTO_PHASE) != 0U) {
        switch (status) {
        case AUTO_RED_GREEN:  next_state = AUTO_RED_YELLOW;  break;
        case AUTO_RED_YELLOW: next_state = AUTO_GREEN_RED;   break;
        case AUTO_GREEN_RED:  next_state = AUTO_YELLOW_RED;  break;
        case AUTO_YELLOW_RED: next_state = AUTO_RED_GREEN;   break;
        default:              return;
        }

        TrafficFSM_TransitionTo(next_state);
    }

    /* Refresh the display buffer; LED_Display_Task() performs physical scanning. */
    {
        uint8_t road_a_remaining;
        uint8_t road_b_remaining;

        FSM_Automatic_GetRemaining(&road_a_remaining, &road_b_remaining);
        LED_Display_SetNumbers(road_a_remaining, road_b_remaining);
    }
}

void FSM_Automatic_GetRemaining(uint8_t *road_a_seconds,
                                uint8_t *road_b_seconds)
{
    uint8_t yellow = FSM_Setting_GetValue(SETTING_YELLOW);
    uint8_t remaining = milliseconds_to_seconds_ceil(
        SoftwareTimer_RemainingMs(SOFT_TIMER_AUTO_PHASE));
    uint8_t road_a = 0U;
    uint8_t road_b = 0U;

    /* RED on one road includes the other road's green plus yellow phases. */
    switch (status) {
    case AUTO_RED_GREEN:
        road_a = (uint8_t)(remaining + yellow);
        road_b = remaining;
        break;

    case AUTO_RED_YELLOW:
        road_a = remaining;
        road_b = remaining;
        break;

    case AUTO_GREEN_RED:
        road_a = remaining;
        road_b = (uint8_t)(remaining + yellow);
        break;

    case AUTO_YELLOW_RED:
        road_a = remaining;
        road_b = remaining;
        break;

    default:
        road_a = 0U;
        road_b = 0U;
        break;
    }

    if (road_a > 99U) road_a = 99U;
    if (road_b > 99U) road_b = 99U;

    if (road_a_seconds != 0) *road_a_seconds = road_a;
    if (road_b_seconds != 0) *road_b_seconds = road_b;
}
