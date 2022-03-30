//Holds data to be used within other modules of the level

class Level
{
	char level_name;									//Display name of the level

	int num_terrain;									//Number of terrain files that make up world (to be implemented [current default = 1])
	char *loc_terrain;									//File location and name of each master terrain file	

	int num_ms3d;										//Number of separate ms3d models used (Not number of sprites using models)
	char *loc_ms3d;										//File location and name of each ms3d file

	char next_level[6];									//Names of the neighboring levels. Used to transition from current area to the next
														//0=top 1=bottom 2=left 3=right 4=up 5=down



};