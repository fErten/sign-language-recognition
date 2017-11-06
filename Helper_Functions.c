#include "stm32f4xx.h"
#include "Helper_Functions.h"

	void Set_Timer(void){
	
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	
		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = (42000 - 1);
	TIM_TimeBaseStructure.TIM_Prescaler = (5 - 1); // (100-1)
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x00;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
		
		TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2,ENABLE);
	

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
	