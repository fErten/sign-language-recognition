#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include "myUsart.h"
#include <stdio.h>
#include <string.h>

#define MAX_STRLEN 24 

char received_string[MAX_STRLEN+1]; 

void myUsart_Init(void){
	
	GPIO_InitTypeDef GPIO_InitStructure; 
	USART_InitTypeDef USART_InitStruct; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;  // Pins 6 (TX) , 7 (RX)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			
	GPIO_Init(GPIOB, &GPIO_InitStructure);					
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1); 
	

	USART_InitStruct.USART_BaudRate = 115200;				
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;	
	USART_InitStruct.USART_Parity = USART_Parity_No;		
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
	USART_InitStruct.USART_Mode = USART_Mode_Tx; 
	USART_Init(USART1, &USART_InitStruct);					
								

	USART_Cmd(USART1, ENABLE);
}
	
	void USART_puts(USART_TypeDef* USARTx, volatile char *s){
		
			while(*s){
				// wait until data register is empty
				while( !(USARTx->SR & 0x00000040) ); 
				USART_SendData(USARTx, *s);
					*s++;
						
			}
	}
	