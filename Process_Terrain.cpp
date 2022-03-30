//This file creates general functions for the interation of Terrain and other objects within the engine.

#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<gl\gl.h>
#include<gl\glu.h>
#include<time.h>

#include"ms3d.h"
#include"terrain.h"
#include"camera.h"
#include"maths.h"

#pragma warning( disable : 4996 )
#define ANG2RAD 3.14159265358979323846/360.0 

void gotoxy(int x, int y);

extern GLUquadricObj *quadratic;

extern MS3D ms_model[100];			//Array of ms3d model objects
extern TERRAIN t_model;			//Terrain Data Object
extern camera cam;					//Camera Objects

bool Process_Terrain(int selected)
{

	t_model.Clip_Grid();

	return true;
}


bool TERRAIN::
	Clip_Grid()
{
//glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
//glLoadIdentity();
	int xcheck = xp;
	int zcheck = zp;
	int dcheck = dist;

	char ldist;


	/////////////ms_model[0].Animate(sp, beg, end, loop);
	CheckGrid(cam.position);
	dist = SetDist(cam.position, planet_rad);


	if(xp != xcheck || zp != zcheck || dist != dcheck)						//Tells loader that tiles must be changed
	{

		SetTerrain(dist, limit);

	}
		Load_Select(false,1);							//Loads map tiles to reflect change in camera position


		float offset = (float)(hysize*hnumy/2);
		float t_pos[3];
		float m_pos[3];
	//	glPolygonMode( GL_FRONT,GL_LINE);

		for(int v = 0; v < hnumx; v++)						//v and w cycle through entire map 
		{
			for(int w = 0; w < hnumy; w++)
			{
				if(rdata[v + (w * hnumx)].lstat == '2')
				{
				t_pos[1] = rdata[v + (w * hnumx)].center[1];
				t_pos[0] = rdata[v + (w * hnumx)].center[0];//((t_model.hxsize * v) + (t_model.hxsize / 2)  - offset) * t_model.HorScale;
				t_pos[2] = rdata[v + (w * hnumx)].center[2];//((t_model.hysize * w) + (t_model.hysize / 2)  - offset) * t_model.HorScale;
				
				ldist = cam.CheckFrustum_Terrain(t_pos, t_rad);
				gotoxy(1+(v*2), -w + 39); printf("%c", cam.CheckFrustum_Terrain(t_pos, t_rad));
				
				if(ldist != '0')
				{		
					rdata[v + (w * hnumx)].rstat = ldist;
					glColor3f(0.0,0.0,0.0);
	

	/*	glTranslatef(t_pos[0], t_pos[1], -t_pos[2]);
		gluSphere(quadratic,t_model.t_rad,12,12);
		glTranslatef(-t_pos[0], -t_pos[1], t_pos[2]);*/

						for(int m = 0; m < t_model.jump; m++)						//v and w cycle through entire map 
						{
								for(int n = 0; n < t_model.jump; n++)
								{
									
									m_pos[1] = rdata[v + (w * hnumx)].part[m + (n * jump)].center[1];
									m_pos[0] = rdata[v + (w * hnumx)].part[m + (n * jump)].center[0];
									m_pos[2] = rdata[v + (w * hnumx)].part[m + (n * jump)].center[2];
								
								rdata[v + (w * hnumx)].part[m + (n * jump)].step = cam.CheckFrustum_Terrain(m_pos, s_rad);
								
									////if(ldist != '0')
									/////{
									/////	rdata[v + (w * hnumx)].part[m + (n * jump)].step = ldist;
									
										//gotoxy(0,45);printf("XYZ %f, %f, %f", rdata[v + (w * hnumx)].part[m + (n * jump)].center[0],rdata[v + (w * hnumx)].part[m + (n * jump)].center[1],rdata[v + (w * hnumx)].part[m + (n * jump)].center[2]);
										//gotoxy(0,46);printf("XYZ %f, %f, %f",m_pos[0], m_pos[1], m_pos[2]);
										
		/*												glBegin(GL_LINES);
		glVertex3f(m_pos[0]+500,m_pos[1],-m_pos[2]);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(m_pos[0]-500,m_pos[1],-m_pos[2]);
		glEnd();

		glColor3f(0.0,0.0,0.0);
		glBegin(GL_LINES);
		glVertex3f(m_pos[0],m_pos[1]+500,-m_pos[2]);
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(m_pos[0],m_pos[1]-500,-m_pos[2]);
		glEnd();

		glColor3f(0.0,0.0,0.0);
		glBegin(GL_LINES);
		glVertex3f(m_pos[0],m_pos[1],-m_pos[2]+500);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(m_pos[0],m_pos[1],-m_pos[2]-500);
		glEnd();*/
////
	///				glTranslatef(m_pos[0], m_pos[1], -m_pos[2]);
	///////			gluSphere(quadratic,s_rad,20,20);
		///			glTranslatef(-m_pos[0], -m_pos[1], m_pos[2]);
									/////}
									/////else
									/////{
									/////	rdata[v + (w * hnumx)].part[m + (n * jump)].step = 0;
									/////}
								}

						}
						
						
				}
				else 
					rdata[v + (w * hnumx)].rstat = '0';

					//gotoxy(1+(v*2), -w + 39); printf("%c", rdata[v + (w * hnumx)].rstat);
/*/////////////
				glBegin(GL_LINES);
		glVertex3f(t_pos[0]+t_rad,t_pos[1],-t_pos[2]);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(t_pos[0]-t_rad,t_pos[1],-t_pos[2]);
		glEnd();

		glColor3f(0.0,0.0,0.0);
		glBegin(GL_LINES);
		glVertex3f(t_pos[0],t_pos[1]+t_rad,-t_pos[2]);
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(t_pos[0],t_pos[1]-t_rad,-t_pos[2]);
		glEnd();

		glColor3f(0.0,0.0,0.0);
		glBegin(GL_LINES);
		glVertex3f(t_pos[0],t_pos[1],-t_pos[2]+t_rad);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(t_pos[0],t_pos[1],-t_pos[2]-t_rad);
		glEnd();
*/////////////////////

	/*
		glTranslatef(t_pos[0], t_pos[1], t_pos[2]);

		
		gluSphere(quadratic,t_model.t_rad,12,12);
		glTranslatef(-t_pos[0], -t_pos[1], -t_pos[2]);
	*/
				}	//end if
			}	//end w
		}	//end v
	//	glPolygonMode( GL_FRONT,GL_FILL);


return true;
}