
#include "Quaternion.h"
#include "Math.h"

/* Vector Maths */

void Vector_Set( Vector_t *v, float  x , float y , float z){

	 v->X = x;
	 v->Y = y;
	 v->Z = z;
}



float Vector_Dot( Vector_t *v1, Vector_t *v2){

	return ( (v1->X * v2->X) + (v1->Y * v2->Y) + (v1->Z * v2->Z) );
}


void Vector_Add(Vector_t *out, Vector_t*v1, Vector_t *v2){

	 out->X = v1->X + v2->X;
	 out->Y = v1->Y + v2->Y;
   out->Z = v1->Z + v2->Z;
	
}		

void Vector_Sub(Vector_t *out, Vector_t*v1, Vector_t *v2){

	 out->X = v1->X - v2->X;
	 out->Y = v1->Y - v2->Y;
   out->Z = v1->Z - v2->Z;
	
}	

void 	Vector_Mult( Vector_t *out, Vector_t *v1, Vector_t *v2){

	out->X = (v1->X * v2->X);
  out->Y = (v1->Y * v2->Y);
  out->Z = (v1->Z * v2->Z);
}



void Vector_Scale(Vector_t *out, Vector_t *in, float s){

	out->X = in->X * s;
  out->Y = in->Y * s;
  out->Z = in->Z * s;

}

void Vector_Cross(Vector_t *out, Vector_t *v1, Vector_t *v2){

	out->X = (v1->Y * v2->Z) - (v1->Z * v2->Y);
  out->Y = (v1->Z * v2->X) - (v1->X * v2->Z);
  out->Z = (v1->X * v2->Y) - (v1->Y * v2->X);

}


float Vector_Length(Vector_t *v ){

return  sqrtf( (v->X * v->X) + (v->Y * v->Y) + (v->Z * v->Z)) + 0.00001f;
}


float Vector_Normalize(Vector_t *out, Vector_t *in){
	
		float l = Vector_Length(in);

		in->X = in->X /l;
		in->Y = in->Y /l;
		in->Z = in->Z /l;
		
		return l;
}

void Vector_Invert(Vector_t *out, Vector_t *in){

		out->X = (-in->X);
		out->Y = (-in->Y);
		out->Z = (-in->Z);

}

/****************************************************************************************/


/*	Quaternion Maths */
void Quat_Set(Quaternion_t *q, float w, float x, float y, float z){
	
		q->W = w;
		q->X = x;
		q->Y = y;
		q->Z = z;
}



void Quat_Normalize	(Quaternion_t *q){

		
		
	float dist, squareRoot;

	squareRoot = sqrtf( (q->X * q->X) + (q->Y * q->Y) + (q->Z * q->Z) + (q->W * q->W) ) + 0.00001f;
	


		q->X /= squareRoot;
		q->Y /= squareRoot;
		q->Z /= squareRoot;
		q->W /= squareRoot;
}


void Quat_Inverse(Quaternion_t *q){
	
	float norm, invNorm;

	norm = sqrtf( (q->X * q->X) + (q->Y * q->Y) + (q->Z * q->Z)  + (q->W * q->W) ) + 0.00001f;
	
	invNorm = (float) (1.0f / norm);
	
	q->X = (-q->X) * invNorm;
	q->Y = (-q->Y) * invNorm;
	q->Z = (-q->Z) * invNorm;
	q->W = ( q->W) * invNorm;

}


void Quat_Multiply (Quaternion_t *res, Quaternion_t *q1, Quaternion_t *q2 ){
// Resulting quat will act as if it does Q2 first then Q1 
	
	float A, B, C, D, E, F, G, H;

	A = (q1->W + q1->X)*(q2->W + q2->X);
	B = (q1->Z - q1->Y)*(q2->Y - q2->Z);
	C = (q1->W - q1->X)*(q2->Y + q2->Z); 
	D = (q1->Y + q1->Z)*(q2->W - q2->X);
	E = (q1->X + q1->Z)*(q2->X + q2->Y);
	F = (q1->X - q1->Z)*(q2->X - q2->Y);
	G = (q1->W + q1->Y)*(q2->W - q2->Z);
	H = (q1->W - q1->Y)*(q2->W + q2->Z);


	res->W = B + (-E - F + G + H) * 0.50f;
	res->X = A - (E + F + G + H) * 0.50f; 
	res->Y = C + (E - F + G - H) * 0.50f; 
	res->Z = D + (E - F - G + H) * 0.50f;
		
}


void Quat_RotateVector(Quaternion_t *q, Vector_t *vOut, Vector_t *vIn){
	
	// v' = q v q^-1

	Quaternion_t qv,iq,r,qvo;
	qv.X = vIn->X;
	qv.Y = vIn->Y;
	qv.Z = vIn->Z;
	qv.W = 0.0f;			// Get v

	iq.X = q->X;
	iq.Y = q->Y;
	iq.Z = q->Z;
	iq.W = q->W;
	Quat_Inverse( &iq );		// get q^-1

	Quat_Multiply ( &r, q, &qv );
	Quat_Multiply ( &qvo, &r, &iq );	
	
	vOut->X = qvo.X;
	vOut->Y = qvo.Y;
	vOut->Z = qvo.Z;	
}

void Quat_SetAxisAndAngle(Quaternion_t *q, Vector_t *axis, float angle){
	
		float l = sqrt( (axis->X * axis->X) + (axis->Y * axis->Y) + (axis->Z * axis->Z ) ) + 0.00001f;

	float omega=0.5f*angle;
	
	
	q->X = (axis->X/l) * sin(omega);
	q->Y = (axis->Y/l) * sin(omega);
	q->Z = (axis->Z/l) * sin(omega);
	q->W = cos(omega);


}

void 			Quat_MagSetAxisAndAngle	(Quaternion_t *q, Vector_t *axis, float angle){

		float omega=0.5f*angle;
	
	q->X = sin(0);
	q->Y = sin(0);
	q->Z = sin(omega);
	q->W = cos(omega);

}


