/*
 * led_display.h
 *
 *  Created on: Nov 1, 2026
 *      Author: DELL
 */

#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_



void display7SEG(int num);
const int MAX_LED = 4;
int index_led = 0;
int led_buffer[4] = {1, 2, 3, 0}; // Gán giá trị cần hiển thị: 1, 2, 3, 0
void update7SEG(int index);
void updateClockBuffer(void);

typedef enum { ROAD_A, ROAD_B } Road;
typedef enum { COLOR_OFF, COLOR_RED, COLOR_YELLOW, COLOR_GREEN } Color;

void led_set_road(Road r, Color c);          // one colour on, others off
void led_set_color_all(Color c, int on);     // for blinking in manual modes
void led_all_off(void);

void seg_set_pair(int pair, int value);      // pair 0 or 1, value 0..99
void display_update(void);                   // multiplex scan, called from main loop

#endif /* INC_LED_DISPLAY_H_ */
