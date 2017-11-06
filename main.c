#include "stm32f4xx.h"      
#include "myI2C.h"
#include "MPU6050.h"
#include "Filters.h"
#include "Orientation.h"
#include "Flex_Reading.h"
#include "myUsart.h"
#include "Helper_Functions.h"
#include "HDD47780.h"
#include "math.h"

#include <stdio.h>
#include <stdlib.h>

uint16_t ADC3ConvertedValue[5] = {0,0,0,0,0};
Finger_t Finger;

float Gyro_Sensitivity = 131.072f;
float Acc_Sensivitiy 	 = 16384;

float  rads = 0.0f;
uint8_t data;

MPU6050_t MPU6050_Data0;

float dT = 0.0025f;
Vector_t Gyro_Rates;
Vector_t Acc;

Vector_t Vector_Down;
Vector_t Vmeasured;
Vector_t Mag_Measured;
Vector_t Vcorrection;

Quaternion_t Gyro_Frame;
Quaternion_t q_erect_out;
float drift =0.0f;

	
uint32_t i=0;

char str[120];


int main(){
	
		/* Initialize I2C1, SCL: PB8 SDA: PB9*/	
		myI2C_Init(); 
		
		/* Initialize USART, TX: PB6 */
		myUsart_Init(); 
	
		/*Initializes MPU6050 and I2C Communication
		 Sets Accelometer and Gyroscope Sensitivies and other initial values */
		MPU6050_Init();
 
		/*Sets offset values*/
		MPUsetXAccelOffset	((430 << 1));
		MPUsetYAccelOffset	((755 << 1));
		MPUsetZAccelOffset	((615 << 1));
		MPUsetXGyroOffset		((34  << 1));
		MPUsetYGyroOffset		(-(23 << 1));
		MPUsetZGyroOffset		((6  << 1));
		
		Quat_Set(&Gyro_Frame, 1.0f, 0.0f, 0.0f, 0.0f );	
		Vector_Set(&Vector_Down, 0.0f,0.0f, 1.0f);
	
		Flex_Init();
		
		// Initialize the LCD
		LCD_Init();
		//Clear the LCD.
		LCD_Clear(); 
		
		/* Set timer configuration in order to read MPU in regular interval*/
		Set_Timer();

		
	while (1) {
				
						Finger.Thumb = ADC3ConvertedValue[0];
						Finger.Fore = ADC3ConvertedValue[1];
						Finger.Middle = ADC3ConvertedValue[2];
						Finger.Ring = ADC3ConvertedValue[3];
						Finger.Little = ADC3ConvertedValue[4];
		
		sprintf(str, "%da%db%dc%d\n "  , 
		((int32_t)(Gyro_Frame.W * 100000)), 
		((int32_t)(Gyro_Frame.X * 100000)), 
		((int32_t)(Gyro_Frame.Y * 100000)),
		((int32_t)(Gyro_Frame.Z * 100000)));
				
//				sprintf(str, "\n%f\n %f\n %f\n %f\n ", Gyro_Frame.W, Gyro_Frame.X*57.2957795f, Gyro_Frame.Y*57.2957795f, Gyro_Frame.Z*57.2957795f);
//				sprintf(str, "\n%d\n %d\n %d\n %d\n %d\n", Finger.Thumb, ADC3ConvertedValue[1], Finger.Middle, Finger.Ring, Finger.Little);
	
	
		USART_puts(USART1, str);
					i = 60000;
					while(i)
						{i--;}
					
							LCD_GoTo(0,0);       //Go to Line 0, position 0 of LCD.
	LCD_SendText ("imam hatipler");
	LCD_GoTo(1,2);       //Go to Line 1, position 2 (on the right) of LCD.
	LCD_SendText ("kapatilsin");
	delay_ms(1000);
		}						
}

void TIM2_IRQHandler(void)
{ 
	
	/* Check if interrupt is actualized*/
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 
	{
		/*Clear interrupt flag so that program can exit from interrupt*/
		TIM_ClearITPendingBit(TIM2 , TIM_IT_Update); 
	
	
		MPU6050_ReadAccelerometer(&MPU6050_Data0);
				Acc.X	=	( MPU6050_Data0.Accelerometer_X / Acc_Sensivitiy );
				Acc.Y	=	( MPU6050_Data0.Accelerometer_Y / Acc_Sensivitiy );
				Acc.Z	=	( MPU6050_Data0.Accelerometer_Z / Acc_Sensivitiy );
	
	 
			MPU6050_ReadGyroscope(&MPU6050_Data0);
				Gyro_Rates.X	=	( MPU6050_Data0.Gyroscope_X / Gyro_Sensitivity) * DEG_TO_RAD ;
				Gyro_Rates.Y	=	( MPU6050_Data0.Gyroscope_Y / Gyro_Sensitivity) * DEG_TO_RAD ;
				Gyro_Rates.Z	=	( MPU6050_Data0.Gyroscope_Z / Gyro_Sensitivity) * DEG_TO_RAD ;
		
				// Integrate gyros into our gyro frame. 
				MPU9250_GetGyros(dT, &Gyro_Rates, &Gyro_Frame);
			
				
				//Acceloremeter vector rotation from body frame into the world frame
				Quat_RotateVector(&Gyro_Frame, &Vmeasured, &Acc);
				
				Vector_Normalize(&Vmeasured, &Vmeasured);

				//Compare this vector in the earth frame for expected result
				Vector_Cross(&Vcorrection, &Vmeasured, &Vector_Down);
					
				
				// Normalized rotation axis 
				Vector_Normalize(&Vcorrection, &Vcorrection);
		
				
				  
				// Error angle 
				drift =  acosf( Vector_Dot( &Vmeasured, &Vector_Down));
				rads = 0.001f*drift;

			
				Quat_SetAxisAndAngle(&q_erect_out, &Vcorrection, rads);
			
				
				Quat_Multiply (&Gyro_Frame,&q_erect_out, &Gyro_Frame );
			
			
			}

	}	


