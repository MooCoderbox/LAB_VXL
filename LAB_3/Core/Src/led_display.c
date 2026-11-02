/*
 * led_display.c
 *
 *  Created on: Nov 1, 2026
 *      Author: DELL
 */
#include "led_display.h"



//void display7SEG(int num)
//{
//    if (num < 0 || num > 9) return;
//
//
//    static const uint8_t seg_table[10][7] = {
//        // a1, b1, c1, d1, e1, f1, g1
//        {  0,  0,  0,  0,  0,  0,  1 }, // 0
//        {  1,  0,  0,  1,  1,  1,  1 }, // 1
//        {  0,  0,  1,  0,  0,  1,  0 }, // 2
//        {  0,  0,  0,  0,  1,  1,  0 }, // 3
//        {  1,  0,  0,  1,  1,  0,  0 }, // 4
//        {  0,  1,  0,  0,  1,  0,  0 }, // 5
//        {  0,  1,  0,  0,  0,  0,  0 }, // 6
//        {  0,  0,  0,  1,  1,  1,  1 }, // 7
//        {  0,  0,  0,  0,  0,  0,  0 }, // 8
//        {  0,  0,  0,  0,  1,  0,  0 }  // 9
//    };
//
//    HAL_GPIO_WritePin(GPIOB, SEG0_Pin, seg_table[num][0] ? GPIO_PIN_SET : GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(GPIOB, SEG1_Pin, seg_table[num][1] ? GPIO_PIN_SET : GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(GPIOB, SEG2_Pin, seg_table[num][2] ? GPIO_PIN_SET : GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(GPIOB, SEG3_Pin, seg_table[num][3] ? GPIO_PIN_SET : GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(GPIOB, SEG4_Pin, seg_table[num][4] ? GPIO_PIN_SET : GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(GPIOB, SEG5_Pin, seg_table[num][5] ? GPIO_PIN_SET : GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(GPIOB, SEG6_Pin, seg_table[num][6] ? GPIO_PIN_SET : GPIO_PIN_RESET);
//}
//
//const int MAX_LED = 4;
//int index_led = 0;
//int led_buffer[4] = {1, 2, 3, 0}; // Gán giá trị cần hiển thị: 1, 2, 3, 0
//
//void update7SEG(int index) {
//    switch (index) {
//        case 0:
//            // Bật LED 1 (EN0 kích hoạt mức RESET)
//            HAL_GPIO_WritePin(GPIOA, EN0_Pin, GPIO_PIN_RESET);
//            HAL_GPIO_WritePin(GPIOA, EN1_Pin, GPIO_PIN_SET);
//            HAL_GPIO_WritePin(GPIOA, EN2_Pin, GPIO_PIN_SET);
//            HAL_GPIO_WritePin(GPIOA, EN3_Pin, GPIO_PIN_SET);
//
//
//
//            display7SEG(led_buffer[0]);
//            break;
//
//        case 1:
//            // Bật LED 2 (EN1 kích hoạt mức RESET)
//            HAL_GPIO_WritePin(GPIOA, EN0_Pin, GPIO_PIN_SET);
//            HAL_GPIO_WritePin(GPIOA, EN1_Pin, GPIO_PIN_RESET);
//            HAL_GPIO_WritePin(GPIOA, EN2_Pin, GPIO_PIN_SET);
//            HAL_GPIO_WritePin(GPIOA, EN3_Pin, GPIO_PIN_SET);
//
//            display7SEG(led_buffer[1]);
//            break;
//
//        case 2:
//            // Bật LED 3 (EN2 kích hoạt mức RESET)
//            HAL_GPIO_WritePin(GPIOA, EN0_Pin, GPIO_PIN_SET);
//            HAL_GPIO_WritePin(GPIOA, EN1_Pin, GPIO_PIN_SET);
//            HAL_GPIO_WritePin(GPIOA, EN2_Pin, GPIO_PIN_RESET);
//            HAL_GPIO_WritePin(GPIOA, EN3_Pin, GPIO_PIN_SET);
//
//
//            display7SEG(led_buffer[2]);
//            break;
//
//        case 3:
//            // Bật LED 4 (EN3 kích hoạt mức RESET)
//            HAL_GPIO_WritePin(GPIOA, EN0_Pin, GPIO_PIN_SET);
//            HAL_GPIO_WritePin(GPIOA, EN1_Pin, GPIO_PIN_SET);
//            HAL_GPIO_WritePin(GPIOA, EN2_Pin, GPIO_PIN_SET);
//            HAL_GPIO_WritePin(GPIOA, EN3_Pin, GPIO_PIN_RESET);
//
//
//            display7SEG(led_buffer[3]);
//            break;
//
//        default:
//            break;
//    }
//}
//int counter = 25;
//int led_blink = 1;
//
//
//void updateClockBuffer(void)
//{
//
//    led_buffer[0] = hour / 10;
//    led_buffer[1] = hour % 10;
//
//    led_buffer[2] = minute / 10;
//    led_buffer[3] = minute % 10;
//}
