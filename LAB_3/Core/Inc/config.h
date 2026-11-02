/*
 * config.h
 *
 *  Created on: Nov 2, 2026
 *      Author: DELL
 */

#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

/* TIM2 button scan is designed for a 10 ms interrupt period. */
#define BUTTON_DEBOUNCE_SAMPLES         2U

#define DEFAULT_GREEN_SECONDS           3U
#define DEFAULT_YELLOW_SECONDS          2U
#define DEFAULT_RED_SECONDS             (DEFAULT_GREEN_SECONDS + DEFAULT_YELLOW_SECONDS)

/* 2 Hz means a complete ON/OFF cycle every 500 ms. */
#define MANUAL_BLINK_TOGGLE_MS           250U
#define MANUAL_IDLE_TIMEOUT_MS           10000U

/* Four-digit multiplex refresh: each digit is selected every 2 ms. */
#define SEVENSEG_SCAN_INTERVAL_MS        2U

/* Set to 1 for active-low digit-enable pins (common-anode style). */
#define SEVENSEG_ENABLE_ACTIVE_LOW       1

#endif /* INC_CONFIG_H_ */
