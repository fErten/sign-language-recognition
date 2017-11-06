#ifndef ORIENTATION_H
	#define ORIENTATION_H
	
	#include "Quaternion.h"
	#include "stdint.h"
	#include "stdbool.h"
	
	typedef int BOOL;
	#define true 1
	#define false 0
	
	void MPU9250_TrueNorthVector( float declination_degrees, float declination_minutes, Vector_t* World_North);
	void Orientation_Init(void);
	void MPU9250_GetGyros(float dt, Vector_t *rates, Quaternion_t *pGyroFrame);
	BOOL IMU_GetErectorQuat( Quaternion_t *q_erect_out, Quaternion_t *frame, Vector_t *v_measured, Vector_t *v_reference, float rads_sec, float dt);


	
	
	
#endif
