/////////////////////////////////////////////////////////////////////////////////////////////
//Obviously it inputs commands from the keyboard and maybe eventually the mouse or joystick//
/////////////////////////////////////////////////////////////////////////////////////////////

#include<windows.h>
#include<stdio.h>
#include<math.h>

void gotoxy(int, int);
extern bool  *key;

extern float zoom;
extern float height;
extern float rotateh;
extern float rotatev;
extern float rotater;
extern bool wp;
extern bool sp;

extern int cammode = 0;
bool campress = false;
bool opress = false;
bool ppress = false;
int step = 8;


extern bool loop;
bool j=false;




//dbones activates/deactivates the drawing of bones of models
bool dbones=false;

void keyinput(void)
{

		if (key['N']) zoom=zoom+10;
		if (key['M']) zoom=zoom-10;
		if (key['Z']) zoom++;
		if (key['X']) zoom--;
	//	if (key['E']) rotater++;
		if (key['Q']) rotater--;
	//	if (key['R']) height++;
		if (key['F']) height--;
		if (key[VK_RIGHT]) rotateh++;
		if (key[VK_LEFT]) rotateh--;
		if (key[VK_UP]) rotatev++;
		if (key[VK_DOWN]) rotatev--;
		
		float rh = rotateh * (float)(3.141592736/180);
		float rv = rotatev * (float)(3.141592736/180);
		float rr = rotater * (float)(3.141592736/180);
		wp = false;
		sp = false;

		if (key['W'])
		{
			wp = true;
			
		}
		if (key['S'])
		{
			sp = true;
			
		}

		if (key['O'] && !opress) 
		{
			step++;
			opress = true;
			gotoxy(0,13);printf("Step: %d", step);
		}
		else if(key['P'] && !ppress)
		{
			step--;
			ppress = true;
			gotoxy(0,13);printf("Step: %d", step);
		}

		
		if (key[VK_CONTROL] && !campress) 
		{
			cammode++;
			campress = true;
		}
		if (cammode > 0) cammode = -1;
		if(!key[VK_CONTROL]) campress = false;
		if(!key['O']) opress = false;
		if(!key['P']) ppress = false;

//Tests whether or not bones will be drawn
		if(key['V'] && dbones==true)
			dbones=false;
		else if(key['V'] && dbones==false)
			dbones=true;
		if(step <= 0)
			step = 1;

}