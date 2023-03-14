#include<time.h>

#include "stm32f1xx_hal.h"

#include <stdint.h>
#include<stdbool.h>

void delay_us(TIM_HandleTypeDef* htim,uint16_t us){
	// set the counter value a 0,wait for the counter to reach the us input in the parameter
	HAL_TIM_Base_Start(htim);
	__HAL_TIM_SET_COUNTER(htim,0);
	while (__HAL_TIM_GET_COUNTER(htim) < us){};
}

void DELAY_TIM_Ms(TIM_HandleTypeDef *htim,uint16_t Time)
{
	HAL_TIM_Base_Start(htim);
	__HAL_TIM_SET_COUNTER(htim,0);
	while(Time--)
	{
		while(__HAL_TIM_GET_COUNTER(htim)<1000){}
	}
}
