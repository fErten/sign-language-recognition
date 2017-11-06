
#ifndef MYUSART_H
#define MYUSART_H

#include "stm32f4xx.h"
#include "stm32f4xx_usart.h" 

	void myUsart_Init(void);
	void USART_puts(USART_TypeDef* USARTx, volatile char *s);
	
	
#endif
