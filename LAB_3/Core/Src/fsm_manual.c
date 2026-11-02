#include "fsm_manual.h"
#include "software_timer.h"
#include "fsm_setting.h"
#include "led_display.h"
#include "config.h"

/* Current manual setting mode: 2 = RED, 3 = YELLOW, 4 = GREEN. */
static uint8_t manual_mode = 2U;
static uint8_t manual_active;
static uint8_t blink_on;

/*
 * Set only when the requested traffic-light output has actually changed.
 * This prevents the superloop from clearing and re-lighting GPIO pins on
 * every iteration while blink_on remains unchanged.
 */
static uint8_t traffic_output_dirty;

/* Debug counters are visible in STM32CubeIDE Expressions/Live Expressions. */

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

/*
 * Write the traffic-light GPIOs only when a blink edge or mode transition
 * occurs. Never call this unconditionally from the superloop.
 */
static void apply_manual_traffic_output(void)
{
    LED_Display_BlinkBothRoads(traffic_color_for_mode(), blink_on);

    

    traffic_output_dirty = 0U;
}

void FSM_Manual_Enter(void)
{
    manual_mode = 2U;
    manual_active = 1U;
    blink_on = 0U;
    traffic_output_dirty = 0U;

    /* Reset debug information each time the user enters manual mode. */
    

    FSM_Setting_BeginEdit(SETTING_RED);
    SoftwareTimer_Start(SOFT_TIMER_MANUAL_BLINK, MANUAL_BLINK_TOGGLE_MS);
    restart_inactivity_timer();

    /* Start with both roads dark. The first ON edge occurs after the interval. */
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

        /* Move to the next color-setting mode. Start its blink phase from OFF. */
        manual_mode++;
        FSM_Setting_BeginEdit(setting_color_for_mode());

        blink_on = 0U;
        SoftwareTimer_Start(SOFT_TIMER_MANUAL_BLINK, MANUAL_BLINK_TOGGLE_MS);
        traffic_output_dirty = 1U;
    }

    if (button2_event != 0U) {
        FSM_Setting_Increment();
        restart_inactivity_timer();
    }

    if (button3_event != 0U) {
        (void)FSM_Setting_Commit();
        /* BTN3 confirms the value; it does not cause a GPIO blink edge. */
        FSM_Setting_BeginEdit(setting_color_for_mode());
        restart_inactivity_timer();
    }

    /* Check inactivity before applying any further manual output update. */
    if (SoftwareTimer_IsExpired(SOFT_TIMER_MANUAL_TIMEOUT) != 0U) {
        manual_active = 0U;
        SoftwareTimer_Stop(SOFT_TIMER_MANUAL_BLINK);
        LED_Display_SetTraffic(TRAFFIC_OFF, TRAFFIC_OFF);
        return 1U;
    }

    /* A blink timer expiration is the only normal source of ON/OFF edges. */
    if (SoftwareTimer_IsExpired(SOFT_TIMER_MANUAL_BLINK) != 0U) {
        blink_on = (uint8_t)!blink_on;

        

        SoftwareTimer_Start(SOFT_TIMER_MANUAL_BLINK, MANUAL_BLINK_TOGGLE_MS);
        traffic_output_dirty = 1U;
    }

    /*
     * IMPORTANT: do not call LED_Display_BlinkBothRoads() every loop.
     * It writes GPIOs. Calling it only when dirty preserves the configured
     * ON/OFF interval and prevents very short accidental OFF pulses.
     */
    if (traffic_output_dirty != 0U) {
        apply_manual_traffic_output();
    }

    /* Updating the display buffer is safe here; physical digit scanning is separate. */
    LED_Display_SetNumbers(FSM_Setting_GetEditValue(), manual_mode);

    return 0U;
}

uint8_t FSM_Manual_GetMode(void)
{
    return manual_mode;
}
