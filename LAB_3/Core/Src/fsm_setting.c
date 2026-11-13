#include "fsm_setting.h"
#include "config.h"
#include "global.h"
#include "button.h"

/*
 * Traffic light durations: RED = GREEN + YELLOW.
 */
uint8_t red_seconds    = DEFAULT_RED_SECONDS;
uint8_t yellow_seconds = DEFAULT_YELLOW_SECONDS;
uint8_t green_seconds  = DEFAULT_GREEN_SECONDS;

static uint8_t pending_value = 1U;

uint8_t getTimeOfSettingColor(SettingColor t) {
    switch (t) {
        case SETTING_RED:
            return red_seconds;
        case SETTING_YELLOW:
            return yellow_seconds;
        case SETTING_GREEN:
            return green_seconds;
        default:
            return 0; // invalid
    }
}

void fsm_setting_load(uint8_t initial_value)
{
    if (initial_value < 1U) {
        pending_value = 1U;
    } else if (initial_value > 99U) {
        pending_value = 99U;
    } else {
        pending_value = initial_value;
    }
}

uint8_t fsm_setting_get_value(void)
{
    return pending_value;
}

void fsm_setting_run(void)
{
    if ((status_fsm != MAN_RED) &&
        (status_fsm != MAN_YELLOW) &&
        (status_fsm != MAN_GREEN)) {
        return;
    }

    if (isButton2Pressed()) {
        pending_value = (pending_value >= 99U) ? 1U : (pending_value + 1U);
    }

    switch (status_fsm) {
    case MAN_RED: /* Mode 2: save RED duration, keep RED = YELLOW + GREEN */
        if (isButton3Pressed()) {
            red_seconds = pending_value;
            if (red_seconds > yellow_seconds) {
                green_seconds = red_seconds - yellow_seconds;
            } else if (red_seconds >= 2U) {
                yellow_seconds = 1U;
                green_seconds = red_seconds - 1U;
            } else {
                yellow_seconds = 0U;
                green_seconds = 1U;
            }
        }
        break;

    case MAN_YELLOW: /* Mode 3: save AMBER duration, keep RED = YELLOW + GREEN */
        if (isButton3Pressed()) {
            yellow_seconds = pending_value;
            if ((uint16_t)green_seconds + yellow_seconds > 99U) {
                green_seconds = 99U - yellow_seconds;
            }
            red_seconds = green_seconds + yellow_seconds;
        }
        break;

    case MAN_GREEN: /* Mode 4: save GREEN duration, keep RED = YELLOW + GREEN */
        if (isButton3Pressed()) {
            green_seconds = pending_value;
            if ((uint16_t)green_seconds + yellow_seconds > 99U) {
                yellow_seconds = 99U - green_seconds;
            }
            red_seconds = green_seconds + yellow_seconds;
        }
        break;

    default:
        break;
    }
}