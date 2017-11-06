#ifndef FLEX_READING_H
	#define FLEX_READING_H
	
	#include "stdint.h"
	
	
typedef struct{
	
	uint16_t Thumb;
	uint16_t Fore;
	uint16_t Middle;
	uint16_t Ring;
	uint16_t Little;
}Finger_t;

		void Flex_Init(void);
	
#endif
