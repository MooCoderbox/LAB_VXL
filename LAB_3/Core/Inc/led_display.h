#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_

#include <stdint.h>

typedef enum {
    TRAFFIC_OFF = 0,
    TRAFFIC_RED,
    TRAFFIC_YELLOW,
    TRAFFIC_GREEN
} TrafficColor;

/*
 * Road A = East-West: LED_*_A and seven-segment digits EN0/EN1 (a1..g1).
 * Road B = North-South: LED_*_B and seven-segment digits EN2/EN3 (a..g).
 */
void LED_Display_Init(void);
void LED_Display_SetTraffic(TrafficColor road_a, TrafficColor road_b);
void LED_Display_BlinkBothRoads(TrafficColor color, uint8_t turn_on);
void LED_Display_SetNumbers(uint8_t road_a_value, uint8_t road_b_value);
/* Call repeatedly from the main superloop; it never blocks. */
void LED_Display_Task(void);

#endif /* INC_LED_DISPLAY_H_ */
