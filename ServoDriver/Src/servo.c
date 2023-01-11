/*
 * servo.c
 *
 *  Created on: Jan 11, 2023
 *      Author: alide
 *
 *      SETUP
 *      You need to set timer to work at 50hz and you must set the counter period as 19.
 *
 *
 */


#include "servo.h"
#include "stm32f4xx_hal.h"
#define KANAL TIM_CHANNEL_1
extern TIM_HandleTypeDef htim2;

void SERVO_setZero(){
	__HAL_TIM_SET_COMPARE(&htim2,KANAL,5);
}
void SERVO_set90()
{
	__HAL_TIM_SET_COMPARE(&htim2,KANAL,15);
}
void SERVO_set180(){
	__HAL_TIM_SET_COMPARE(&htim2,KANAL,25);
}
void setDegrees(uint8_t x){
	__HAL_TIM_SET_COMPARE(&htim2,KANAL,5+(float)x/9.0);
}
