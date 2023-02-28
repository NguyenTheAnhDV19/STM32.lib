#ifndef _DHT11_
#define _DHT11_

#include "stm32f1xx_hal.h"

#include <stdint.h>
#include<stdbool.h>

typedef struct {
	char buffer[5];
	bool bug;
} DHT11;

void getData(DHT11* dht11, TIM_HandleTypeDef* htim);
void delay_us(TIM_HandleTypeDef* htim,uint16_t us);
int getTemp(DHT11* dht11,bool C);
int getHumid(DHT11* dht11);
int getTime(TIM_HandleTypeDef* htim,uint8_t time, bool mode);
bool debug(DHT11* dht11);

#endif
