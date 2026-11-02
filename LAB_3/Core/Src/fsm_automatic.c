#include "fsm_automatic.h"
#include "software_timer.h"
#include "fsm_setting.h"
#include "led_display.h"

/* Road A = East-West; Road B = North-South. */
typedef enum {
    AUTO_A_RED_B_GREEN = 0,
    AUTO_A_RED_B_YELLOW,
    AUTO_A_GREEN_B_RED,
    AUTO_A_YELLOW_B_RED
} AutomaticState;

static AutomaticState current_state;
static uint8_t initialized;

static uint8_t milliseconds_to_seconds_ceil(uint32_t milliseconds)
{
    uint32_t seconds = (milliseconds + 999U) / 1000U;
    return (uint8_t)((seconds > 99U) ? 99U : seconds);
}

static void enter_state(AutomaticState next_state)
{
    uint8_t green = FSM_Setting_GetValue(SETTING_GREEN);
    uint8_t yellow = FSM_Setting_GetValue(SETTING_YELLOW);
    uint32_t duration_ms;

    current_state = next_state;

    /* Set the physical lamp pattern and duration associated with this phase. */
    switch (current_state) {
    case AUTO_A_RED_B_GREEN:
        LED_Display_SetTraffic(TRAFFIC_RED, TRAFFIC_GREEN);
        duration_ms = (uint32_t)green * 1000U;
        break;

    case AUTO_A_RED_B_YELLOW:
        LED_Display_SetTraffic(TRAFFIC_RED, TRAFFIC_YELLOW);
        duration_ms = (uint32_t)yellow * 1000U;
        break;

    case AUTO_A_GREEN_B_RED:
        LED_Display_SetTraffic(TRAFFIC_GREEN, TRAFFIC_RED);
        duration_ms = (uint32_t)green * 1000U;
        break;

    case AUTO_A_YELLOW_B_RED:
    default:
        LED_Display_SetTraffic(TRAFFIC_YELLOW, TRAFFIC_RED);
        duration_ms = (uint32_t)yellow * 1000U;
        break;
    }

    SoftwareTimer_Start(SOFT_TIMER_AUTO_PHASE, duration_ms);
    initialized = 1U;
}

void FSM_Automatic_Enter(void)
{
    /* INIT -> RED_GREEN in the provided diagram: A red, B green. */
    enter_state(AUTO_A_RED_B_GREEN);
}

void FSM_Automatic_Run(void)
{
    if (initialized == 0U) {
        FSM_Automatic_Enter();
    }

    /* The timer flag is consumed once; state changes happen in the main loop. */
    if (SoftwareTimer_IsExpired(SOFT_TIMER_AUTO_PHASE) != 0U) {
        switch (current_state) {
        case AUTO_A_RED_B_GREEN:  enter_state(AUTO_A_RED_B_YELLOW); break;
        case AUTO_A_RED_B_YELLOW: enter_state(AUTO_A_GREEN_B_RED);  break;
        case AUTO_A_GREEN_B_RED:  enter_state(AUTO_A_YELLOW_B_RED); break;
        case AUTO_A_YELLOW_B_RED:
        default:                  enter_state(AUTO_A_RED_B_GREEN);  break;
        }
    }

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
    uint8_t a = 0U;
    uint8_t b = 0U;

    switch (current_state) {
    case AUTO_A_RED_B_GREEN:
        a = (uint8_t)(remaining + yellow); /* A red includes B green + B yellow. */
        b = remaining;
        break;

    case AUTO_A_RED_B_YELLOW:
        a = remaining;
        b = remaining;
        break;

    case AUTO_A_GREEN_B_RED:
        a = remaining;
        b = (uint8_t)(remaining + yellow);
        break;

    case AUTO_A_YELLOW_B_RED:
    default:
        a = remaining;
        b = remaining;
        break;
    }

    if (a > 99U) a = 99U;
    if (b > 99U) b = 99U;

    if (road_a_seconds != 0) *road_a_seconds = a;
    if (road_b_seconds != 0) *road_b_seconds = b;
}

void FSM_Automatic_Update(void)
{
    switch (status) {

    case AUTO_RED_GREEN:

        if (SoftwareTimer_IsExpired(SOFT_TIMER_AUTO_PHASE)) {
            FSM_TransitionTo(AUTO_RED_YELLOW);
        }

        break;

    case AUTO_RED_YELLOW:

        if (SoftwareTimer_IsExpired(SOFT_TIMER_AUTO_PHASE)) {
            FSM_TransitionTo(AUTO_GREEN_RED);
        }

        break;

    case AUTO_GREEN_RED:

        if (SoftwareTimer_IsExpired(SOFT_TIMER_AUTO_PHASE)) {
            FSM_TransitionTo(AUTO_YELLOW_RED);
        }

        break;

    case AUTO_YELLOW_RED:

        if (SoftwareTimer_IsExpired(SOFT_TIMER_AUTO_PHASE)) {
            FSM_TransitionTo(AUTO_RED_GREEN);
        }

        break;

    default:
        break;
    }
}
