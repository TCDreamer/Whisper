///////////////////////////////////////////////////////////
//Does the drawing and sets up the screen stuff.
//Model drawing is sent to load
///////////////////////////////////////////////////////////


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


GLUquadricObj *quadratic;

MS3D ms_model[100];			//Array of ms3d model objects
TERRAIN t_model;			//Terrain Data Object
camera cam;					//Camera Objects



int LoadGLTextures();		//Prototype for texture loading function

extern BOOL stop;



GLfloat LightAmbient[];
GLfloat LightDiffuse[];
GLfloat LightPosition[];



GLfloat fogColor[4]= {0.5f, 0.5f, 0.5f, 1.0f};				// Fog Color


extern float zoom;
extern float height;

extern float rotateh;
extern float rotatev;
extern float rotater;
extern GLuint texture[];
extern float beg=0.0;
extern float end=0.0;
extern int cammode;

float sp=1;

bool loop=true;


void gotoxy(int, int);




GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100000.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}



int Drawscene()
{
    const float sfactor= 0.1f;
	static FLOAT fps      = 0.0f;
    static FLOAT last_time= 0.0f;
    static DWORD frames	  = 0L;
	float time;
	static float frametest= 0.0f;


glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
glLoadIdentity();

//	glDisable(GL_DEPTH_TEST);
//	glColor3f(1.0,1.0,1.0);
//	glBegin(GL_LINES);
//	glVertex3f(0,.5,-10);
//	glVertex3f(100,.5,-10);
//	glEnd();
//	glEnable(GL_DEPTH_TEST);
//glLoadIdentity();

//	gotoxy(0,47);printf("XYZ %f, %f, %f", t_model.rdata[53].part[9 + (9 * 10)].center[0],t_model.rdata[53].part[9 + (9 * 10)].center[1],t_model.rdata[53].part[9 + (9 * 10)].center[2]);



if(cammode == -1)
{
	glTranslatef(0.0f, height, zoom);
	glRotatef(rotatev,1.0f,0.0f,0.0f);
	glRotatef(rotateh,0.0f,1.0f,0.0f);

}

if(cammode == 0)
{
	cam.SetPrespective();

}

glDisable(GL_TEXTURE_2D);


//render model here

//glScalef(sfactor, sfactor, sfactor);


//ms_model[0].Animate(sp, beg, end, loop);
/*////////////////////////////////////////////////////////////////////////////////////////////////////////////

int xcheck = t_model.xp;
int zcheck = t_model.zp;
int dcheck = t_model.dist;


/////////////ms_model[0].Animate(sp, beg, end, loop);
t_model.CheckGrid(cam.position);
t_model.dist = t_model.SetDist(cam.position, t_model.limit, t_model.planet_rad);


if(t_model.xp != xcheck || t_model.zp != zcheck || t_model.dist != dcheck)						//Tells loader that tiles must be changed
{

	t_model.SetTerrain(t_model.dist);

}
	t_model.Load_Select(false,1);							//Loads map tiles to reflect change in camera position


float offset = (float)(t_model.hysize*t_model.hnumy/2);// + (t_model.hysize / 2);
	float t_pos[3];
	float m_pos[3];
	glPolygonMode( GL_FRONT,GL_LINE);

	for(int v = 0; v < t_model.hnumx; v++)						//v and w cycle through entire map 
	{
		for(int w = 0; w < t_model.hnumy; w++)
		{
			if(t_model.rdata[v + (w * t_model.hnumx)].lstat == '2')
			{
			t_pos[1] = t_model.rdata[v + (w * t_model.hnumx)].center[1];
			t_pos[0] = t_model.rdata[v + (w * t_model.hnumx)].center[0];//((t_model.hxsize * v) + (t_model.hxsize / 2)  - offset) * t_model.HorScale;
			t_pos[2] = t_model.rdata[v + (w * t_model.hnumx)].center[2];//((t_model.hysize * w) + (t_model.hysize / 2)  - offset) * t_model.HorScale;

			
			if(cam.CheckFrustum_Point(t_pos, t_model.t_rad) == true)
			{		
				t_model.rdata[v + (w * t_model.hnumx)].rstat = '1';

					glColor3f(0.0,0.0,0.0);
//	glTranslatef(t_pos[0], t_pos[1], -t_pos[2]);

	
//	gluSphere(quadratic,t_model.t_rad,12,12);
//	glTranslatef(-t_pos[0], -t_pos[1], t_pos[2]);

					for(int m = 0; m < t_model.jump; m++)						//v and w cycle through entire map 
					{
							for(int n = 0; n < t_model.jump; n++)
							{
								
								m_pos[1] = t_model.rdata[v + (w * t_model.hnumx)].part[m + (n * t_model.jump)].center[1];
								m_pos[0] = t_model.rdata[v + (w * t_model.hnumx)].part[m + (n * t_model.jump)].center[0];
								m_pos[2] = t_model.rdata[v + (w * t_model.hnumx)].part[m + (n * t_model.jump)].center[2];
								if(cam.CheckFrustum_Point(m_pos, t_model.s_rad) == true)
								{
									t_model.rdata[v + (w * t_model.hnumx)].part[m + (n * t_model.jump)].step = 1;
									glTranslatef(m_pos[0], m_pos[1], -m_pos[2]);
									gluSphere(quadratic,t_model.s_rad,20,20);
									glTranslatef(-m_pos[0], -m_pos[1], m_pos[2]);
								}
								else
								{
									t_model.rdata[v + (w * t_model.hnumx)].part[m + (n * t_model.jump)].step = 0;
								}
							}

					}
					
					
			}
			else 
				t_model.rdata[v + (w * t_model.hnumx)].rstat = '0';

				gotoxy(1+(v*2), -w + 39); printf("%c", t_model.rdata[v+(w*t_model.hnumx)].rstat);

			glBegin(GL_LINES);
	glVertex3f(t_pos[0]+t_model.t_rad,t_pos[1],t_pos[2]);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(t_pos[0]-t_model.t_rad,t_pos[1],t_pos[2]);
	glEnd();

	glColor3f(0.0,0.0,0.0);
	glBegin(GL_LINES);
	glVertex3f(t_pos[0],t_pos[1]+t_model.t_rad,t_pos[2]);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(t_pos[0],t_pos[1]-t_model.t_rad,t_pos[2]);
	glEnd();

	glColor3f(0.0,0.0,0.0);
	glBegin(GL_LINES);
	glVertex3f(t_pos[0],t_pos[1],t_pos[2]+t_model.t_rad);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(t_pos[0],t_pos[1],t_pos[2]-t_model.t_rad);
	glEnd();



//	glTranslatef(t_pos[0], t_pos[1], t_pos[2]);

	
//	gluSphere(quadratic,t_model.t_rad,12,12);
//	glTranslatef(-t_pos[0], -t_pos[1], -t_pos[2]);

			}	//end if
		}	//end w
	}	//end v
	glPolygonMode( GL_FRONT,GL_FILL);
////////////////////////////////////////////////////////////*/

t_model.Renderter();
//glLoadIdentity();

//gluSphere(quadratic,1.3f,32,32);
//	glDisable(GL_DEPTH_TEST);
	glColor3f(0.0,0.0,0.0);
	glBegin(GL_LINES);
	glVertex3f(100,0,0);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(-100,0,0);
	glEnd();

	glColor3f(0.0,0.0,0.0);
	glBegin(GL_LINES);
	glVertex3f(0,100,0);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0,-100,0);
	glEnd();

	glColor3f(0.0,0.0,0.0);
	glBegin(GL_LINES);
	glVertex3f(0,0,100);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0,0,-100);
	glEnd();
//	glEnable(GL_DEPTH_TEST);
//glLoadIdentity();




///////////////////////////////////////////////////
//Calculates fps
///////////////////////////////////////////////////
time= clock()*0.001f; // Gets the time in seconds
    ++frames;

    // Update the frame rate once per second
    if(time-last_time>1.0f)
		{
        fps      = frames/(time-last_time);
        last_time= time;
        frames   = 0L;
		
		}
if( frametest != fps)
{
////////prints the frames at every change in the file titled frames
	gotoxy(0,0);
	printf("FPS:%f",fps);
}

	frametest = fps;
///////////////////////////////////////////////////
glFlush();
return TRUE;
}



int init(void)
{
extern int wwide;
extern int wheight;

	glEnable(GL_TEXTURE_2D);										// Enable Texture Mapping ( NEW )
	glShadeModel(GL_SMOOTH);										// Enable Smooth Shading
//	glClearColor(0.2f, 0.2f, 0.8f, 0.5f);							// Black Background
	glClearDepth(1.0f);												// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);										// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);											// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);		



	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);	// Position The Light
//	glEnable(GL_LIGHT1);								// Enable Light One
											

	glClearColor(0.5f,0.5f,0.8f,1.0f);				// We'll Clear To The Color Of The Fog

	glFogi(GL_FOG_MODE, GL_EXP);					// Fog Mode
//	glFogfv(GL_FOG_COLOR, fogColor);				// Set Fog Color
	glFogf(GL_FOG_DENSITY, 0.5f);					// How Dense Will The Fog Be
	glHint(GL_FOG_HINT, GL_NICEST);					// Fog Hint Value
	glFogf(GL_FOG_START, 10100.0f);					// Fog Start Depth
	glFogf(GL_FOG_END, 20480.0f);					// Fog End Depth
//	glEnable(GL_FOG);								// Enables GL_FOG
	glEnable(GL_COLOR_MATERIAL);
	glPolygonMode( GL_FRONT,GL_FILL);
	glPolygonMode( GL_BACK, GL_LINE );

	cam.ratio = ((float)wwide) / ((float)wheight);
	cam.fnear = 0.01f;
	cam.ffar = 100000.0f;
	cam.rfar = 50000.0f;
	cam.rad = (float)(45.0f * ANG2RAD);
//enter model to load here
//if(ms_model[0].Load("data/test.ms3d")==false) stop=TRUE;

	quadratic=gluNewQuadric();			// Create A Pointer To The Quadric Object ( NEW )
	gluQuadricNormals(quadratic, GLU_SMOOTH);	// Create Smooth Normals ( NEW )


return TRUE;
}

void loadmodels(char* location)
{
	/*FILE *mfile;
	if((mfile = fopen(location,"r"))==false) 
	{
		stop=TRUE;
		MessageBox(NULL,"File.txt Could Not Be Loaded.","ERROR",MB_OK|MB_ICONSTOP);
	}
	else
	{
		char letter;
		const int limit=64;
		char filename[limit];
		int counter = 0;
		int m_num=0;
		bool m_end = false;

		while(m_num<100)
		{
			counter=0;
			while(counter<limit)
			{
				fread(&letter,sizeof(char),1,mfile);
				if(letter != ';' && letter != ':') 
				{
					filename[counter]=letter;
					counter++;
				}
				else if(letter==':')
				{
					filename[counter]='\0';
					counter=limit;
					m_end = true;
				}
				else
				{
					fread(&letter,sizeof(char),1,mfile);
					filename[counter]='\0';
					counter=limit;
				}


			}

*/
			if(ms_model[0].Load("data/models/vtech.ms3d")==false) stop=TRUE;
	//		m_num++;
//
//		if(m_end==true || stop==TRUE)
//				m_num=100;
//	
//		}
//		
//	}*/
//	ms_model[0].reloadt();
//	fclose(mfile);
}

void loadterrain(char* location)
{
	t_model.LoadData(location, cam.position, cam.direction, 2);
}

void loadcam(void)
{
	cam.SetPrespective();
}




//loads texture data for program from file(Largely Disabled as actual Loader is not finished)
void loadtexture()
{
	FILE *tfile;
	if((tfile = fopen("data/texture_files.txt","r"))==false) 
	{
		stop=TRUE;
		MessageBox(NULL,"texture_files.txt Could Not Be Loaded.","ERROR",MB_OK|MB_ICONSTOP);
	}
	else
	{
		char letter;
		const int limit=24;
		char filename[limit];
		int counter = 0;
		int t_num=0;
		bool t_end = false;

		while(t_num<100)
		{
			counter=0;
			while(counter<limit)
			{
				fread(&letter,sizeof(char),1,tfile);
				if(letter != ';' && letter != ':') 
				{
					filename[counter]=letter;
					counter++;
				}
				else if(letter==':')
				{
					filename[counter]='\0';
					counter=limit;
					t_end = true;
				}
				else
				{
					fread(&letter,sizeof(char),1,tfile);
					filename[counter]='\0';
					counter=limit;
				}
				printf("%d",counter);

			}

			if(t_num==0)
			{
			gotoxy(3,4);
				printf("%s",filename);
			}

	//		if(ms_model[t_num].reloadt(texture[t_num]))==false) stop=TRUE;
		//	ms_model[t_num].reloadt(filename[t_num]);
			t_num++;

		if(t_end==true || stop==TRUE)
				t_num=100;
	
		}
		
	}
	fclose(tfile);
}








void gotoxy(int x, int y) 
{ 
    COORD coord; 
    coord.X = x; coord.Y = y; 
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); 
}

