#include "DHT11.h"

void getData(DHT11* dht11,TIM_HandleTypeDef* htim){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0);
	HAL_Delay(20);

		GPIO_InitTypeDef GPIO_InitStruct = {0};

		GPIO_InitStruct.Pin = GPIO_PIN_14;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	delay_us(htim, 45);

	getTime(htim,80,1);
	getTime(htim,80,0);

	for(int i=0;i<5;i++){
	    for(int j=0;j<8;j++){
	      uint16_t time_wait =getTime(htim,50,1);
	      uint16_t time_bit = getTime(htim,70,0);
	      if(time_bit< time_wait){
	        (dht11->buffer[i] <<=1);
	      }else{
	    	  dht11->buffer[i] = (dht11->buffer[i] <<1) |1;
	      }
	    }
	  }
	  if(debug(dht11) == false) dht11->bug =1;
}

int getTemp(DHT11* dht11, bool C){
	if(C) return dht11->buffer[2];
	else return dht11->buffer[2]*9/5+32;
}

int getHumid(DHT11* dht11){
	return dht11->buffer[0];
}

int getTime(TIM_HandleTypeDef* htim,uint8_t time, bool mode){
	HAL_TIM_Base_Start(htim);
	__HAL_TIM_SET_COUNTER(htim,0);
	while( __HAL_TIM_GetCounter(htim) < time+10){
		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == mode) return __HAL_TIM_GetCounter(htim);
	}
	return 0;
}

void delay_us(TIM_HandleTypeDef* htim,uint16_t us){
	// set the counter value a 0,wait for the counter to reach the us input in the parameter
	HAL_TIM_Base_Start(htim);
	__HAL_TIM_SET_COUNTER(htim,0);
	while (__HAL_TIM_GET_COUNTER(htim) < us){};
}

bool debug(DHT11* dht11)
{
    if(dht11->buffer[0]+dht11->buffer[1]+dht11->buffer[2]+dht11->buffer[3] != dht11->buffer[4]) return false;
    return true;
}
