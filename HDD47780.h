#ifndef HDD47780_H
	#define HDD47780_H
	
	#include "stdint.h"
	
/* Connections between LCD and STM32F103RB  */
#define LCD_RS          GPIO_Pin_2   // PB2
#define LCD_RW          GPIO_Pin_11  // PC11
#define LCD_EN          GPIO_Pin_7   // PB7
#define LCD_D4          GPIO_Pin_12   // PC12
#define LCD_D5          GPIO_Pin_13   // PC13
#define LCD_D6					GPIO_Pin_12   // PB12
#define LCD_D7          GPIO_Pin_13   // PB13


void delay_ms(int ms);
void delay_us(int us);
unsigned char LCD_ReadByte(void);
void LCD_SendByte(unsigned char cmd); //Sends a data byte to LCD (byte 'cmd').
void LCD_SendCmd (unsigned char cmd); //Sends a command byte to LCD (byte 'cmd').
void LCD_SendData(unsigned char data);
void LCD_SendText(char *text);
void LCD_GoTo (unsigned char line, unsigned char column);
void LCD_Clear (void);
void LCD_Init (void);


	
#endif
