#include "Orientation.h"
#include "Quaternion.h"
#include "MPU6050.h"
#include "Math.h"



void MPU9250_TrueNorthVector( float declination_degrees, float declination_minutes, Vector_t* World_North)
// Set magnetometer "north" vector
{
	Quaternion_t q;
	Vector_t rot;
	float declination = (declination_degrees  + declination_minutes / 60.0f) * (float) RADIANS_PER_DEGREE;

	// Start vector pointing "real" north, along the X axis.
	Vector_Set(World_North, 1.0f, 0.0f, 0.0f);

	// Rotate according to the declination	
	Vector_Set(&rot, 0.0f, 0.0f, 1.0f);	
	Quat_SetAxisAndAngle( &q, &rot, declination);
	Quat_RotateVector(&q, World_North, World_North);
}



void MPU9250_GetGyros(float dt, Vector_t *rates, Quaternion_t *pGyroFrame)
	//Integrate gyros angle values (radians per second) our gyro frame. 
{
	// Get rotation axis from gyros, and rate in rads/sec	
	
  float rate = Vector_Length(rates);
	// Get a quat for the displacement this timestep did.
	Quaternion_t q_dot;
	Quat_SetAxisAndAngle( &q_dot, rates, rate * dt);

	// Multiply it in (This is euler integration)
	Quat_Multiply(pGyroFrame, pGyroFrame, &q_dot);
	Quat_Normalize( pGyroFrame);
}







