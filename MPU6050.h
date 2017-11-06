#ifndef MPU6050_H
	#define MPU6050_H

/**
MPU6050		STM32F4x Discovery

SCL			PB8			Clock line for I2C
SDA			PB9			Data line for I2C
VCC			3.3V
GND			GND
AD0			-			If pin is low, I2C address is 0xD0, if pin is high, the address is 0xD2

 */

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_i2c.h"
#include "myI2C.h"
#include "stdbool.h"
#include "stdint.h"
		
		#define TRUE 1
		#define FALSE 0


#define	MPU6050_I2C					I2C1


/* Who I am register value */
#define MPU6050_I_AM				0x68

/* MPU6050 registers */
#define MPU6050_SMPLRT_DIV						0x19
#define MPU6050_CONFIG								0x1A
#define MPU6050_GYRO_CONFIG						0x1B
#define MPU6050_ACCEL_CONFIG					0x1C
#define MPU6050_MOTION_THRESH					0x1F
#define MPU6050_FIFO_EN          			0x23
#define MPU6050_I2C_MST_CTRL     			0x24
#define MPU6050_I2C_SLV0_ADDR    			0x25
#define MPU6050_I2C_SLV0_REG     			0x26
#define MPU6050_I2C_SLV0_CTRL    			0x27
#define MPU6050_I2C_SLV1_ADDR    			0x28
#define MPU6050_I2C_SLV1_REG     			0x29
#define MPU6050_I2C_SLV1_CTRL    			0x2A
#define MPU6050_I2C_SLV2_ADDR    			0x2B
#define MPU6050_I2C_SLV2_REG     			0x2C
#define MPU6050_I2C_SLV2_CTRL    			0x2D
#define MPU6050_I2C_SLV3_ADDR    			0x2E
#define MPU6050_I2C_SLV3_REG     			0x2F
#define MPU6050_I2C_SLV3_CTRL    			0x30
#define MPU6050_I2C_SLV4_ADDR    			0x31
#define MPU6050_I2C_SLV4_REG     			0x32
#define MPU6050_I2C_SLV4_DO      			0x33
#define MPU6050_I2C_SLV4_CTRL   			0x34
#define MPU6050_I2C_SLV4_DI      			0x35
#define MPU6050_I2C_MST_STATUS   			0x36
#define MPU6050_INT_PIN_CFG						0x37
#define MPU6050_INT_ENABLE						0x38
#define MPU6050_INT_STATUS						0x3A
#define MPU6050_ACCEL_XOUT_H					0x3B
#define MPU6050_ACCEL_XOUT_L					0x3C
#define MPU6050_ACCEL_YOUT_H					0x3D
#define MPU6050_ACCEL_YOUT_L					0x3E
#define MPU6050_ACCEL_ZOUT_H					0x3F
#define MPU6050_ACCEL_ZOUT_L					0x40
#define MPU6050_TEMP_OUT_H						0x41
#define MPU6050_TEMP_OUT_L						0x42
#define MPU6050_GYRO_XOUT_H						0x43
#define MPU6050_GYRO_XOUT_L						0x44
#define MPU6050_GYRO_YOUT_H						0x45
#define MPU6050_GYRO_YOUT_L						0x46
#define MPU6050_GYRO_ZOUT_H						0x47
#define MPU6050_GYRO_ZOUT_L						0x48
#define MPU6050_I2C_SLV0_DO      			0x63
#define MPU6050_I2C_SLV1_DO      			0x64
#define MPU6050_I2C_SLV2_DO      			0x65
#define MPU6050_I2C_SLV3_DO      			0x66
#define MPU6050_I2C_MST_DELAY_CTRL   	0x67
#define MPU6050_SIGNAL_PATH_RESET			0x68
#define MPU6050_MOT_DETECT_CTRL				0x69
#define MPU6050_USER_CTRL							0x6A
#define MPU6050_PWR_MGMT_1						0x6B
#define MPU6050_PWR_MGMT_2						0x6C
#define MPU6050_FIFO_COUNTH						0x72
#define MPU6050_FIFO_COUNTL						0x73
#define MPU6050_FIFO_R_W							0x74
#define MPU6050_WHO_AM_I							0x75

// MPU WakeUp
#define MPU6050_PWR1_SLEEP_BIT          6
#define MPU6050_USERCTRL_DMP_EN_BIT			7
#define MPU6050_USERCTRL_FIFO_EN_BIT		6

/* Calibration registers - OFFSET */
#define MPU6050_XA_OFFS_H        			0x06 
#define MPU6050_XA_OFFS_L							0x07
#define MPU6050_YA_OFFS_H        			0x08 
#define MPU6050_YA_OFFS_L    					0x09
#define MPU6050_ZA_OFFS_H        			0x0A 
#define MPU6050_ZA_OFFS_L     				0x0B
#define MPU6050_XG_OFFS_H     				0x13 
#define MPU6050_XG_OFFS_L     				0x14
#define MPU6050_YG_OFFS_H     				0x15 
#define MPU6050_YG_OFFS_L     				0x16
#define MPU6050_ZG_OFFS_H     				0x17 
#define MPU6050_ZG_OFFS_L 						0x18


  // FS_SEL 2 bit valuse in order to scale gyro
typedef enum {
	MPU6050_Gyroscope_250s = 0x00,  //	+- 250 degrees/s 
	MPU6050_Gyroscope_500s = 0x01,  //	+- 500 degrees/s 
	MPU6050_Gyroscope_1000s = 0x02, // 	+- 1000 degrees/s 
	MPU6050_Gyroscope_2000s = 0x03  //  +- 2000 degrees/s 
} MPU6050_Gyroscope_t;

//
typedef enum {
	MPU6050_Accelerometer_2G = 0x00, /*!< Range is +- 2G */
	MPU6050_Accelerometer_4G = 0x01, /*!< Range is +- 4G */
	MPU6050_Accelerometer_8G = 0x02, /*!< Range is +- 8G */
	MPU6050_Accelerometer_16G = 0x03 /*!< Range is +- 16G */
} MPU6050_Accelerometer_t;



 //	Main MPU6050 structure
typedef struct{
	
	int16_t Accelerometer_X; /*!< Accelerometer value X axis */
	int16_t Accelerometer_Y; /*!< Accelerometer value Y axis */
	int16_t Accelerometer_Z; /*!< Accelerometer value Z axis */
	int16_t Gyroscope_X;     /*!< Gyroscope value X axis */
	int16_t Gyroscope_Y;     /*!< Gyroscope value Y axis */
	int16_t Gyroscope_Z;     /*!< Gyroscope value Z axis */
	float Temperature;       /*!< Temperature in degrees */
} MPU6050_t;


typedef enum {
	MPU6050_Result_Ok = 0x00,          /*!< Everything OK */
	MPU6050_Result_DeviceNotConnected, /*!< There is no device with valid slave address */
	MPU6050_Result_DeviceInvalid       /*!< Connected device with address is not MPU6050 */
} MPU6050_Result_t;

/**
 * Initializes MPU6050 and I2C Communication
 * If you set AD0 pin to low,  MPU Device Adrees = 0x68
 * If you set AD0 pin to low,  MPU Device Adrees = 0x69
 * AccelerometerSensitivity: Set accelerometer sensitivity @ref MPU6050_Accelerometer_t enumeration
 * GyroscopeSensitivity: Set gyroscope sensitivity @ref_MPU6050_Gyroscope_t enumeration
 */
void MPU6050_Init(void);



/**
 * Reads accelerometer data from sensor
 * *DataStruct: Pointer to @ref MPU6050_t structure to store data to
 */
MPU6050_Result_t MPU6050_ReadAccelerometer(MPU6050_t* DataStruct);



/**
 * Reads gyroscope data from sensor
 **DataStruct: Pointer to @ref MPU6050_t structure to store data to
 */
MPU6050_Result_t MPU6050_ReadGyroscope(MPU6050_t* DataStruct);



/**
 * Reads temperature data from sensor
 * *DataStruct: Pointer to @ref MPU6050_t structure to store data to
 */
MPU6050_Result_t MPU6050_ReadTemperature(MPU6050_t* DataStruct);


/**
 * Sets offset values
 * *DataStruct: Pointer to @ref MPU6050_t structure to store data to
 */
void MPUsetXAccelOffset(int16_t offset);
void MPUsetYAccelOffset(int16_t offset);
void MPUsetZAccelOffset(int16_t offset);
void MPUsetXGyroOffset(int16_t offset);
void MPUsetYGyroOffset(int16_t offset);
void MPUsetZGyroOffset(int16_t offset);


#endif
