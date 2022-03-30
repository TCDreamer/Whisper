#include"Terrain.h"
#include<string.h>
#include<stdio.h>
#include<math.h>


#pragma warning( disable : 4996 )


void gotoxy(int, int);

bool TERRAIN :: 
	LoadData(char *datafilename, float pos[], float dir[], int ldist)
{
		
		hxsize = 1024;									//Pixel size of heightmap files
		hysize = 1024;									//Pixel size of heightmap files
		MaxAlt = 3501;									//Maximum Altitude
		MinAlt = -4921;									//Minimum Altitude
		HorScale = 10;									//Horizontal Scale (Increases Pixel Size to real size)(Meters/Pixel)
		strcpy(hlocname, "data/genesis/HM/genesis_hm");	//Location and Name of files
		hnumx = 8;										//Number of terrain files in the X direction
		hnumy = 8;										//Number of terrain files in the Y direction
		strcpy(hext, ".raw");							//Extension of terrain files


		txsize = 10;									//Pixel size of texture
		tysize = 10;									//Pixel size of texture
		strcpy(tlocname, "data/textures/genesis");		//Location and name of files
		tnumx = 4;										//Number of texture files in X direction
		tnumy = 4;										//Number of texture files in Y direction
		strcpy(text, ".tga");							//Extension of texture files

		zscale = (MaxAlt - MinAlt) / 65535;				//Number to scale the heights by (calculated by (MaxAlt - MinAlt) / 65535)
		hnum = hnumx * hnumy;
		tnum = tnumx * tnumy; 

		float t_scale = 1;								//Variable that changes the size of the bounding sphere of and entire map
		float s_scale = 1.5;							//Variable that changes the size of the bounding sphere of the sub maps


		planet_rad = 6371000;
		jump = 16;
		t_rad = (sqrt((float)((hxsize * hxsize / 4) + (hxsize * hxsize / 4)))) * t_scale * HorScale;
		s_rad = (sqrt((float)((hxsize * hxsize / (4 * jump * jump)) + (hxsize * hxsize / (4 * jump * jump))))) * s_scale * HorScale;

		rdata = new rawdata [hnum];						//Data storage for individual height maps	


//	texturedata = new GLuint[tnum];				//Data storage for individual texture files

		/*texturedata = new GLuint [tnumx];			
		for(int v=0; v < tnumx; v++)
		{
			texturedata[v] = new GLuint [tnumy];
		}
		*/
		limit = 5;
		dist = SetDist(pos, planet_rad);


		current_x = -1;
		current_z = -1;


		for(int g = 0; g < hnum; g++)				//Sets each status initially at zero
		{
			rdata[g].gridstat = '0';
			rdata[g].lstat = '0';
			rdata[g].rstat = '0';
		}		
		CheckGrid(pos);
		SetTerrain(dist, limit);

		Load_Select(true, 1);
	



		return true;

}

bool TERRAIN::
	Get_Center(int v, int w)
{
	float offset = (float)(hysize*hnumy/2);
	float offset2 = (float)((hysize / 2) - (hysize/(jump * 2)));
	int ixsize = (int)(hxsize / jump);



			rdata[v + (w * hnumx)].center[1] =  ((MaxAlt + MinAlt)/2);
			rdata[v + (w * hnumx)].center[0] = ((hxsize * v) + (hxsize / 2)  - offset) * HorScale;
			rdata[v + (w * hnumx)].center[2] = ((hysize * w) + (hysize / 2)  - offset) * HorScale;
			rdata[v + (w * hnumx)].part  = new piece [jump * jump];


			for(int m = 0; m < jump; m++)						//v and w cycle through entire map 
			{
				for(int n = 0; n < jump; n++)
				{																						
					rdata[v + (w * hnumx)].part[m + (n * jump)].center[1] =  (rdata[v + (w * hnumx)].hdata[((m*ixsize)+(ixsize/2)) + (((n*ixsize)+(ixsize/2))*hxsize)] * zscale) + MinAlt;
					rdata[v + (w * hnumx)].part[m + (n * jump)].center[0] = rdata[v + (w * hnumx)].center[0] + (((hxsize / jump * m)- offset2) * HorScale);
					rdata[v + (w * hnumx)].part[m + (n * jump)].center[2] = rdata[v + (w * hnumx)].center[2] + (((hysize / jump * n) - offset2) * HorScale);

				}
			}


	return true;
}


int TERRAIN::
	SetDist(float pos[], float rad)
{
	float adj_rad = MinAlt + rad;								//adjusted radius accounts for the lowest position on the map
																//a^2 + b^2 = c^2
																//a^2 + b^2 = (a + h)^2
																//b^2 = (a + h)^2 - a^2
																//b = [(a + h)^2 - a^2]^(1/2)
																//a2 +2ah +h2 - a2
																//b = (2ah +h^2)^(1/2)
	if (pos[1] >= MinAlt)
	{
		float dist = sqrt((2*adj_rad*(pos[1] - MinAlt)) + ((pos[1] - MinAlt)*(pos[1] - MinAlt)));

		int ldist = (int)((dist / (hxsize * HorScale))) + 1;

	//	if(ldist > limit)
	//		ldist = limit;
//	gotoxy(10,18);printf("                      ");
//	gotoxy(10,18);printf("ldist: %d", ldist);
//	gotoxy(10,19);printf("                      ");
//	gotoxy(10,19);printf("dist: %f", dist);
	
		return ldist;
	}
	else
		return NULL;
}




bool TERRAIN::
	CheckGrid(float pos[])
{
	float standard;												//Converted camera position. Removes horizontal scale and offsets it.

	standard = ((pos[0]/HorScale)/hxsize) + (hnumx/2);
	gotoxy(0,5);printf("standx: %f", standard);
	xp = (int)floor(standard);

	standard = ((pos[2]/HorScale)/hysize) + (hnumy/2);
	gotoxy(18,5);printf("standy: %f", standard);
	zp = (int)floor(standard);

	gotoxy(0,6); printf("x num: %d, z num: %d", xp, zp);
	gotoxy(0,7); printf("xsize: %d, ysize: %d", hxsize, hysize);
	gotoxy(0,8); printf("HorScale: %f", HorScale);

	return true;
}




bool TERRAIN::
	SetTerrain(int dist, int limit)
{

	int setx;
	int sety;

	if (xp >= hnumx -1)
		setx = hnumx - 1;
	else if(xp < 0)
		setx = 0;
	else
		setx = xp;

	if(zp >= hnumy - 1)
		sety = hnumy - 1;
	else if(zp < 0)
		sety = 0;
	else 
		sety = zp;



	for(int q = 0; q < hnumx; q++)
	{
		for(int p = 0; p < hnumy; p++)
		{
			if( q == setx && p == sety)
				rdata[q + (p * hnumx)].gridstat = '3';															//Sets map to render with high detail
			else
			{
				
				if(((q <= setx + dist && q > setx + limit) || (q >= setx - dist && q < setx - limit)) || ((p <= sety + dist && p > sety + limit) || (p >= sety - dist && p < sety - limit)))
					rdata[q + (p * hnumx)].gridstat = '4';														//Load in super low buffer 

				else if((q < setx + limit && q > setx - limit) && (p < sety + limit && p > sety - limit))
					rdata[q + (p * hnumx)].gridstat = '2';														//Sets map to render with low detail
				else if((q == setx + limit || q == setx - limit) && (p <= sety + limit && p >= sety - limit))
					rdata[q + (p * hnumx)].gridstat = '1';														//Load in buffer 
				else if((q <= setx + limit && q >= setx - limit) && (p == sety + limit || p == sety - limit))
					rdata[q + (p * hnumx)].gridstat = '1';
				else
					rdata[q + (p * hnumx)].gridstat = '0';														//Sets map to not load
			}

			gotoxy(1+(q*2), -p + 18); printf("%c", rdata[q+(p*hnumx)].gridstat);
		}	//End p for loop

	}	//End q for loop

return true;

}//End SetTerrain


bool TERRAIN:: 
	Load_Select(bool init, float ltime)
{
	int v = 0;
	int w = 0;
	bool go = true;


	static float last = 0;


	if(init == true)																	//If program is being initialized, cycle through and load all terrain at once.
	{
		last = clock()*0.001f;

		for(v = 0; v < hnumx; v++)
		{
			for(w = 0; w < hnumy; w++)
			{
				if((rdata[v + (w*hnumx)].gridstat != '0' && rdata[v + (w*hnumx)].gridstat != '4') && (rdata[v +(w*hnumx)].lstat != '2'))
				{
					//for(int b = 0; b < 8; b++)
					while(rdata[v + (w * hnumx)].lstat != '2')
					{
						LoadTerrain(v, w);
					}
					Get_Center(v, w);

				}

				gotoxy(1+(v*2), -w + 28); printf("%c", rdata[v+(w*hnumx)].lstat);
			}
			
		}


	}
	else																			//If the program is running, incrementally load new terrain data.
	{
			while(v < hnumx && go == true)
			{
				w = 0;
				while(w < hnumy && go == true)
				{
					if ((v ==current_x && w == current_z) || (current_x == -1 && current_z == -1))
					{
						if((rdata[v + (w*hnumx)].gridstat != '0') && (rdata[v +(w*hnumx)].lstat == '1'))		//If the file is currently being loaded
						{

							LoadTerrain(v, w);
							go = false;
						}
						else if((rdata[v + (w*hnumx)].gridstat != '0' && rdata[v + (w*hnumx)].gridstat != '4') && (rdata[v +(w*hnumx)].lstat == '0'))	//If the file hasn't started being loaded and is next to be being loading.
						{

							LoadTerrain(v, w);
							go = false;
							current_x = v;
							current_z = w;
						}
						else
						{
							current_x = -1;
							current_z = -1;
						}

						
					}
					w++;
				}				//end while w
				v++;
			}					//end while v
			if (go == false && (rdata[v - 1 + ((w - 1)*hnumx)].lstat == '2'))
			{
				Get_Center(v - 1, w - 1);
				current_x = -1;
				current_z = -1;
			}
			
	}


	if(go == true)																					//Unloads terrain data no longer needed
	{
		for(v = 0; v < hnumx; v++)
		{
			for(w = 0; w < hnumy; w++)
			{
				if((rdata[v + (w*hnumx)].gridstat == '0' || rdata[v + (w*hnumx)].gridstat == '4') && (rdata[v +(w*hnumx)].lstat != '0'))
				{
					Unload(v, w);
				}
				gotoxy(1+(v*2), -w + 28); printf("%c", rdata[v+(w*hnumx)].lstat);

			}
			
		}

	}


return true;

}


bool TERRAIN ::
	LoadTerrain(int x, int z)
{
		int end;
		bool stop;
		int max = hxsize * hysize;

		static FILE *rawfile;
		char string[128];
		char step = rdata[x + (hnumx * z)].lstat;



		if(step == '0')
		{
			loadp = 0;
			string[0] = '\n';


			sprintf(string, "%s_x%dy%d%s", hlocname, x, (z), hext);
		

			
			if((rawfile = fopen(string,"rb"))==false) 
				{
					stop = true;
					MessageBox(NULL,"Terrain Data Could Not Be Opened.","ERROR",MB_OK|MB_ICONSTOP);

				}

		}
				if(stop != true)
				{
					if(step == '0')
					{
						rdata[x + (z*hnumx)].hdata = new unsigned short [hxsize * hysize];
						//rdata[x + (z*hnumx)].part  = new piece [jump * jump];
					}

					end = loadp + (hxsize * 4);									//The multiplier determines how many columns are loaded at one time
					gotoxy(1,29); printf("loadp: %d   ", loadp);
					gotoxy(1,30); printf("  end: %d     ", end);
					gotoxy(20,30); printf("%d%%   ",(end*100/max));
					if(end > max)
					{
						end = max;
						rdata[x + (hnumx * z)].lstat = '2';
					}
					else if(end == max)
					{
						rdata[x + (hnumx * z)].lstat = '2';
					}
					else
					{
						rdata[x + (hnumx * z)].lstat = '1';
					}

					
					for(int b = loadp; b < end; b++)
					{
						if(fread(&rdata[x+(z*hnumx)].hdata[b], sizeof(unsigned short), 1, rawfile )!= 1)
						{
							printf("raw file read error");
							Sleep(10000);
						}

					}
					

					if(rdata[x + (hnumx * z)].lstat == '2')
						fclose(rawfile);
				
					loadp = end;
				}
					
					gotoxy(1+(x*2), -z + 28); printf("%c", rdata[x+(z*hnumx)].lstat);
				if(rdata[x + (hnumx * z)].lstat == '2')
				{
					//loadp = 0;
					return true;
				}
				else
					return false;
}



bool TERRAIN::
	Unload(int x, int z)
{
		delete rdata[x+(z*hnumx)].hdata;
		rdata[x+(z*hnumx)].lstat = '0';
		return true;
}


