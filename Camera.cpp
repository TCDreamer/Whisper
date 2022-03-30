#include<windows.h>
#include<gl\gl.h>
#include<gl\glu.h>
#include<stdio.h>

#include "Camera.h"
#include "Maths.h"
#include <math.h>


extern float rotateh;
extern float rotatev;
extern bool wp;
extern bool sp;

extern void gotoxy(int, int);




void camera::SetPrespective()
{

float maxpitch=10;
float maxheading=10;
float pitch=0;
float heading=0;
float maxfvelocity=100;
float fvelocity;
if(wp==true)  fvelocity=13;
else if(sp==true)  fvelocity=-13;

else  
	fvelocity =0;



  

		float rh = rotateh * (float)(3.141592736/180);
		float rv = rotatev * (float)(3.141592736/180);

	float Matrix[16] = {0, 0, 0, 0,
						0, 0, 0, 0,
						0, 0, 0, 0,
						0, 0, 0, 0};
	float q[4] = {0, 0, 0, 1};

	// Make the Quaternions that will represent our rotations
	qeulers(rv, 0, 0, qpitch);
	qeulers(0, rh, 0, qheading);
	
	// Combine the pitch and heading rotations and store the results in q
	Quant_Mult(qpitch, qheading, q);
	qmatrix(q, Matrix) ;

	// Let OpenGL set our new prespective on the world!
	glMultMatrixf(Matrix);
	
	// Create a matrix from the pitch Quaternion and get the j vector 
	// for our direction.
	qmatrix(qpitch, Matrix) ;
	direction[1] = Matrix[9];

	// Combine the heading and pitch rotations and make a matrix to get
	// the i and j vectors for our direction.
	Quant_Mult(qheading, qpitch, q);
	qmatrix(q, Matrix) ;
	direction[0] = Matrix[8];
	direction[2] = Matrix[10];

//6	gotoxy(0,12);printf( "Dru: x:%f, y:%f, z:%f, scalar:%f",direction[0], direction[1], direction[2], sqrt((direction[0]*direction[0])+(direction[1]*direction[1])+(direction[2]*direction[2])));
	//gotoxy(0,13);printf( "UPV: x:%f, y:%f, z:%f, scalar:%f",Matrix[1], Matrix[5], Matrix[9], sqrt((Matrix[1]*Matrix[1])+(Matrix[5]*Matrix[5])+(Matrix[9]*Matrix[9])));
//	gotoxy(0,13);printf( "UPV: x:%f, y:%f, z:%f, scalar:%f",Matrix[4], Matrix[5], Matrix[6], sqrt((Matrix[1]*Matrix[1])+(Matrix[5]*Matrix[5])+(Matrix[9]*Matrix[9])));

	//Extracts Look Unit Vector of camera from matrix;
	look[0] = Matrix[8];
	look[1] = Matrix[9];
	look[2] = Matrix[10];
	//Extracts Up Unit Vector of camera from matrix
	up[0] = Matrix[4];
	up[1] = Matrix[5];
	up[2] = Matrix[6];

	//Extracts Right Unit Vector of camera from matrix
	right[0] = Matrix[0];
	right[1] = Matrix[1];
	right[2] = Matrix[2];

//	gotoxy(0,14);printf( "RIV: x:%f, y:%f, z:%f, scalar:%f", right[0], right[1], right[2], sqrt((right[0]*right[0]) + (right[1]*right[1]) + (right[2]*right[2])));
	// Scale the direction by our speed.
	direction[0] *= fvelocity;
	direction[1] *= fvelocity;
	direction[2] *= fvelocity;

	// Increment our position by the vector
	position[0] += direction[0];
	position[1] += direction[1];
	position[2] += direction[2];

	// Translate to our new position.
	glTranslatef(-position[0], -position[1], position[2]);



//		gotoxy(0,15);printf( "%f %f %f %f",Matrix[0], Matrix[4], Matrix[8], Matrix[12]);
//		gotoxy(0,16);printf( "%f %f %f %f",Matrix[1], Matrix[5], Matrix[9], Matrix[13]);
//		gotoxy(0,17);printf( "%f %f %f %f",Matrix[2], Matrix[6], Matrix[10],Matrix[14]);
//		gotoxy(0,18);printf( "%f %f %f %f",Matrix[3], Matrix[7], Matrix[11],Matrix[15]);

}


/*char camera::
	CheckFrustum_Point(float pos[], float radius)
{
	float vector[3];
	float dot[3];
	dot[0] = 0;
	dot[1] = 0;
	dot[2] = 0;
	float y_bound;
	float x_bound;
	
	char inside = true;



	vector[0] = pos[0] - position[0];
	vector[1] = pos[1] - position[1];
	vector[2] = pos[2] - position[2];
	

	dot[0] = Dot_Product(vector, right);						//Calculates component of X vector
	dot[1] = Dot_Product(vector, up);							//Calculates component of Y vector
	dot[2] = Dot_Product(vector, look);							//Calculates component of Z vector


	y_bound = dot[2] * tan(rad);
	x_bound = y_bound * ratio;


	radius = radius / cos(rad);													//45 is the angle of the viewing window.

	if(dot[2] < (500 + radius) && dot[2] > (fnear - radius))					//High Detail
	{
		inside = '1';
	}
	else if(dot[2] >= (500 + radius) && dot[2] < ((ffar/2) + radius))			//Lower Detail
	{
		inside = '2';
	}
	else if(dot[2] < (ffar + radius) && dot[2] >= ((ffar/2) + radius))			//Low Detail
	{
		inside = '3';
	}

	else 
	{
		inside = '0';
	}

	if(dot[1] > (y_bound + radius) || dot[1] < (-y_bound - radius))
	{
		inside = '0';
	}


	if(dot[0] > (x_bound + radius)  || dot[0] < (-x_bound - radius))
	{
		inside = '0';
	}
	


	return inside;
}*/


bool camera::
	CheckFrustum_Point(float vector[], float radius) 
	//CheckFrustum_Point(float pos[], float radius)
{
	//float vector[3];
	float dot[3];
	dot[0] = 0;
	dot[1] = 0;
	dot[2] = 0;
	float y_bound;
	float x_bound;
	

	bool inside = true;


//	vector[0] = pos[0] - position[0];
//	vector[1] = pos[1] - position[1];
//	vector[2] = pos[2] - position[2];
	

	dot[0] = Dot_Product(vector, right);						//Calculates component of X vector
	dot[1] = Dot_Product(vector, up);							//Calculates component of Y vector
	dot[2] = Dot_Product(vector, look);							//Calculates component of Z vector


	y_bound = dot[2] * tan(rad);
	x_bound = y_bound * ratio;


	radius = radius / cos(rad);										//45 is the angle of the viewing window.

	if(dot[2] > (ffar + radius) || dot[2] < (fnear - radius))
	{
		inside = false;
	}
	if(dot[1] > (y_bound + radius) || dot[1] < (-y_bound - radius))
	{
		inside = false;
	}


	if(dot[0] > (x_bound + radius)  || dot[0] < (-x_bound - radius))
	{
		inside = false;
	}
	


	return inside;
}


char camera::
	CheckFrustum_Terrain(float pos[], float radius)
{
	float vector[3];
	char inside;

	vector[0] = pos[0] - position[0];
	vector[1] = pos[1] - position[1];
	vector[2] = pos[2] - position[2];
	
	
	if(CheckFrustum_Point(vector, radius) == true)
	{
		float p =	sqrt(((vector[0]*vector[0]) + (vector[1] * vector[1]) + (vector[2] * vector[2])));

		if(p < (10 + radius) && p > (fnear - radius))					//High Detail
		{
			inside = '1';
		}
		else if(p >= (10 + radius) && p < (10000 + radius))			//Lower Detail
		{
			inside = '2';
		}
		else if(p < ((rfar/2) + radius) && p >= (10000 + radius))			//Low Detail
		{
			inside = '3';
		}
		else if(p < (rfar + radius) && p >= ((rfar/2) + radius))			//Low Detail
		{
			inside = '4';
		}

		else 
		{
			inside = '0';
		}
	}
	else
	{
		inside = '0';
	}

	return inside;

}