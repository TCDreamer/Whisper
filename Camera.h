//Class that controls the camera of the engine
//Uses the same functions that are used to animate ms3d models


class camera
{
public:
	float maxpitch;
	float maxheading;
	float pitch;
	float heading;
	float maxfvelocity;
	float fvelocity;
	float qpitch[4];
	float qheading[4];
	float position[3];
	float direction[3];
	float look[3];
	float up[3];
	float right[3];

	float ratio;
	float fnear;
	float ffar;
	float rfar;
	float rad;

	//void ChangeVelocity(float vel);
	//void ChangeHeading(float degrees);
	//void ChangePitch(float degrees);
	void SetPrespective(void);

	bool CheckFrustum_Point(float [], float);											//Checks whether a point is in frustum
	char CheckFrustum_Terrain(float [], float);									//Checks terrain boxes. Float is max values of points. Int is max number of divisions to create



};