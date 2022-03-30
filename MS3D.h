//Header for the Milkshape3d file Loader

#ifndef ms3d_h
#define ms3d_h



#include"timer.h"



//model structures

typedef struct ms3dheader
	{
    char id[10];				//The ID number of the model (always "MS3D000000")
	int  version;				//The version number of the model
	} header;					//MS3D_HEADER;

typedef struct ms3dvertex
	{
	unsigned char flags;
	float vertex[3];
	char boneID;				//The bone ID (used for skeletal animation)
	unsigned char refCount;
	//not in file
	int bone;
	} vertex;


typedef struct ms3dtriangle
	{
	unsigned short flags;
	unsigned short vertexIndices[3];
	float vertexNormals[3][3];
 
	float s[3];
	float t[3];
	unsigned char smoothingGroup;
	unsigned char groupIndex;
	} triangle;


typedef struct ms3dgroup
	{
    unsigned char	flags;
    char			name[32];
    unsigned short	numTriangles;
	unsigned short*	triangleIndices;
	char			materialIndex;


	} group;


typedef struct ms3dmaterial
	{
	char            name[32];                           //
    float           ambient[4];                         //
    float           diffuse[4];                         //
    float           specular[4];                        //
    float           emissive[4];                        //
    float           shininess;                          // 0.0f - 128.0f
    float           transparency;                       // 0.0f - 1.0f
    char            mode;                               // 0, 1, 2 is unused now
    char            texture[128];                        // texture.bmp
    char            alphamap[128];                       // alpha.bmp

	GLuint			texturedata;

	}material;




typedef struct ms3dKeyframe_rot
	{
	float time;					//time in seconds
	float rotation[3];			//x,y,z angles
	}keyframe_rot;

typedef struct ms3dKeyframe_pos
	{
	float time;					//time in seconds
	float position[3];			//local position
	}keyframe_pos;



typedef struct ms3djoint
	{
	unsigned char	flags;
	char			name[32];
	char			parentName[32];
	float			rotation[3];			//local reference matrix
	float			position[3];

	unsigned short	numKeyFramesRot;
	unsigned short	numKeyFramesTrans;
	
	keyframe_rot* keyFramesRot;				//local animation matrices
	keyframe_pos* keyFramesTrans;			//local animation matrices
	

///////////////////////////////////
//data below not loaded from file//
///////////////////////////////////

//joints parent
	 short parent;


//all matrices are setup as identity matrices	
//local matrix
	float matrl[16];

//absolute matrix
	float matra[16];

//final matrix
	float matrf[16];

//current frames
	unsigned short crotframe;
	unsigned short ctraframe;

	}joint;

	
//model class

class MS3D
	{
	public:
		unsigned short numVertices;
		vertex*   vertices;
		unsigned short numTriangles;
		triangle* triangles;
		unsigned short numGroups;
		group*	  groups;
		unsigned short numMaterials;
		material* materials;

		float Animation_FPS;
		float Current_Time;
		int total_frames;

		unsigned short numJoints;
		joint*	  joints;




		cTimer time;


		float vec[3];

	bool Load(char* filename);

	void Setup(void);

	void Render(void);

	void reloadt(char* filenamet);

	int Animate(float speed, float starttime, float endtime, bool loop);

	void Reset(void);

	MS3D()
		{	}

	~MS3D()
		{	
		if(vertices)
			delete vertices;		
		if(triangles)
			delete triangles;
		if(groups)
			delete groups;
		if(materials)
			delete materials;
		if(joints)
			delete joints;

		}
	};




#endif //ms3d_h