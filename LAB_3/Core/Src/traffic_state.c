#include "traffic_state.h"

#include "fsm_automatic.h"
#include "fsm_manual.h"
#include "led_display.h"

/* Define the shared status exactly once in the entire application. */
TrafficState status = TRAFFIC_STATE_INIT;

static uint8_t is_automatic_state(TrafficState state)
{
    return (state >= AUTO_RED_GREEN && state <= AUTO_YELLOW_RED) ? 1U : 0U;
}

static uint8_t is_manual_state(TrafficState state)
{
    return (state >= MAN_RED && state <= MAN_GREEN) ? 1U : 0U;
}

void TrafficFSM_Init(void)
{
    /* Startup order: SoftwareTimer_Init(), LED_Display_Init(), FSM_Setting_Init(). */
    status = TRAFFIC_STATE_INIT;
    LED_Display_SetTraffic(TRAFFIC_OFF, TRAFFIC_OFF);
}

void TrafficFSM_TransitionTo(TrafficState next_state)
{
    TrafficState previous_state = status;

    /* Ignore invalid enum values instead of leaving the application undefined. */
    if ((next_state != TRAFFIC_STATE_INIT) &&
        (is_automatic_state(next_state) == 0U) &&
        (is_manual_state(next_state) == 0U)) {
        return;
    }

    /* Do not re-run entry actions when asked to remain in the same state. */
    if (previous_state == next_state) {
        return;
    }

    /* Stop timers owned by the state being exited. */
    if (is_automatic_state(previous_state) != 0U) {
        FSM_Automatic_Exit();
    } else if (is_manual_state(previous_state) != 0U) {
        FSM_Manual_Exit();
    }

    /* This is the only runtime location that changes the global status. */
    status = next_state;

    /* Run exactly one entry function for the destination state. */
    if (is_automatic_state(next_state) != 0U) {
        FSM_Automatic_Enter(next_state);
    } else if (is_manual_state(next_state) != 0U) {
        FSM_Manual_Enter(next_state);
    } else {
        /* INIT is an inactive state; leave traffic lamps safely OFF. */
        LED_Display_SetTraffic(TRAFFIC_OFF, TRAFFIC_OFF);
    }
}

void TrafficFSM_Update(uint8_t button1_event,
                       uint8_t button2_event,
                       uint8_t button3_event)
{
    if (is_automatic_state(status) != 0U) {
        /* BTN1 enters manual configuration; BTN2/BTN3 are ignored in AUTO. */
        FSM_Automatic_Update(button1_event);
    } else if (is_manual_state(status) != 0U) {
        FSM_Manual_Update(button1_event, button2_event, button3_event);
    } else {
        /* Safe recovery if the application is still in INIT. */
        TrafficFSM_TransitionTo(AUTO_RED_GREEN);
    }
}
