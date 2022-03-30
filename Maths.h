//This header contains function prototypes for the various mathematical functions found in Maths.cpp
#define SQU(x) x * x

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

void Cross_Product(float [], float [], float []);
float Dot_Product(float [], float[]);					//Calculate the dot produt
float Dot_Component(float, float);