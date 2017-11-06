#include "stm32f4xx.h"                  // Device header
#include "myI2C.h"
#include "stm32f4xx_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO



/* Private variables */
static uint32_t I2C_Timeout;


/* Private defines */
#define I2C_TRANSMITTER_MODE   0
#define I2C_RECEIVER_MODE      1
#define I2C_ACK_ENABLE         1
#define I2C_ACK_DISABLE        0


void myI2C_Init(void){

	
		I2C_InitTypeDef I2C_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure; // this is for the GPIO pins used as I2C1-SDA and I2C1-SCL
		

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//PB8 for I2C SCL and PB9 for I2C1_SDL
	
	
			//I2C SCL-SDA GPIO Pins Configuration
		GPIO_InitStructure.GPIO_Pin		 = GPIO_Pin_8 | GPIO_Pin_9; // Pins 8(I2C1_SCL) and 9(I2C1_SDA)
		GPIO_InitStructure.GPIO_Mode 	 = GPIO_Mode_AF; // the pins are configured as alternate function so the USART peripheral has access to them
		GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;// this defines the IO speed and has nothing to do with the baudrate!			GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;// this defines the output type as open drain
		GPIO_InitStructure.GPIO_OType  = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_PuPd 	 = GPIO_PuPd_UP;// this activates the pullup resistors on the IO pins
			GPIO_Init(GPIOB, &GPIO_InitStructure);// now all the values are passed to the GPIO_Init() 			GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1); //
		
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1); // SCL
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);	// SDA

      
    I2C_InitStructure.I2C_Mode 									= I2C_Mode_I2C;
		I2C_InitStructure.I2C_DutyCycle 						= I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 					= 0x00;
    I2C_InitStructure.I2C_Ack									 	= I2C_Ack_Disable;
    I2C_InitStructure.I2C_AcknowledgedAddress 	= I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed 						= 400000;  
			
			I2C_Init(I2C1, &I2C_InitStructure);
			I2C_Cmd(I2C1, ENABLE);
		
}

		uint8_t I2C_Read(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg) {
			
		uint8_t received_data;
		I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
		I2C_WriteData(I2Cx, reg);
		I2C_Stop(I2Cx);
		I2C_Start(I2Cx, address, I2C_RECEIVER_MODE, I2C_ACK_DISABLE);
		received_data = I2C_ReadNack(I2Cx);
		return received_data;
			
		}

void I2C_ReadMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* data, uint16_t count) {
	I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_ENABLE);
	I2C_WriteData(I2Cx, reg);
	//I2C_Stop(I2Cx);
	I2C_Start(I2Cx, address, I2C_RECEIVER_MODE, I2C_ACK_ENABLE);
	while (count--) {
		if (!count) {
			/* Last byte */
			*data++ = I2C_ReadNack(I2Cx);
		} else {
			*data++ = I2C_ReadAck(I2Cx);
		}
	}
}

void I2C_Write(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t data) {
	I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
	I2C_WriteData(I2Cx, reg);
	I2C_WriteData(I2Cx, data);
	I2C_Stop(I2Cx);
}

void I2C_WriteMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* data, uint16_t count) {
	I2C_Start(I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
	I2C_WriteData(I2Cx, reg);
	while (count--) {
		I2C_WriteData(I2Cx, *data++);
	}
	I2C_Stop(I2Cx);
}


/* Private functions */
int16_t I2C_Start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction, uint8_t ack) {
	/* Generate I2C start pulse */
	I2Cx->CR1 |= I2C_CR1_START;
	
	/* Wait till I2C is busy */
	I2C_Timeout = I2C_TIMEOUT;
	while (!(I2Cx->SR1 & I2C_SR1_SB)) {
		if (--I2C_Timeout == 0x00) {
			return 1;
		}
	}

	/* Enable ack if we select it */
	if (ack) {
		I2Cx->CR1 |= I2C_CR1_ACK;
	}

	/* Send write/read bit */
	if (direction == I2C_TRANSMITTER_MODE) {
		/* Send address with zero last bit */
		I2Cx->DR = address & ~I2C_OAR1_ADD0;
		
		/* Wait till finished */
		I2C_Timeout = I2C_TIMEOUT;
		while (!(I2Cx->SR1 & I2C_SR1_ADDR)) {
			if (--I2C_Timeout == 0x00) {
				return 1;
			}
		}
	}
	if (direction == I2C_RECEIVER_MODE) {
		/* Send address with 1 last bit */
		I2Cx->DR = address | I2C_OAR1_ADD0;
		
		/* Wait till finished */
		I2C_Timeout = I2C_TIMEOUT;
		while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
			if (--I2C_Timeout == 0x00) {
				return 1;
			}
		}
	}
	
	/* Read status register to clear ADDR flag */
	I2Cx->SR2;
	
	/* Return 0, everything ok */
	return 0;
}

void I2C_WriteData(I2C_TypeDef* I2Cx, uint8_t data) {
	/* Wait till I2C is not busy anymore */
	I2C_Timeout = I2C_TIMEOUT;
	while (!(I2Cx->SR1 & I2C_SR1_TXE) && I2C_Timeout) {
		I2C_Timeout--;
	}
	
	/* Send I2C data */
	I2Cx->DR = data;
}

uint8_t I2C_ReadAck(I2C_TypeDef* I2Cx) {
	uint8_t data;
	
	/* Enable ACK */
	I2Cx->CR1 |= I2C_CR1_ACK;
	
	/* Wait till not received */
	I2C_Timeout = I2C_TIMEOUT;
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
		if (--I2C_Timeout == 0x00) {
			return 1;
		}
	}
	
	/* Read data */
	data = I2Cx->DR;
	
	/* Return data */
	return data;
}

uint8_t I2C_ReadNack(I2C_TypeDef* I2Cx) {
	uint8_t data;
	
	/* Disable ACK */
	I2Cx->CR1 &= ~I2C_CR1_ACK;
	
	/* Generate stop */
	I2Cx->CR1 |= I2C_CR1_STOP;
	
	/* Wait till received */
	I2C_Timeout = I2C_TIMEOUT;
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
		if (--I2C_Timeout == 0x00) {
			return 1;
		}
	}

	/* Read data */
	data = I2Cx->DR;
	
	/* Return data */
	return data;
}

uint8_t I2C_Stop(I2C_TypeDef* I2Cx) {
	/* Wait till transmitter not empty */
	I2C_Timeout = I2C_TIMEOUT;
	while (((!(I2Cx->SR1 & I2C_SR1_TXE)) || (!(I2Cx->SR1 & I2C_SR1_BTF)))) {
		if (--I2C_Timeout == 0x00) {
			return 1;
		}
	}
	
	/* Generate stop */
	I2Cx->CR1 |= I2C_CR1_STOP;
	
	/* Return 0, everything ok */
	return 0;
}

void I2C_writeWords(uint8_t slaveAddr, uint8_t writeAddr, uint8_t length, uint16_t* data){
	
	int i;
	/* Send START condition */
	I2C_GenerateSTART(I2C1, ENABLE);

	/* Test on EV5 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

	/* Send MPU6050 address for write */
	I2C_Send7bitAddress(I2C1, slaveAddr, I2C_Direction_Transmitter);

	/* Test on EV6 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));	
	
	/* Send the MPU6050's internal address to write to */
	I2C_SendData(I2C1, writeAddr);

	/* Test on EV8 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
		
	/* Send the byte to be written */
	for(i=0; i<length*2; i++){
		
		I2C_SendData(I2C1, (uint8_t)(data[i++] >> 8));
		//delay_ms(25);

		I2C_SendData(I2C1, (uint8_t)data[i]);
		//delay_ms(25);

		
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	}
	
	/* Send STOP condition */
	I2C_GenerateSTOP(I2C1, ENABLE);
	//delay_ms(25);
}

void I2C_writeWord(uint8_t devAddr, uint8_t regAddr, uint16_t data) {
     I2C_writeWords(devAddr, regAddr, 1, &data);
}
void I2C_readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data)
{
    // 01101001 read byte
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    //    010   masked
    //   -> 010 shifted
    uint8_t mask,tmp;
    I2C_readByte(devAddr, regAddr, &tmp); 
    mask = ((1 << length) - 1) << (bitStart - length + 1);
    tmp &= mask;
    tmp >>= (bitStart - length + 1);
		*data = tmp;
}
void I2C_readByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data){
	
	I2C_readBytes(devAddr, regAddr, 1, data);
}

void I2C_readBytes(uint8_t slaveAddr, uint8_t readAddr, uint8_t NumByteToRead, uint8_t *pBuffer) {

/* While the bus is busy */
  while(I2C_GetFlagStatus(I2C1 ,I2C_FLAG_BUSY));
	
  /* Send START condition */
  I2C_GenerateSTART(I2C1, ENABLE);

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

  /* Send MPU6050 address for write */
  I2C_Send7bitAddress(I2C1, slaveAddr, I2C_Direction_Transmitter); 

  /* Test on EV6 and clear it *///takiliyor
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
		
  /* Clear EV6 by setting again the PE bit */
  I2C_Cmd(I2C1, ENABLE);

  /* Send the MPU6050's internal address to write to */
  I2C_SendData(I2C1, readAddr);
	
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Send STRAT condition a second time */
  I2C_GenerateSTART(I2C1, ENABLE);

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

  /* Send MPU6050 address for read */
  I2C_Send7bitAddress(I2C1, slaveAddr, I2C_Direction_Receiver);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	
  /* While there is data to be read */
  while(NumByteToRead)
  {
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(I2C1, DISABLE);

      /* Send STOP Condition */
      I2C_GenerateSTOP(I2C1, ENABLE);
    }
			
    /* Test on EV7 and clear it */
    if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
    {
      /* Read a byte from the MPU6050 */
      *pBuffer = I2C_ReceiveData(I2C1);

      /* Point to the next location where the byte read will be saved */
      pBuffer++;

      /* Decrement the read bytes counter */
      NumByteToRead--;
    }
  }

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(I2C1, ENABLE);
}


			void I2C_writeBit(uint8_t slave_address, uint8_t regAddr, uint8_t bitNum, uint8_t data)
				{
  
				uint8_t tmp;
				I2C_readByte(slave_address, regAddr, &tmp);  
				tmp = (data != 0) ? (tmp | (1 << bitNum)) : (tmp & ~(1 << bitNum));
					I2C_Write(I2C1,slave_address,regAddr,tmp); 
				}
