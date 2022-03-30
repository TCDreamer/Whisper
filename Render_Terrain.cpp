//Includes functions to display the terrain that is loaded into memory
#include"Terrain.h"
#include<stdio.h>


extern int step;

bool TERRAIN::
	Renderter()
{
	//int STEP_SIZE = 80;
	int STEP_SIZE;

	int ixsize = (int)(hxsize / jump);
	int iysize = (int)(hysize / jump);


	//STEP_SIZE = (int) (ixsize / step);

	float y;
	float x1, z1;						//calculated values for vertex
	float x2, z2;
	float offset = (float)(hysize*hnumy/2);// + (hysize/2);

//	if(bRender)						// What We Want To Render
		glBegin( GL_QUADS );				// Render Polygons
//	else 
//		glBegin( GL_LINES );				// Render Lines Instead
//		glColor3f(1.0f, 1.0f, 0.0f);

	for(int v = 0; v < hnumx; v++)						//v and w cycle through entire map 
	{
		for(int w = 0; w < hnumy; w++)
		{
			if((rdata[v + (w * hnumx)].lstat == '2') && (rdata[v + (w * hnumx)].gridstat == '2' || rdata[v + (w * hnumx)].gridstat == '3')&& rdata[v + (w * hnumx)].rstat != '0')
			{
			for(int c = 0; c < jump; c++)				//c and a break each individual map into jump^2 parts
			{
				for(int a = 0; a < jump; a++)
				{

					if(rdata[v+(w*hnumx)].part[c + (a *jump)].step != '0')
					{
						if(rdata[v+(w*hnumx)].part[c + (a *jump)].step == '1')
							STEP_SIZE = 1;
						else if(rdata[v+(w*hnumx)].part[c + (a *jump)].step == '2')
							STEP_SIZE = 4;//(int) (ixsize / (8));
						else if(rdata[v+(w*hnumx)].part[c + (a *jump)].step =='3')
							STEP_SIZE = (int) (ixsize / (2));
						else if(rdata[v+(w*hnumx)].part[c + (a *jump)].step =='4')
							STEP_SIZE = (int) (ixsize / (1));

						for (int X = (c * ixsize); X < ((ixsize*(c+1)) - STEP_SIZE); X += STEP_SIZE )			//X and Y 
						{
							for (int Y = (a * iysize); Y < ((iysize*(a+1)) - STEP_SIZE); Y += STEP_SIZE )

							{
								if(X  < hxsize - STEP_SIZE && Y < hysize - STEP_SIZE)				//Prevents the data overflow when 
								{
									x1 = ((X + STEP_SIZE + (hxsize * v)) - offset)	* HorScale;
									x2 = ((X + (hxsize * v)) - offset)	* HorScale;	
									
									z1 = -(((Y + (hysize * w)) -  offset) * HorScale);
									z2 = -(((Y + STEP_SIZE + (hysize * w)) -  offset) * HorScale);


									y = (rdata[v+(w*hnumx)].hdata[(X+STEP_SIZE)+(Y*hysize)] * zscale) + MinAlt;	

									glColor3f(((y-MinAlt)/zscale)/65535, ((y-MinAlt)/zscale)/65535, ((y-MinAlt)/zscale)/65535);
									glVertex3f(x1, y, z1);		


									y =	(rdata[v+(w*hnumx)].hdata[(X+STEP_SIZE)+((Y+STEP_SIZE)*hysize)] * zscale) + MinAlt;	

									glColor3f(((y-MinAlt)/zscale)/65535, ((y-MinAlt)/zscale)/65535, ((y-MinAlt)/zscale)/65535);
									glVertex3f(x1, y, z2);		


									y = (rdata[v+(w*hnumx)].hdata[X+((Y+STEP_SIZE)*hysize)] * zscale) + MinAlt;

									glColor3f(((y-MinAlt)/zscale)/65535, ((y-MinAlt)/zscale)/65535, ((y-MinAlt)/zscale)/65535);
									glVertex3f(x2, y, z2);		


									y = (rdata[v+(w*hnumx)].hdata[X+(Y*hysize)] * zscale) + MinAlt;

									glColor3f(((y-MinAlt)/zscale)/65535, ((y-MinAlt)/zscale)/65535, ((y-MinAlt)/zscale)/65535);
									glVertex3f(x2, y, z1);		

	
								}//end if

					

						
							}//end Y for
						}//end X for
					}//end if
				}//end a for
			}//end c for
			}//end if
		}//end w for
	}//end v for

		glEnd();

	return true;

}