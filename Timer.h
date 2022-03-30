//Timer functions are located within this file
//Used for MS3D animations mainly


#include<windows.h>
#ifndef timer_h
#define timer_h


class cTimer
{
	public:
		
		void init();

		unsigned int getmili();
		float getsec();


	private:

		//multipling by this gives you time in seconds	
		float InvTicksPerSec;
		//multipling by this gives you time in milliseconds
		float InvTicksPerMs;
		//start time
		LARGE_INTEGER StartTime;
		__int64 LastTime;
	
};




#endif //timer_h