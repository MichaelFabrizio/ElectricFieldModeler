#pragma once

#include "Shape.h"

class RPrism : public Shape
{
public:
	RPrism();
	int Integrate() override;
	bool CheckPointForOccupation(int i, int j, int k, float scale) override;

	float length;
	float width;
	float height;
	float theta;
	float phi;
};