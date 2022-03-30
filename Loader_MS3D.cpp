////////////////////////////////////////////////////////////////////////
//Loads the textures and models and maybe some other stuff.
//Also puts the model on the screen because its easier to have it here.
////////////////////////////////////////////////////////////////////////

#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<gl\gl.h>
#include<gl\glu.h>

#include <math.h>
#include <string.h>
#include <soil.h>

#include"ms3d.h"
#include"camera.h"
#include"maths.h"

#include<time.h>

#pragma warning( disable : 4996 )

extern int texnum;
extern GLuint	texture[];

extern float rotateh;
extern float rotatev;
extern bool wp;
extern bool sp;

extern camera cam;


extern bool dbones;



extern void gotoxy(int, int);


bool LoadTexture(char filename[], GLuint &texid)
{

	/* load an image file directly as a new OpenGL texture */
	texid= SOIL_load_OGL_texture(filename, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	/* check for an error during the load process */
	if( 0 == texid )
	{
		printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
		return FALSE;
	}
	else
		return TRUE;

}




	// Loads info for the model!!
bool MS3D::
	Load(char* filename)
	{
	FILE* file;
	header header;
	int loop;

	//Open the .ms3d model file
	if((file= fopen(filename, "rb"))==NULL)
		{

		extern BOOL stop;
		stop = TRUE;
		MessageBox(NULL,"File Could Not Be Loaded.","ERROR",MB_OK|MB_ICONSTOP);	

		return true;
		
		}

	//Read the header data in
	fread(&header.id, sizeof(char), 10, file);
	fread(&header.version, 1, sizeof(int), file);





	if(strncmp(header.id, "MS3D000000", 10)!=0)
		{
		MessageBox( NULL, "Not Valid File", "Error", MB_OK | MB_ICONERROR );
		return 0;//false;	
		}
	if(header.version!=3 && header.version!=4)
		{
		MessageBox( NULL, "Not Valid File", "Error", MB_OK | MB_ICONERROR );
		return 0;//false;
		}


	//Read the vertex data in
	fread(&numVertices, sizeof(unsigned short), 1, file);
	vertices= new vertex [numVertices];
	float vertzmax=0;
	float vertzmin=0;
	for(loop=0; loop<numVertices; loop++)
		{
		fread(&vertices[loop].flags,	sizeof(BYTE),  1, file);
		fread( vertices[loop].vertex,	sizeof(float), 3, file);
		fread(&vertices[loop].boneID,	sizeof(char),  1, file);
		fread(&vertices[loop].refCount, sizeof(BYTE),  1, file);
		vertices[loop].bone=(int)vertices[loop].boneID;
		if(vertices[loop].vertex[2] >vertzmax)
			vertzmax = vertices[loop].vertex[2];
		else if(vertices[loop].vertex[2] < vertzmin)
			vertzmin = vertices[loop].vertex[2];


		}
		gotoxy(0,3);
		printf("%f: %f", vertzmax, vertzmin);
	//Read the triangle data in
	fread(&numTriangles, sizeof(unsigned short), 1, file);
	triangles= new triangle [numTriangles];
	for(loop=0; loop<numTriangles; loop++)
		{
		float mt[3]={1.0f-triangles->t[0],1.0f-triangles->t[1],	1.0f-triangles->t[2]};
		fread(&triangles[loop].flags,			 sizeof(unsigned short), 1, file);
		fread( triangles[loop].vertexIndices,	 sizeof(unsigned short), 3, file);
		fread( triangles[loop].vertexNormals[0], sizeof(float),			 3, file);
		fread( triangles[loop].vertexNormals[1], sizeof(float),		 	 3, file);
		fread( triangles[loop].vertexNormals[2], sizeof(float),			 3, file);
		fread( triangles[loop].s,				 sizeof(float),			 3, file);
		fread( triangles[loop].t,				 sizeof(float),			 3, file);
		fread(&triangles[loop].smoothingGroup,	 sizeof(unsigned char),  1, file);
		fread(&triangles[loop].groupIndex,		 sizeof(unsigned char),  1, file);

	
		}

	//Read the group data in
	fread(&numGroups, sizeof(unsigned short), 1, file);
	groups= new group [numGroups];
	for(loop=0; loop<numGroups; loop++)
		{
		fread(&groups[loop].flags,			 sizeof(unsigned char),  1,	 file);
		fread( groups[loop].name,			 sizeof(char),			 32, file);
		fread(&groups[loop].numTriangles,	 sizeof(unsigned short), 1,	 file);

		groups[loop].triangleIndices=new unsigned short [groups[loop].numTriangles];

		fread( groups[loop].triangleIndices, sizeof(unsigned short), groups[loop].numTriangles,file);
		fread(&groups[loop].materialIndex,	 sizeof(char), 1, file);
	

		}


	
	
	fread(&numMaterials,sizeof(unsigned short), 1, file);
	materials= new material[numMaterials];

	for(loop=0; loop<numMaterials;loop++)
		{

		fread( materials[loop].name,    sizeof(char) ,32,file);
		fread( materials[loop].ambient, sizeof(float),4,file);
		fread( materials[loop].diffuse, sizeof(float),4,file);
		fread( materials[loop].specular,sizeof(float),4,file);
		fread( materials[loop].emissive,sizeof(float),4,file);
		fread(&materials[loop].shininess,sizeof(float),1,file);
		fread(&materials[loop].transparency,sizeof(float),1,file);
		fread(&materials[loop].mode,	sizeof(char),1,file);
		fread( materials[loop].texture,	sizeof(char),128,file);
		fread( materials[loop].alphamap,sizeof(char),128,file);
		gotoxy(1,1+loop);
		printf("%s \n", materials[loop].texture);
		}


	fread(&Animation_FPS, sizeof(float), 1, file);
	fread(&Current_Time,  sizeof(float), 1, file);
	fread(&total_frames,    sizeof(int),   1, file);



	fread(&numJoints, sizeof(unsigned short), 1, file);
	joints= new joint[numJoints];
	
	for(loop=0;loop<numJoints;loop++)
		{

		fread(&joints[loop].flags, sizeof(unsigned char),1 ,file);
		fread( joints[loop].name, sizeof(char)          ,32,file);
		fread( joints[loop].parentName, sizeof(char)    ,32,file);
		fread( joints[loop].rotation,sizeof(float)      ,3 ,file);
		fread( joints[loop].position,sizeof(float)      ,3 ,file);
		fread(&joints[loop].numKeyFramesRot,sizeof(unsigned short),1,file);
		fread(&joints[loop].numKeyFramesTrans,sizeof(unsigned short),1,file);

		joints[loop].keyFramesRot=new keyframe_rot [joints[loop].numKeyFramesRot];
		joints[loop].keyFramesTrans=new keyframe_pos [joints[loop].numKeyFramesTrans];

		fread( joints[loop].keyFramesRot, sizeof(keyframe_rot), joints[loop].numKeyFramesRot,file);
		fread( joints[loop].keyFramesTrans, sizeof(keyframe_pos), joints[loop].numKeyFramesTrans,file);
		//automatically sets joint to start with no parent. If one exists, it is assigned below.
		joints[loop].parent=-1;
	
		}
	fclose(file);	
	
///////////////////////////////////////////////////////////////////////////////////
//new section
//basically copied from other source with some variable and structure name changes 
///////////////////////////////////////////////////////////////////////////////////
	//Find the parent joint array indices
	for(int x1=0;x1<numJoints;x1++)
	{
		//If the bone has a parent
		if(joints[x1].parentName != '\0')
		{
			//Compare names of the parent bone of x with the names of all bones
			for(int x2=0;x2<numJoints;x2++)
			{
				//A match has been found
				if(strcmp(joints[x2].name, joints[x1].parentName) == 0)
				{
					joints[x1].parent = x2;
				}
			}
		}
	}
//end of loop to find parents




	Setup();
reloadt(filename);





	


	return true;
}

////////////////////////////////////////////////////////////////////////////////
//sets up the begining matrices	
////////////////////////////////////////////////////////////////////////////////	

void MS3D::
	Setup(void)
	{
	int vindex;


	for(int l=0; l<numJoints; l++)
	{
		
		for(int v=0;v<16;v++)
		{
			joints[l].matrl[v]=0;
			joints[l].matra[v]=0;
			joints[l].matrf[v]=0;
		}
		Set_Rotation(joints[l].rotation[0], joints[l].rotation[1], joints[l].rotation[2], joints[l].matrl);
		Set_Position(joints[l].position[0], joints[l].position[1], joints[l].position[2], joints[l].matrl);
		
		if(joints[l].parent != -1)
		{
			Matrix_Mult(joints[joints[l].parent].matra, joints[l].matrl, joints[l].matra);
			
		}
		else
		{
			Matrix_Copy(joints[l].matrl, joints[l].matra);
			
		}
			Matrix_Copy(joints[l].matra, joints[l].matrf);
	}

	for(int c = 0; c < numVertices; c++)
	{
		//If there is no bone..
		if(vertices[c].boneID== -1)
			continue;

			invtrans(vertices[c].vertex, joints[vertices[c].boneID].matrf);
			invrotate(vertices[c].vertex, joints[vertices[c].boneID].matrf);

	}



	for(int d=0;d<numTriangles;d++)
	{
	//int triangleIndex		=  groups[loopa].triangleIndices[loopb];


	
		for(int e=0;e<3;e++)
		{
	
			vindex = triangles[d].vertexIndices[e];


	
			if(vertices[vindex].boneID != -1)
			{
			invrotate(triangles[d].vertexNormals[e], joints[vertices[vindex].boneID].matrf);
			}
	}
}





}



//Loads the textures that are built into the ms3d file
void MS3D::
	reloadt(char* filenamet)
{


//Finds the root directory of the model
	int last = 0;
	int len = strlen(filenamet)-1;
	char root[128];

	for(int s=0; s<len; s++)
	{
		if(filenamet[s] == '/')
		{
			last=s;
		}
	}

	strncpy(root,filenamet,last);
	root[last] = '\0';


//Moves up to common directory and creates path for texture files
	int back;
	int bslashcount;
	int matlen;
	char location[128];
	last = 0;
	for( int k = 0; k < numMaterials; k++)
	{
		back=1;
		bslashcount = 0;
		matlen = 0;
		while (back == 1)
		{
			if(strncmp(materials[k].texture, ".\\", 2 ) == 0)				//For files that are within the same root
			{
				matlen = strlen(materials[k].texture);
				for(int g=2; g<matlen; g++)
				{
					materials[k].texture[g-2] = materials[k].texture[g];
				}
				materials[k].texture[matlen-2] = '\0';

				back = 0;

			}
			else if(strncmp(materials[k].texture, "..",2) == 0)				//For files that are not in the same root
			{
				bslashcount++;
				matlen = strlen(materials[k].texture);
				for(int g=3; g<matlen; g++)
				{
					materials[k].texture[g-3] = materials[k].texture[g];
				}
				materials[k].texture[matlen-3] = '\0';


			}

			
			else
			{
				back = 0;
			}
		}
		strncpy(location,root,128);
		if(bslashcount != 0)								//This checks the root directory and removes the incorrect path pieces.
		{
			while(bslashcount > 0)
			{
				for( int a=0; a<128; a++)
				{
					if(location[a] == '/')
					{
						last=a;
					}
				}
			strncpy(root,filenamet,last);
			location[last] = '\0';
			bslashcount--;
			}
		}

		strcat(location, "/");								//Adds the slash in between the root and file path
		strcat(location, materials[k].texture);				//Adds the file path to the root
		strncpy(materials[k].texture, location, 128);		//Copies the path from the dummy var. to the actual variable
		location[0] = '\0';									//Clears dummy var
	}
	
														
	for ( int l = 0; l < numMaterials; l++ )
	{
			if(!LoadTexture( materials[l].texture, materials[l].texturedata))								//Texturing Loading Function
			{
				MessageBox( NULL, "Not Valid Texture", "Error", MB_OK | MB_ICONERROR );
			}
	}
		

}



