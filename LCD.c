#include<LCD.h>
#include<timer.h>

/*
link to the datasheet of HD44780
	https://html.alldatasheet.com/html-pdf/63673/HITACHI/HD44780/11541/46/HD44780.html
 */

//here is the function to setup LCD to work, look at the datasheet in page 46 to know more about this
//the address of slave is in this case have to leftshift 1 (<<1), read STM32 datasheet to know more about.
//so if the address of the slave is 0x27 then pass 0x4e in this function as address
void initLCD(LCD* lcd,I2C_HandleTypeDef* hi2c1, uint8_t address,TIM_HandleTypeDef* htim){
	lcd->address = address;
	lcd->hi2c1 = hi2c1;
	lcd->htim = htim;

	//wait for more than 40ms after Vcc rises to 2.7V
	DELAY_TIM_Ms(lcd->htim, 50);

	//here send byte with value of 0x04 in 4 mode bit, so left shift it 4 times
	//wait for at least 4.1 ms according to datasheet
	writeByteLCD(lcd, 0x03<<4);
	DELAY_TIM_Ms(lcd->htim, 5);

	//seconds time, wait for more than 100 us
	writeByteLCD(lcd, 0x03<<4);
	DELAY_TIM_Ms(lcd->htim, 5);

	writeByteLCD(lcd, 0x03<<4);
	DELAY_TIM_Ms(lcd->htim, 1);

	//return home with address of 0x02
	homeLCD(lcd);

	//turn the LCD off to confingure the LCD - or else it will display some weird symbols
	offLCD(lcd);

	// according to the datasheet
	commandLCD(lcd, 0x04);

	//display clear and set entry mode
	//here i chose I/D to be 1 , so that the cursor move and display standstill
	//also set LCD to 4 bit mode, 2 line, 5x7 Dots, the address is 0x28
	clearLCD(lcd);
	commandLCD(lcd, 0x28|0x06);

	//turn the LCD back on with cursor off and end the setup - LCD is ready to work
	onLCD(lcd);

}

void homeLCD(LCD*lcd){
	commandLCD(lcd, returnHome);
}

void offLCD(LCD* lcd){
	commandLCD(lcd, displayOFF);
}

void onLCD(LCD*lcd){
	commandLCD(lcd,cursorOff);
}

void writeByteLCD(LCD* lcd,uint8_t value){
	/*
	 * The principals of working of the LCD is that it has to put the EN pin high and delay for a periods of time (here i set it for 1 ms) and
	  	  then reset it and wait for 49ms every time the master (microcontroller- STM32) sends data to LCD.

	 * The R/S bit is set to 1 when we want to read the data from the slave, and set to low when we want to send the data to the slave and we commit
	 	 that by function sendLCD

	 * Dont want it to blink everytime setCursor or clear so have to | noBlinkLCD
	 * */
	uint8_t data = value|noBlinkLCD;
	uint8_t enHigh = value|EnableLCD|noBlinkLCD;
	uint8_t enLow = (value & ~EnableLCD)|noBlinkLCD;

	uint8_t* d = &data;
	uint8_t* eh  = &enHigh;
	uint8_t* el = &enLow;

	HAL_I2C_Master_Transmit(lcd->hi2c1,lcd->address, d, sizeof(data), 1);
	DELAY_TIM_Ms(lcd->htim, 1);
	HAL_I2C_Master_Transmit(lcd->hi2c1,lcd->address ,eh, sizeof(enHigh), 1);
	DELAY_TIM_Ms(lcd->htim, 1);
	HAL_I2C_Master_Transmit(lcd->hi2c1,lcd->address, el, sizeof(enLow), 1);
	DELAY_TIM_Ms(lcd->htim, 50);
}

void printNumLCD(LCD* lcd,uint8_t num){
	//note that this function can only use to send the data in type INT
	char string[16];
	sprintf(string,"%d",num);
	printLCD(lcd, string);
}

void printLCD(LCD* lcd,char* string){
	for(int index=0;string[index]!= '\0';index++){
		sendLCD(lcd, string[index], 1);
	}
}

void commandLCD(LCD* lcd,uint8_t value){
	sendLCD(lcd, value, 0);
}

void clearLCD(LCD* lcd){
	commandLCD(lcd, clear);
}

void setCursor(LCD* lcd,uint8_t row, uint8_t column){
	 !row ? (commandLCD(lcd,firstLine|column)) : (commandLCD(lcd,secondLine|column));
}

void sendLCD(LCD* lcd,uint8_t value,bool mode){
	//have to split the data to highnib and low nib, then send the highnib first, lownib later
	//mode 0 is for command, 1 is for sending data
	uint8_t highnib=value&0xf0;
	uint8_t lownib=(value<<4)&0xf0;
	writeByteLCD(lcd,highnib|mode);
	writeByteLCD(lcd,lownib|mode);
}
