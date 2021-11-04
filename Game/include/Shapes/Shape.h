#pragma once
#include <string>

class Shape
{
public:
	Shape(int shapeTyp) : name{ "" }
	{
		shapeType = shapeTyp;
	}

	int shapeType;
	char name[32];
	double x;
	double y;
	double z;
	double voltage;

	//Obsolete - Nonfunctional
	virtual int Integrate() = 0;


	virtual bool CheckPointForOccupation(int i, int j, int k, float scale) = 0;
};