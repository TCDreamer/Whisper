//This file contains functions that combine together and effectively handle all interation between objects.
//It basically runs the separate processes that are contained in within and objects 'Process_*.cpp' file.

#include<stdio.h>
#include<stdlib.h>

//Function Prototypes
bool Process_Terrain(int);

bool Run_Processes()
{
	bool check = true;

	if(Process_Terrain(1) != true) 
		check = false;


	return check;
}
