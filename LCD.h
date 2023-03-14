#ifndef _LCD_
#define _LCD_

#include "stm32f1xx_hal.h"

#include <stdint.h>
#include<string.h>
#include<stdbool.h>
#include<stdio.h>

#define clear      0x01
#define returnHome 0x02
#define shiftLeft  0x04
#define shiftRight 0x06
#define cursorOff  0x0c
#define cursorOn   0x0e
#define firstLine  0x80
#define secondLine 0xC0
#define noBlinkLCD 0x08
#define EnableLCD  0x04
#define displayOFF 0x08

typedef struct {
	I2C_HandleTypeDef* hi2c1;
	uint16_t address;
	TIM_HandleTypeDef* htim;
} LCD;

void sendLCD(LCD* lcd,uint8_t value,bool mode);
void setCursor(LCD* lcd,uint8_t row, uint8_t column);
void clearLCD(LCD* lcd);
void commandLCD(LCD* lcd,uint8_t value);
void printLCD(LCD* lcd,char* string);
void initLCD(LCD* lcd,I2C_HandleTypeDef* hi2c1, uint8_t address,TIM_HandleTypeDef* htim);
void printNumLCD(LCD* lcd,uint8_t num);
void writeByteLCD(LCD* lcd,uint8_t value);
void homeLCD(LCD* lcd);
void offLCD(LCD* lcd);
void onLCD(LCD* lcd);

#endif

