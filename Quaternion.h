#ifndef QUATERNION_H
	#define QUATERNION_H


	
	#define RADIANS_PER_DEGREE  0.0174532925199432957692f
	#define PI                  3.1415926535897932384626433832795f
	
	
/* 
Struct defines for Quaternion multiplications, 
*/	

	typedef struct{
		
		float W;
		float X;
		float Y;
		float Z;
	}Quaternion_t;
	
	typedef struct{
	
		float X;
		float Y;
		float Z;
	}Vector_t;
	
/*
Quaternion_t; quaternion vector which read values from MPU
Vector_t; vector which defines rotation around x,y,z axes
*/
	
/************************************************************************************************/	
	
/* Vector Maths */

void 			Vector_Set				(Vector_t *v, float  x , float y , float z);			
float 		Vector_Dot				(Vector_t *v1, Vector_t *v2);  																	// Vector dot product
void 			Vector_Add				(Vector_t *out, Vector_t*v1, Vector_t *v2);											// Adds to vectors
void 			Vector_Sub				(Vector_t *out, Vector_t*v1, Vector_t *v2);
void 			Vector_Mult				(Vector_t *out, Vector_t *v1, Vector_t *v2);   						 		 	// Vector multiply
void 			Vector_Scale			(Vector_t *out, Vector_t *in, float s);
void			Vector_Cross			(Vector_t *out, Vector_t *v1, Vector_t *v2);	  								// Cross product
float 		Vector_Length			(Vector_t *v );																									// Length of vector, norm
float 		Vector_Normalize	(Vector_t *out, Vector_t *in);								 									// Unit vectors, normalization
void 			Vector_Invert			(Vector_t *out, Vector_t *in);																	// Inverse vector

/************************************************************************************************/	
	
/* Quaternion Maths */

void 			Quat_Set							(Quaternion_t *q, float w, float x, float y, float z);	
void 			Quat_Normalize				(Quaternion_t *q); 																					// Unit quaternions, normalization
void 			Quat_Inverse					(Quaternion_t *q);														  						// Inverse of quaternion
void 			Quat_Multiply 				(Quaternion_t *res, Quaternion_t *q1, Quaternion_t *q2 );		// Quaternion Multiplication
void 			Quat_RotateVector			(Quaternion_t *q, Vector_t *vOut, Vector_t *vIn);						// Rotation vector of q aroun v
void 			Vector_RotateQuat			( Vector_t *v, Quaternion_t *qOut, Vector_t *vIn);
void 			Quat_SetAxisAndAngle	(Quaternion_t *q, Vector_t *axis, float angle);	
void 			Quat_MagSetAxisAndAngle	(Quaternion_t *q, Vector_t *axis, float angle);	

#endif
