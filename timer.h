#ifndef _timer_
#define _timer_

#include "stm32f1xx_hal.h"

#include <stdint.h>
#include<stdbool.h>

void DELAY_TIM_Ms(TIM_HandleTypeDef *htim, uint16_t Time);
void delay_us(TIM_HandleTypeDef* htim,uint16_t us);


#endif
