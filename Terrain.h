//Data structurs for dynamically loading terrain 
#include<windows.h>
#include<gl\gl.h>
#include<time.h>

typedef struct piece
{
	int step;
	float center[3];
}piece;

typedef struct rawdata						//Data structure for height data
{
	unsigned short *hdata;					//stores the data loaded from raw file. Its unsigned short for 16 bit precision
	char gridstat;							//Tells the render ready status of each map within level
	char lstat;								//Tells the load status of the map within memory
	char rstat;								//Tells the render whether or not to render it. Determined by camera position
	float center[3];						//The point that is the center of the map. Use in frustum culling.
	piece* part;							//The number of points to jump over in a certain point within the map

}rawdata;


class TERRAIN
	{
	public:

		rawdata* rdata;							//Data storage for individual height maps

		int   hxsize;								//Pixel size of heightmap files
		int   hysize;								//Pixel size of heightmap files
		float MaxAlt;								//Maximum Altitude
		float MinAlt;								//Minimum Altitude
		float HorScale;								//Horizontal Scale (Increases Pixel Size to real size)(Meters/Pixel)
		char  hlocname[128];						//Location and Name of files
		int   hnumx;								//Number of terrain files in the X direction
		int   hnumy;								//Number of terrain files in the Y direction
		char  hext[16];								//Extension of terrain files


		int  txsize;								//Pixel size of texture
		int  tysize;								//Pixel size of texture
		char tlocname[128];							//Location and name of files
		int  tnumx;									//Number of texture files in X direction
		int  tnumy;									//Number of texture files in Y direction
		char text[16];								//Extension of texture files
		GLuint* texturedata;						//Data storage for individual texture files

		float planet_rad;							//The planetary radius in meters. Used to determine the maximum distance to render.
		float t_rad;								//Radius of a circle that covers one entire map
		float s_rad;								//Radius of a circle that covers the sub maps


		float zscale;								//Number to scale the heights by (calculated by (MaxAlt - MinAlt) / 65535)
		int hnum;									//Total number of heightmaps
		int tnum;									//Total number of textures

		int dist;									//Number of rings of terrain to create
		int limit;									//Limit to dist

		int cnum;									//Number of map files that the current distance setting creates

		int xp;
		int zp;


		int loadp;									//Position in map file to add data to when loading

		int current_x;								//Which map in X is begining loaded. None = -1
		int current_z;								//Which map in Z is begining loaded. None = -1

		int jump;									//Splits the maps into tiles

//Functions located within Loader_Terrain.cpp

///////////////////////////////////////////////////////////////////replace ldist

		bool LoadData(char* datafilename, float pos[], float dir[], int ldist);			//Loads the initial data for the individual maps (dist is how far the rendering goes ex. the rings of maps around camera)
					//File of Terrain info, Camera position, Camera direction, Distance of render
		bool Get_Center(int v, int w);																//Obtains the position of the center of the main maps and sub maps.
		int  SetDist(float pos[], float rad);								//Determines the number of tile rings by calculating the max distance viewed. 
																						//Limit is the max amount of rings to load before the renderer switches to a larger map. Rad is radius of planet.
																						//Returns (int dist)

		bool CheckGrid(float pos[]);													//Determines what map file the camera is over
		bool SetTerrain(int dist, int limit);														//Determines the status of a map file. (0 = Do not load 1 = Send to load buffer, 2 = Render)
						//investigate time var
		bool Load_Select(bool init, float time);										//Selects maps to load individually
		bool LoadBuffer();																//Loads data into terrain swap buffer
		bool LoadTerrain(int x, int z);													//Loads data to setup terrain


//Functions located within Process_Terrain.cpp
		bool Clip_Grid();																//Interacts with the camera class to determine whats actually visible
		bool LOD_Grid();																//Determines the 'step' amount for each section of the map

//Functions located within Render_Terrain.cpp
		bool Renderter();																//Renders the combined terrain
		bool Unload(int x, int z);														//Deletes data from unneeded maps
		


	};
