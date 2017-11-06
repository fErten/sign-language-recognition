#ifndef FILTERS_H
	#define FILTERS_H

#include <stdint.h>


#define DEG_TO_RAD    0.0174532f
#define RAD_TO_DEG    57.2957795f

typedef struct
{
    
    // The time delta between updates. deltaT = 1/(sampling frequency)
    float deltaT;
    
    // Weighting factor 
    float alpha;
   
    // The most recent accelerometer readings.
    float Ax, Ay, Az;
 
    // The most recent gyroscope readings.
    float Gx, Gy, Gz;
  
    // Accelerometer angles in relation to the X and Y axes.
    float accelAngleX, accelAngleY;
   
    // Comp. filter angle output in relation to the X and Y axes.
    float compAngleX, compAngleY;
}
SixAxis;



// Prototypes


// Comp Filter Initialize
// Description:
//     Initializes the complementary filter.
extern void CompInit(SixAxis *filter, float deltaT, float tau);


// Complementary Filter Start
// Description:Should be called once before CompUpdate can be called at the next interval.
//      CompAccelUpdate must be called before this function.
extern void CompStart(SixAxis *filter);


// Complementary Filter Update
// Description:
//      Must be called on a regular interval specified by deltaT.
extern void CompUpdate(SixAxis *filter);


// Complementary Filter Angles Get
// Description:
//      Acquires the angles in radians relative to ground along the positive
//      X and Y axes.
extern void CompAnglesGet(SixAxis *filter, float *XAngle, float *YAngle);


// Complementary Filter Accelerometer Update
// Description:
//      Updates the comp. filter with new accelerometer values.
extern void CompAccelUpdate(SixAxis *filter, float accelX, float accelY, 
                            float accelZ);
                               

// Complementary Filter Gyroscope Update
// Description:
//      Updates the comp. filter with new gyroscope values.                              
extern void CompGyroUpdate(SixAxis *filter, float gyroX, float gyroY, float gyroZ);

#endif
