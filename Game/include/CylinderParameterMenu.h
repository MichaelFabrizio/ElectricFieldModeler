#pragma once
#include "Shapes/Cylinder.h"

class CylinderParameterMenu
{
public:
	CylinderParameterMenu();
	~CylinderParameterMenu();
	//Can't even remember what this spawn method does? I don't think it's being used at all
	void Spawn(float xOffset, float yOffset);
	Cylinder GenerateObject();
	void ResetObject();
	
	//It doesn't make sense for these booleans to even be here.
	//Furthermore, a SHAPE which is a DETECTION *SHAPE* should end up in the detection list. That is why this bool is redundant
	bool ObjectPendingCreation;
	bool detectionCyl_;
private:
	//The name of the object when it gets put into the Big List of Shapes (see: Core.cpp)
	char namebuf_[32];

	double* width_;
	double* height_;
	double* x_;
	double* y_;
	double* z_;
	double* radius_;
	double* voltage_;
	double* theta_;
	double* phi_;
};