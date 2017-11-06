
#include "stm32f4xx.h"                  // Device header
#include "MPU6050.h"

/* I2C slave adress contains 7 bit so that 
MPU device adrees 1 bit shifted to left */
#define MPU6050_I2C_ADDR  (0x68 <<1)

void MPU6050_Init(void) {
 
	I2C_Write(I2C1, 0x68<<1, 0x6B, 0x00);
	I2C_Write(I2C1, 0x68<<1, 0x1B, 0x00);
	I2C_Write(I2C1, 0x68<<1, 0x1C, 0x08);

}

		

MPU6050_Result_t MPU6050_ReadAccelerometer(MPU6050_t* DataStruct) {
	uint8_t data[6];
	
	/* Read accelerometer data */
	I2C_ReadMulti(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_ACCEL_XOUT_H, data, 6);
	
	/* Format */
	DataStruct->Accelerometer_X = (int16_t)(data[0] << 8 | data[1]);	
	DataStruct->Accelerometer_Y = (int16_t)(data[2] << 8 | data[3]);
	DataStruct->Accelerometer_Z = (int16_t)(data[4] << 8 | data[5]);
	
	/* Return OK */
	return MPU6050_Result_Ok;
}

MPU6050_Result_t MPU6050_ReadGyroscope(MPU6050_t* DataStruct) {
	uint8_t data[6];
	
	/* Read gyroscope data */
	I2C_ReadMulti(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_GYRO_XOUT_H, data, 6);
	
	/* Format */
	DataStruct->Gyroscope_X = (int16_t)(data[0] << 8 | data[1]);
	DataStruct->Gyroscope_Y = (int16_t)(data[2] << 8 | data[3]);
	DataStruct->Gyroscope_Z = (int16_t)(data[4] << 8 | data[5]);

	/* Return OK */
	return MPU6050_Result_Ok;
}

MPU6050_Result_t MPU6050_ReadTemperature(MPU6050_t* DataStruct) {
	uint8_t data[2];
	int16_t temp;
	
	/* Read temperature */
	I2C_ReadMulti(MPU6050_I2C, MPU6050_I2C_ADDR, MPU6050_TEMP_OUT_H, data, 2);
	
	/* Format temperature */
	temp = (data[0] << 8 | data[1]);
	DataStruct->Temperature = (float)((int16_t)temp / (float)340.0 + (float)36.53);
	
	/* Return OK */
	return MPU6050_Result_Ok;
}

void MPUsetXAccelOffset(int16_t offset) {
	int8_t reg = 0;
		  reg = ((offset & 0xFF00) >> 8);	
			I2C_Write(I2C1, MPU6050_I2C_ADDR, MPU6050_XA_OFFS_H, reg);
			
			reg = (offset & 0x00FF);	
		  I2C_Write(I2C1, MPU6050_I2C_ADDR, MPU6050_XA_OFFS_L, reg);

				reg = 0;
}
			
			void MPUsetYAccelOffset(int16_t offset) {
			int8_t reg = 0;
		  reg = ((offset & 0xFF00) >> 8);	
			I2C_Write(I2C1, MPU6050_I2C_ADDR, MPU6050_YA_OFFS_H, reg);
			reg = (offset & 0x00FF);	
		  I2C_Write(I2C1, MPU6050_I2C_ADDR, MPU6050_YA_OFFS_L, reg);

				reg = 0;
		}
			void MPUsetZAccelOffset(int16_t offset) {
				int8_t reg = 0;
			reg = ((offset & 0xFF00) >> 8);	
			I2C_Write(I2C1, MPU6050_I2C_ADDR, MPU6050_ZA_OFFS_H, reg);
			reg = (offset & 0x00FF);	
		  
			I2C_Write(I2C1, MPU6050_I2C_ADDR, MPU6050_ZA_OFFS_L, reg);

				reg = 0;		}
			
				
			void MPUsetXGyroOffset(int16_t offset) {
				int8_t reg = 0;
			reg = ((offset & 0xFF00) >> 8);	
			I2C_Write(I2C1, MPU6050_I2C_ADDR, MPU6050_XG_OFFS_H, reg);
			reg = (offset & 0x00FF);	
		  
			I2C_Write(I2C1, MPU6050_I2C_ADDR, MPU6050_XG_OFFS_L, reg);

				reg = 0;	
		}
			void MPUsetYGyroOffset(int16_t offset) {
				int8_t reg = 0;
			reg = ((offset & 0xFF00) >> 8);	
			I2C_Write(I2C1, MPU6050_I2C_ADDR, MPU6050_YG_OFFS_H, reg);
			reg = (offset & 0x00FF);	
		  
			I2C_Write(I2C1, MPU6050_I2C_ADDR, MPU6050_YG_OFFS_L, reg);

				reg = 0;
		}
		void MPUsetZGyroOffset(int16_t offset) {
			int8_t reg = 0;
			reg = ((offset & 0xFF00) >> 8);	
			I2C_Write(I2C1, MPU6050_I2C_ADDR, MPU6050_ZG_OFFS_H, reg);
			reg = (offset & 0x00FF);	
		  
			I2C_Write(I2C1, MPU6050_I2C_ADDR, MPU6050_ZG_OFFS_L, reg);

				reg = 0;
		}
		