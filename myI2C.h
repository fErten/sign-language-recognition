
#ifndef MYI2C_H	
			
			#define MYI2C_H
			
			#include "stm32f4xx.h"                  // Device header
			#include "stm32f4xx_i2c.h"              // Keil::Device:StdPeriph Drivers:I2C
		
		//	* I2C1 settings*/		
#define I2C I2C1
#define I2C1_ACKNOWLEDGED_ADDRESS	I2C_AcknowledgedAddress_7bit
#define I2C1_MODE					I2C_Mode_I2C
#define I2C1_OWN_ADDRESS				0x00
#define I2C1_ACK						I2C_Ack_Disable
#define I2C1_DUTY_CYCLE				I2C_DutyCycle_2
#define I2C_TIMEOUT					20000

			
void myI2C_Init(void);

	
	
/**
 * Reads single byte from slave
 * I2Cx: I2C used
 * address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * reg: register to read from
 */
uint8_t I2C_Read(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg);



/**
 * Reads multi bytes from slave
 **I2Cx: I2C used
 * uint8_t address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * uint8_t reg: register to read from
 * uint8_t *data: pointer to data array to store data from slave
 * uint8_t count: how many bytes will be read
 */
void I2C_ReadMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t *data, uint16_t count);



/**
 * Writes single byte to slave
 * *I2Cx: I2C used
 * address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * reg: register to write to
 * data: data to be written
 */
void I2C_Write(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t data);



/**
 * Writes multi bytes to slave
 * *I2Cx: I2C used
 * address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * reg: register to write to
 * *data: pointer to data array to write it to slave
 * count: how many bytes will be written
 */
void I2C_WriteMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t *data, uint16_t count);



/**
 * @brief  I2C Start condition
 * @param  *I2Cx: I2C used
 * @param  address: slave address
 * @param  direction: master to slave or slave to master
 * @param  ack: ack enabled or disabled
 * @retval Start condition status
 * @note   For private use
 */
int16_t I2C_Start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction, uint8_t ack);

/**
 * @brief  Stop condition on I2C
 * @param  *I2Cx: I2C used
 * @retval Stop condition status
 * @note   For private use
 */
uint8_t I2C_Stop(I2C_TypeDef* I2Cx);

/**
 * @brief  Reads byte without ack
 * @param  *I2Cx: I2C used
 * @retval Byte from slave
 * @note   For private use
 */
uint8_t I2C_ReadNack(I2C_TypeDef* I2Cx);

/**
 * @brief  Reads byte with ack
 * @param  *I2Cx: I2C used
 * @retval Byte from slave
 * @note   For private use
 */
uint8_t I2C_ReadAck(I2C_TypeDef* I2Cx);

/**
 * @brief  Writes to slave
 * @param  *I2Cx: I2C used
 * @param  data: data to be sent
 * @retval None
 * @note   For private use
 */
void I2C_WriteData(I2C_TypeDef* I2Cx, uint8_t data);
void I2C_writeWords(uint8_t slaveAddr, uint8_t writeAddr, uint8_t length, uint16_t* data);
void I2C_writeWord(uint8_t devAddr, uint8_t regAddr, uint16_t data);
void I2C_readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data);
void I2C_readByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data);
void I2C_readBytes(uint8_t slaveAddr, uint8_t readAddr, uint8_t NumByteToRead, uint8_t *pBuffer);
void I2C_writeBit(uint8_t slave_address, uint8_t regAddr, uint8_t bitNum, uint8_t data);			
	
			
#endif
