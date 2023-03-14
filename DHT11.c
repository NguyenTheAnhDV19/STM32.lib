/*
	link to the datasheet is here:
		https://html.alldatasheet.com/html-pdf/1440068/ETC/DHT11/395/6/DHT11.html
*/


#include "DHT11.h"

void getData(DHT11* dht11,TIM_HandleTypeDef* htim){
	//here confingure the pin B14 on Stm32 at output mode first to put it in reset mode -> signal the slave to gives feed back to master
	inPutSetup();
	
	//put Pin14 in reset mode and delay for at least 18ms (see the datasheet page 6/9 for more information)
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0);
	delay_us(htim, 18000);
	
	//configure Pin14 to Input_Pullup mode so the slave can pulldown the pinmode, this process takes up about 20-40ms
	outPutSetUp();

	//here delay for 45 ms, wait till slave pullpin down
	delay_us(htim, 45);

	//see datasheet page 6/9 for more information 
	getTime(htim,80,1);
	getTime(htim,80,0);

	// get data to buffer here - including humid and tempeature 
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

//This function used to wait till the mode of the pin meet mode, then return the total waiting time, it'll return 0 if the time is exceeded limit
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

//see the datasheet for more information
bool debug(DHT11* dht11)
{
    if(dht11->buffer[0]+dht11->buffer[1]+dht11->buffer[2]+dht11->buffer[3] != dht11->buffer[4]) return false;
    return true;
}

void inPutSetUp(){
	 GPIO_InitTypeDef GPIO_InitStruct = {0};
	 GPIO_InitStruct.Pin = GPIO_PIN_14;
	 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	 GPIO_InitStruct.Pull = GPIO_NOPULL;
	 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	 HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void outPutSetUp(){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_14;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
