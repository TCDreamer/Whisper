#include <math.h>

#include "Maths.h"
#include "Timer.h"

void Set_Rotation(float, float, float, float[]);
void Set_Position(float, float, float, float[]);
void Matrix_Mult(float [], float [], float []);
void Matrix_Copy(float [], float []);
void invtrans(float [], float[]);
void invrotate(float[], float []);
void transvert(float [], float [], float[]);
void normrot(float [], float [], float[]);
void getqeulers(float[], float []);

void qeulers(float , float , float , float []);
void SLERP(float [],float [], float , float []);
void LERP(float[], float[], float,float[]);
void qmatrix(float[], float []);
void Quant_Mult(float [], float [], float []);

float distance(float[], int[], float);

extern float rotateh;
extern float rotatev;
extern bool wp;
extern bool sp;


/////////////////////////////
//set rotation matrix
/////////////////////////////
void Set_Rotation(float mx, float my, float mz, float matrix[])
{
//static int j=1;	



	double cx = cos(mx);
	double sx = sin(mx);
	double cy = cos(my);
	double sy = sin(my);
	double cz = cos(mz);
	double sz = sin(mz);


	
	matrix[0] = (float)(cy * cz);
	matrix[1] = (float)(cy * sz);
	matrix[2] = (float)(-sy);

	matrix[4] = (float)(sx * sy * cz - cx * sz);
	matrix[5] = (float)(sx * sy * sz + cx * cz);
	matrix[6] = (float)(sx * cy);

	matrix[8] = (float)(cx * sy * cz + sx * sz);
	matrix[9] = (float)(cx * sy * sz - sx * cz);
	matrix[10] = (float)(cx * cy);

	matrix[15] = 1.0f;


}//end set rotation

////////////////////////////////
//set translation matrix
////////////////////////////////
void Set_Position(float mx, float my, float mz, float matrix[])
{
	
	matrix[12] = mx;
	matrix[13] = my;
	matrix[14] = mz;
}//end set position(translation)

///////////////////////////////
//multiply matrices
///////////////////////////////

void Matrix_Mult(float m1[],float m2[],float c[])//a[], float b[], float c[])
{


	c[0]=m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
	c[1]=m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
	c[2]=m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
	c[3]=m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3];

	c[4]=m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
	c[5]=m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
	c[6]=m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
	c[7]=m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7];

	c[8]=m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
	c[9]=m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
	c[10]=m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
	c[11]=m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11];

	c[12]=m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];
	c[13]=m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];
	c[14]=m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
	c[15]=m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];




}//end matrix mult

//////////////////////////////////////////
//copy matrix
/////////////////////////////////////////
//a = array to be copied
//b = array to copy to
void Matrix_Copy(float a[], float b[])
{
	b[0]=a[0];
	b[1]=a[1];
	b[2]=a[2];
	b[3]=a[3];
	b[4]=a[4];
	b[5]=a[5];
	b[6]=a[6];
	b[7]=a[7];
	b[8]=a[8];
	b[9]=a[9];
	b[10]=a[10];
	b[11]=a[11];
	b[12]=a[12];
	b[13]=a[13];
	b[14]=a[14];
	b[15]=a[15];
}

//////////////////////
//inverse translate
//////////////////////
void invtrans(float vector[], float mat[])
{
	vector[0] = vector[0] - mat[12];
	vector[1] = vector[1] - mat[13];
	vector[2] = vector[2] - mat[14];

//	vector[0] = vector[0];
//	vector[1] = vector[1];
//	vector[2] = vector[2];
}
/////////end inverse translate


void invrotate(float vector[], float mat[])
{


	
	float tmp[3];

	tmp[0] = vector[0] * mat[0] + vector[1] * mat[1] + vector[2] * mat[2];
	tmp[1] = vector[0] * mat[4] + vector[1] * mat[5] + vector[2] * mat[6];
	tmp[2] = vector[0] * mat[8] + vector[1] * mat[9] + vector[2] * mat[10];



	vector[0] = tmp[0];
	vector[1] = tmp[1];
	vector[2] = tmp[2];
	
}



void transvert(float vert[], float m[],float vec[])
{
	float tmp[3];
		tmp[0] = vert[0] * m[0] + vert[1] * m[4] + vert[2] * m[8] + m[12];

		tmp[1] = vert[0] * m[1] + vert[1] * m[5] + vert[2] * m[9] + m[13];

	    tmp[2] = vert[0] * m[2] + vert[1] * m[6] + vert[2] * m[10]+ m[14];

		vec[0]=tmp[0];
		vec[1]=tmp[1];
		vec[2]=tmp[2];
		
}

void normrot(float vert[], float m[],float vec[])
{
	float tmp[3];
		tmp[0] = vert[0] * m[0] + vert[1] * m[4] + vert[2] * m[8];

		tmp[1] = vert[0] * m[1] + vert[1] * m[5] + vert[2] * m[9];
	    tmp[2] = vert[0] * m[2] + vert[1] * m[6] + vert[2] * m[10];

		vec[0]=tmp[0];
		vec[1]=tmp[1];
		vec[2]=tmp[2];
		
}

void getqeulers(float ang[], float quat[])
{
	qeulers(ang[0], ang[1], ang[2], quat);
}



void qeulers(float fX, float fY, float fZ, float quat[])
{

	
	float dSY = (float)sin(fZ * 0.5f);
	float dSP = (float)sin(fY * 0.5f);
	float dSR = (float)sin(fX * 0.5f);
	float dCY = (float)cos(fZ * 0.5f);
	float dCP = (float)cos(fY * 0.5f);
	float dCR = (float)cos(fX * 0.5f);

	quat[0] = dSR * dCP * dCY - dCR * dSP * dSY;
	quat[1] = dCR * dSP * dCY + dSR * dCP * dSY;
	quat[2] = dCR * dCP * dSY - dSR * dSP * dCY;
	quat[3] = dCR * dCP * dCY + dSR * dSP * dSY;

	

}

void SLERP(float rQuat0[],float rQuat1[], float fInterp, float fQuat[])
{
//	float * q0 = rQuat0.Get();
//	float * q1 = rQuat1.Get();

	//Calculate the dot product
//	float fDot = rQuat0[0] * rQuat1[0] + rQuat0[1] * rQuat1[1] + rQuat0[2] * rQuat1[2] + rQuat0[3] * rQuat1[3];
	float fDot = rQuat0[0] * rQuat1[0] + rQuat0[1] * rQuat1[1] + rQuat0[2] * rQuat1[2] + rQuat0[3] * rQuat1[3];

	if(fDot < 0.0f)
	{
		//rQuat1 = -rQuat1;
		rQuat1[0] = -rQuat1[0];
		rQuat1[1] = -rQuat1[1];
		rQuat1[2] = -rQuat1[2];
		rQuat1[3] = -rQuat1[3];
		fDot = -fDot;
	}


	if(fDot < 1.00001f && fDot > 0.99999f)
	{
		LERP(rQuat0, rQuat1, fInterp,fQuat);
	}
	else
	{
	//calculate the angle between the quaternions 
	float fTheta = acosf(fDot);

	fQuat[0]= (rQuat0[0] * sinf(fTheta * (1 - fInterp)) + rQuat1[0] * sinf(fTheta * fInterp))/sinf(fTheta);
	fQuat[1]= (rQuat0[1] * sinf(fTheta * (1 - fInterp)) + rQuat1[1] * sinf(fTheta * fInterp))/sinf(fTheta);
	fQuat[2]= (rQuat0[2] * sinf(fTheta * (1 - fInterp)) + rQuat1[2] * sinf(fTheta * fInterp))/sinf(fTheta);
	fQuat[3]= (rQuat0[3] * sinf(fTheta * (1 - fInterp)) + rQuat1[3] * sinf(fTheta * fInterp))/sinf(fTheta);

	}
}


void LERP(float rQuat0[], float rQuat1[], float fInterp, float ret[])
{
//	float ret[4];
	ret[0]=(((rQuat1[0] - rQuat0[0]) * fInterp) + rQuat0[0]);
	ret[1]=(((rQuat1[1] - rQuat0[1]) * fInterp) + rQuat0[1]);
	ret[2]=(((rQuat1[2] - rQuat0[2]) * fInterp) + rQuat0[2]);
	ret[3]=(((rQuat1[3] - rQuat0[3]) * fInterp) + rQuat0[3]);
	//ret.Normalize();
	float Magnitude = sqrtf(SQU(ret[0]) + SQU(ret[1]) + SQU(ret[2]) + SQU(ret[3]));
	
	float fInvMag = 1/Magnitude;
	if(fInvMag != 1.0f)
	{
		ret[0] *= fInvMag;
		ret[1] *= fInvMag;
		ret[2] *= fInvMag;
		ret[3] *= fInvMag;
	}
//	return ret;
}

void qmatrix(float fquant[], float fmat[]) 
{
	

	fmat[0] = 1.0f - 2.0f * (SQU(fquant[1]) + SQU(fquant[2])); 
	fmat[1] = 2.0f * fquant[0] * fquant[1] + 2.0f * fquant[2] * fquant[3];
	fmat[2] = 2.0f * fquant[0] * fquant[2] - 2.0f * fquant[1] * fquant[3];

	fmat[4] = 2.0f * fquant[0] * fquant[1] - 2.0f * fquant[2] * fquant[3];
	fmat[5] = 1.0f - 2.0f * (SQU(fquant[0]) + SQU(fquant[2]));
	fmat[6] = 2.0f * fquant[1] * fquant[2] + 2.0f * fquant[0] * fquant[3];
	
	fmat[8] = 2.0f * fquant[2] * fquant[0] + 2.0f * fquant[1] * fquant[3];
	fmat[9] = 2.0f * fquant[1] * fquant[2] - 2.0f * fquant[0] * fquant[3];
	fmat[10] = 1.0f - 2.0f * (SQU(fquant[0]) + SQU(fquant[1]));
	
	fmat[15] = 1.0f;

	
}



void cTimer::init(void)
{
	LARGE_INTEGER Freq;// = 0;
	QueryPerformanceFrequency(&Freq);
	QueryPerformanceCounter(&StartTime);
	InvTicksPerSec = (float)1/Freq.QuadPart;
	InvTicksPerMs = (float)1000/Freq.QuadPart;
	LastTime = 0;


	
	getmili();

}


unsigned int cTimer::getmili(void)
{
	LARGE_INTEGER NewTime;// = 0;
	unsigned int uiMs;
	
	QueryPerformanceCounter(&NewTime);
	uiMs = (unsigned int)((NewTime.QuadPart - LastTime) * InvTicksPerMs);
	LastTime = NewTime.QuadPart;
					

	
	return uiMs;
}

float cTimer::getsec(void)
{

	
	return (float)(getmili() * 0.001f);
}

void Quant_Mult(float q1[], float q2[], float q[])
{

	q[3] = q1[3]*q2[3] - q1[0]*q2[0] - q1[1]*q2[1] - q1[2]*q2[2];
	q[0] = q1[3]*q2[0] + q1[0]*q2[3] + q1[1]*q2[2] - q1[2]*q2[1];
	q[1] = q1[3]*q2[1] + q1[1]*q2[3] + q1[2]*q2[0] - q1[0]*q2[2];
	q[2] = q1[3]*q2[2] + q1[2]*q2[3] + q1[0]*q2[1] - q1[1]*q2[0];


}

void Cross_Product(float v1[], float v2[], float v3[])
{
	
	v3[0] = ( v1[1] * v2[2]) - (v1[2] * v2[1]);
	v3[1] = ( v1[2] * v2[0]) - (v1[0] * v2[2]);
	v3[2] = ( v1[0] * v2[1]) - (v1[1] * v2[0]);

}

float Dot_Product(float v1[], float v2[])
{
	float s = ((v1[0] * v2[0]) + (v1[1] * v2[1]) + (v1[2] * v2[2]));
	
	return s;
}

float Dot_Component(float v1, float v2)
{
	float s = v1 * v2;

	return s;
}