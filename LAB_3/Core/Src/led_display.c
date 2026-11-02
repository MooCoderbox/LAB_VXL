#include "main.h"
#include "led_display.h"
#include "config.h"
#include "software_timer.h"

/* Segment lookup table from the supplied Common-Anode example: 0 = ON. */
static const uint8_t digit_segments[10][7] = {
    /* a, b, c, d, e, f, g */
    {0, 0, 0, 0, 0, 0, 1}, /* 0 */
    {1, 0, 0, 1, 1, 1, 1}, /* 1 */
    {0, 0, 1, 0, 0, 1, 0}, /* 2 */
    {0, 0, 0, 0, 1, 1, 0}, /* 3 */
    {1, 0, 0, 1, 1, 0, 0}, /* 4 */
    {0, 1, 0, 0, 1, 0, 0}, /* 5 */
    {0, 1, 0, 0, 0, 0, 0}, /* 6 */
    {0, 0, 0, 1, 1, 1, 1}, /* 7 */
    {0, 0, 0, 0, 0, 0, 0}, /* 8 */
    {0, 0, 0, 0, 1, 0, 0}  /* 9 */
};

/* These values are written by the main loop and read by LED_Display_Task(). */
static uint8_t road_a_number;
static uint8_t road_b_number;
/*
 * 0: Scan tens digits (EN0 + EN2).
 * 1: Scan units digits (EN1 + EN3).
 */
static uint8_t current_pair;

static void write_road_a_segments(uint8_t number)
{
    const uint8_t *s = digit_segments[number % 10U];

    /* Road A / EW uses the a1..g1 segment bank on GPIOB. */
    HAL_GPIO_WritePin(GPIOB, a1_Pin, s[0] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, b1_Pin, s[1] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, c1_Pin, s[2] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, d1_Pin, s[3] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, e1_Pin, s[4] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, f1_Pin, s[5] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, g1_Pin, s[6] ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static void write_road_b_segments(uint8_t number)
{
    const uint8_t *s = digit_segments[number % 10U];

    /* Road B / NS uses the a..g segment bank on GPIOA. */
    HAL_GPIO_WritePin(GPIOA, a_Pin, s[0] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, b_Pin, s[1] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, c_Pin, s[2] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, d_Pin, s[3] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, e_Pin, s[4] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, f_Pin, s[5] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, g_Pin, s[6] ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static void disable_all_digits(void)
{
#if SEVENSEG_ENABLE_ACTIVE_LOW
    HAL_GPIO_WritePin(GPIOB, EN0_Pin | EN1_Pin | EN2_Pin | EN3_Pin, GPIO_PIN_SET);
#else
    HAL_GPIO_WritePin(GPIOB, EN0_Pin | EN1_Pin | EN2_Pin | EN3_Pin, GPIO_PIN_RESET);
#endif
}

// static void enable_digit(uint16_t pin)
// {
// #if SEVENSEG_ENABLE_ACTIVE_LOW
//     HAL_GPIO_WritePin(GPIOB, pin, GPIO_PIN_RESET);
// #else
//     HAL_GPIO_WritePin(GPIOB, pin, GPIO_PIN_SET);
// #endif
// }

/*
 * Enable one or multiple digits simultaneously.
 * All EN pins share GPIOB.
 */
static void enable_digits(uint16_t pins)
{
#if SEVENSEG_ENABLE_ACTIVE_LOW
    HAL_GPIO_WritePin(GPIOB, pins, GPIO_PIN_RESET);
#else
    HAL_GPIO_WritePin(GPIOB, pins, GPIO_PIN_SET);
#endif
}

static void set_road_a_color(TrafficColor color)
{
    const uint16_t all_road_a = LED_RED_A_Pin | LED_YEL_A_Pin | LED_GRE_A_Pin;
    HAL_GPIO_WritePin(GPIOA, all_road_a, GPIO_PIN_RESET);

    switch (color) {
    case TRAFFIC_RED:    HAL_GPIO_WritePin(GPIOA, LED_RED_A_Pin, GPIO_PIN_SET); break;
    case TRAFFIC_YELLOW: HAL_GPIO_WritePin(GPIOA, LED_YEL_A_Pin, GPIO_PIN_SET); break;
    case TRAFFIC_GREEN:  HAL_GPIO_WritePin(GPIOA, LED_GRE_A_Pin, GPIO_PIN_SET); break;
    case TRAFFIC_OFF:
    default: break;
    }
}

static void set_road_b_color(TrafficColor color)
{
    const uint16_t all_road_b = LED_RED_B_Pin | LED_YEL_B_Pin | LED_GRE_B_Pin;
    HAL_GPIO_WritePin(GPIOA, all_road_b, GPIO_PIN_RESET);

    switch (color) {
    case TRAFFIC_RED:    HAL_GPIO_WritePin(GPIOA, LED_RED_B_Pin, GPIO_PIN_SET); break;
    case TRAFFIC_YELLOW: HAL_GPIO_WritePin(GPIOA, LED_YEL_B_Pin, GPIO_PIN_SET); break;
    case TRAFFIC_GREEN:  HAL_GPIO_WritePin(GPIOA, LED_GRE_B_Pin, GPIO_PIN_SET); break;
    case TRAFFIC_OFF:
    default: break;
    }
}

// void LED_Display_Init(void)
// {
//     set_road_a_color(TRAFFIC_OFF);
//     set_road_b_color(TRAFFIC_OFF);
//     disable_all_digits();

//     /* Turn all segments OFF before multiplex scanning starts. */
//     HAL_GPIO_WritePin(GPIOA, a_Pin | b_Pin | c_Pin | d_Pin | e_Pin | f_Pin | g_Pin,
//                       GPIO_PIN_SET);
//     HAL_GPIO_WritePin(GPIOB, a1_Pin | b1_Pin | c1_Pin | d1_Pin |
//                       e1_Pin | f1_Pin | g1_Pin, GPIO_PIN_SET);

//     road_a_number = 0U;
//     road_b_number = 0U;
//     current_digit = 3U;

//     /* First scan takes place after one configured scan interval. */
//     SoftwareTimer_Start(SOFT_TIMER_DISPLAY_SCAN, SEVENSEG_SCAN_INTERVAL_MS);
// }

void LED_Display_Init(void)
{
    set_road_a_color(TRAFFIC_OFF);
    set_road_b_color(TRAFFIC_OFF);

    disable_all_digits();

    HAL_GPIO_WritePin(GPIOA,
        a_Pin | b_Pin | c_Pin | d_Pin |
        e_Pin | f_Pin | g_Pin,
        GPIO_PIN_SET);

    HAL_GPIO_WritePin(GPIOB,
        a1_Pin | b1_Pin | c1_Pin | d1_Pin |
        e1_Pin | f1_Pin | g1_Pin,
        GPIO_PIN_SET);

    road_a_number = 0U;
    road_b_number = 0U;

    current_pair = 0U;

    SoftwareTimer_Start(
        SOFT_TIMER_DISPLAY_SCAN,
        SEVENSEG_SCAN_INTERVAL_MS
    );
}

void LED_Display_SetTraffic(TrafficColor road_a, TrafficColor road_b)
{
    set_road_a_color(road_a);
    set_road_b_color(road_b);
}

void LED_Display_BlinkBothRoads(TrafficColor color, uint8_t turn_on)
{
    if (turn_on != 0U) {
        /* In manual modes, the selected color on BOTH roads blinks together. */
        LED_Display_SetTraffic(color, color);
    } else {
        LED_Display_SetTraffic(TRAFFIC_OFF, TRAFFIC_OFF);
    }
}

void LED_Display_SetNumbers(uint8_t road_a_value, uint8_t road_b_value)
{
    road_a_number = (road_a_value > 99U) ? 99U : road_a_value;
    road_b_number = (road_b_value > 99U) ? 99U : road_b_value;
}

// void LED_Display_Task(void)
// {
//     uint8_t value;

//     /* Poll a timer flag: no delay and no waiting inside the display driver. */
//     if (SoftwareTimer_IsExpired(SOFT_TIMER_DISPLAY_SCAN) == 0U) {
//         return;
//     }
//     SoftwareTimer_Start(SOFT_TIMER_DISPLAY_SCAN, SEVENSEG_SCAN_INTERVAL_MS);

//     current_digit = (uint8_t)((current_digit + 1U) % 4U);

//     /* Disable all digits before changing segment data to avoid ghosting. */
//     disable_all_digits();

//     switch (current_digit) {
//     case 0U: /* EN0: EW tens digit. */
//         value = (uint8_t)(road_a_number / 10U);
//         write_road_a_segments(value);
//         enable_digit(EN0_Pin);
//         break;

//     case 1U: /* EN1: EW units digit. */
//         value = (uint8_t)(road_a_number % 10U);
//         write_road_a_segments(value);
//         enable_digit(EN1_Pin);
//         break;

//     case 2U: /* EN2: NS tens digit. */
//         value = (uint8_t)(road_b_number / 10U);
//         write_road_b_segments(value);
//         enable_digit(EN2_Pin);
//         break;

//     case 3U: /* EN3: NS units digit. */
//     default:
//         value = (uint8_t)(road_b_number % 10U);
//         write_road_b_segments(value);
//         enable_digit(EN3_Pin);
//         break;
//     }
// }

void LED_Display_Task(void)
{
    uint8_t road_a_value;
    uint8_t road_b_value;

    /* Wait until the scan timer expires. */
    if (SoftwareTimer_IsExpired(SOFT_TIMER_DISPLAY_SCAN) == 0U) {
        return;
    }

    SoftwareTimer_Start(
        SOFT_TIMER_DISPLAY_SCAN,
        SEVENSEG_SCAN_INTERVAL_MS
    );

    /*
     * Step 1: Disable all digits first.
     * Prevent ghosting while changing segment signals.
     */
    disable_all_digits();

    if (current_pair == 0U)
    {
        /* =================================
         * Scan tens digits simultaneously.
         * EW: EN0
         * NS: EN2
         * ================================= */

        road_a_value = road_a_number / 10U;
        road_b_value = road_b_number / 10U;

        /* Each road has its own segment bank. */
        write_road_a_segments(road_a_value);
        write_road_b_segments(road_b_value);

        /* Enable both tens digits at the same time. */
        enable_digits(EN0_Pin | EN2_Pin);

        current_pair = 1U;
    }
    else
    {
        /* =================================
         * Scan units digits simultaneously.
         * EW: EN1
         * NS: EN3
         * ================================= */

        road_a_value = road_a_number % 10U;
        road_b_value = road_b_number % 10U;

        write_road_a_segments(road_a_value);
        write_road_b_segments(road_b_value);

        /* Enable both units digits at the same time. */
        enable_digits(EN1_Pin | EN3_Pin);

        current_pair = 0U;
    }
}
