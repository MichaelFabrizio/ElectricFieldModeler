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
	float x;
	float y;
	float z;
	float voltage;
	virtual int Integrate() = 0;
	virtual bool CheckPointForOccupation(int i, int j, int k, float scale) = 0;
};