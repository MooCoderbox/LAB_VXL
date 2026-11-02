#include "fsm_setting.h"
#include "config.h"

/*
 * For the four-state two-road sequence, RED = GREEN + YELLOW:
 * when one road is green and then yellow, the other road remains red.
 * Keeping this invariant avoids an unsafe overlap between green/yellow phases.
 */
static uint8_t red_seconds = DEFAULT_RED_SECONDS;
static uint8_t yellow_seconds = DEFAULT_YELLOW_SECONDS;
static uint8_t green_seconds = DEFAULT_GREEN_SECONDS;

static SettingColor selected_color = SETTING_RED;
static uint8_t edit_value = DEFAULT_RED_SECONDS;

static uint8_t get_maximum_edit_value(void)
{
    switch (selected_color) {
    case SETTING_RED:
        return 99U;
    case SETTING_YELLOW:
        return (uint8_t)(99U - green_seconds);
    case SETTING_GREEN:
        return (uint8_t)(99U - yellow_seconds);
    default:
        return 99U;
    }
}

void FSM_Setting_Init(void)
{
    green_seconds = DEFAULT_GREEN_SECONDS;
    yellow_seconds = DEFAULT_YELLOW_SECONDS;
    red_seconds = (uint8_t)(green_seconds + yellow_seconds);

    selected_color = SETTING_RED;
    edit_value = red_seconds;
}

void FSM_Setting_BeginEdit(SettingColor color)
{
    selected_color = color;
    edit_value = FSM_Setting_GetValue(color);
}

void FSM_Setting_Increment(void)
{
    uint8_t maximum = get_maximum_edit_value();
    uint8_t minimum = (selected_color == SETTING_RED) ? 2U : 1U;

    if (maximum < minimum) {
        maximum = minimum;
    }

    if ((edit_value >= maximum) || (edit_value < minimum)) {
        edit_value = minimum;
    } else {
        edit_value++;
    }
}

uint8_t FSM_Setting_Commit(void)
{
    switch (selected_color) {
    case SETTING_RED:
        /* At least one second of green and one second of yellow are required. */
        if (edit_value < 2U) {
            edit_value = 2U;
        }
        red_seconds = edit_value;

        /* Preserve yellow when possible; the remaining red interval becomes green. */
        if (yellow_seconds >= red_seconds) {
            yellow_seconds = (uint8_t)(red_seconds - 1U);
        }
        green_seconds = (uint8_t)(red_seconds - yellow_seconds);
        break;

    case SETTING_YELLOW:
        if (edit_value < 1U) edit_value = 1U;
        if (edit_value > (uint8_t)(99U - green_seconds)) {
            edit_value = (uint8_t)(99U - green_seconds);
        }
        yellow_seconds = edit_value;
        red_seconds = (uint8_t)(green_seconds + yellow_seconds);
        break;

    case SETTING_GREEN:
        if (edit_value < 1U) edit_value = 1U;
        if (edit_value > (uint8_t)(99U - yellow_seconds)) {
            edit_value = (uint8_t)(99U - yellow_seconds);
        }
        green_seconds = edit_value;
        red_seconds = (uint8_t)(green_seconds + yellow_seconds);
        break;

    default:
        break;
    }

    return FSM_Setting_GetValue(selected_color);
}

uint8_t FSM_Setting_GetEditValue(void)
{
    return edit_value;
}

uint8_t FSM_Setting_GetValue(SettingColor color)
{
    switch (color) {
    case SETTING_RED:    return red_seconds;
    case SETTING_YELLOW: return yellow_seconds;
    case SETTING_GREEN:  return green_seconds;
    default:             return 0U;
    }
}

void FSM_Setting_GetAll(uint8_t *red, uint8_t *yellow, uint8_t *green)
{
    if (red != 0) *red = red_seconds;
    if (yellow != 0) *yellow = yellow_seconds;
    if (green != 0) *green = green_seconds;
}
