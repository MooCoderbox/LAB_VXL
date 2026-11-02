/*
 * led_display.c
 *
 *  Created on: Nov 1, 2026
 *      Author: DELL
 */
#include "main.h"
#include "led_display.h"
#include "config.h"

static const uint8_t digit_segments[10][7] = {
    /* a, b, c, d, e, f, g. 0 = segment ON for common-anode wiring. */
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

static volatile uint8_t road_a_number = 0U;
static volatile uint8_t road_b_number = 0U;
static uint8_t current_digit = 3U;
static uint32_t last_scan_ms = 0U;

static void write_road_a_segments(uint8_t number)
{
    const uint8_t *s = digit_segments[number % 10U];
    /* The a1..g1 segment bank is connected to GPIOB. */
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
    /* The a..g segment bank is connected to GPIOA. */
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

static void enable_digit(uint16_t pin)
{
#if SEVENSEG_ENABLE_ACTIVE_LOW
    HAL_GPIO_WritePin(GPIOB, pin, GPIO_PIN_RESET);
#else
    HAL_GPIO_WritePin(GPIOB, pin, GPIO_PIN_SET);
#endif
}

static void set_road_a_color(TrafficColor color)
{
    uint16_t all = LED_RED_A_Pin | LED_YEL_A_Pin | LED_GRE_A_Pin;
    HAL_GPIO_WritePin(GPIOA, all, GPIO_PIN_RESET);

    switch (color) {
    case TRAFFIC_RED:
        HAL_GPIO_WritePin(GPIOA, LED_RED_A_Pin, GPIO_PIN_SET);
        break;
    case TRAFFIC_YELLOW:
        HAL_GPIO_WritePin(GPIOA, LED_YEL_A_Pin, GPIO_PIN_SET);
        break;
    case TRAFFIC_GREEN:
        HAL_GPIO_WritePin(GPIOA, LED_GRE_A_Pin, GPIO_PIN_SET);
        break;
    case TRAFFIC_OFF:
    default:
        break;
    }
}

static void set_road_b_color(TrafficColor color)
{
    uint16_t all = LED_RED_B_Pin | LED_YEL_B_Pin | LED_GRE_B_Pin;
    HAL_GPIO_WritePin(GPIOA, all, GPIO_PIN_RESET);

    switch (color) {
    case TRAFFIC_RED:
        HAL_GPIO_WritePin(GPIOA, LED_RED_B_Pin, GPIO_PIN_SET);
        break;
    case TRAFFIC_YELLOW:
        HAL_GPIO_WritePin(GPIOA, LED_YEL_B_Pin, GPIO_PIN_SET);
        break;
    case TRAFFIC_GREEN:
        HAL_GPIO_WritePin(GPIOA, LED_GRE_B_Pin, GPIO_PIN_SET);
        break;
    case TRAFFIC_OFF:
    default:
        break;
    }
}

void LED_Display_Init(void)
{
    set_road_a_color(TRAFFIC_OFF);
    set_road_b_color(TRAFFIC_OFF);
    disable_all_digits();

    /* Turn all segment outputs OFF (the segment bank is active-low). */
    HAL_GPIO_WritePin(GPIOA, a_Pin | b_Pin | c_Pin | d_Pin | e_Pin | f_Pin | g_Pin,
                      GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, a1_Pin | b1_Pin | c1_Pin | d1_Pin |
                      e1_Pin | f1_Pin | g1_Pin, GPIO_PIN_SET);

    road_a_number = 0U;
    road_b_number = 0U;
    current_digit = 3U;
    last_scan_ms = HAL_GetTick();
}

void LED_Display_SetTraffic(TrafficColor road_a, TrafficColor road_b)
{
    set_road_a_color(road_a);
    set_road_b_color(road_b);
}

void LED_Display_BlinkBothRoads(TrafficColor color, uint8_t turn_on)
{
    if (turn_on != 0U) {
        LED_Display_SetTraffic(color, color);
    } else {
        LED_Display_SetTraffic(TRAFFIC_OFF, TRAFFIC_OFF);
    }
}

void LED_Display_SetNumbers(uint8_t road_a_value, uint8_t road_b_value)
{
    if (road_a_value > 99U) road_a_value = 99U;
    if (road_b_value > 99U) road_b_value = 99U;

    road_a_number = road_a_value;
    road_b_number = road_b_value;
}

/* One digit is refreshed every 2 ms: all four digits refresh at 125 Hz. */
void LED_Display_Task(void)
{
    uint32_t now = HAL_GetTick();
    uint8_t value;

    if ((now - last_scan_ms) < SEVENSEG_SCAN_INTERVAL_MS) {
        return;
    }
    last_scan_ms = now;
    current_digit = (uint8_t)((current_digit + 1U) % 4U);

    /* Disable before changing segment lines to prevent ghosting. */
    disable_all_digits();

    switch (current_digit) {
    case 0U: /* EN0: road A tens */
        value = (uint8_t)(road_a_number / 10U);
        write_road_a_segments(value);
        enable_digit(EN0_Pin);
        break;

    case 1U: /* EN1: road A units */
        value = (uint8_t)(road_a_number % 10U);
        write_road_a_segments(value);
        enable_digit(EN1_Pin);
        break;

    case 2U: /* EN2: road B tens */
        value = (uint8_t)(road_b_number / 10U);
        write_road_b_segments(value);
        enable_digit(EN2_Pin);
        break;

    case 3U: /* EN3: road B units */
    default:
        value = (uint8_t)(road_b_number % 10U);
        write_road_b_segments(value);
        enable_digit(EN3_Pin);
        break;
    }
}
