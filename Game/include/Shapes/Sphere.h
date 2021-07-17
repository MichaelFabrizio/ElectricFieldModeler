#pragma once
#include "Shape.h"

class Sphere : public Shape
{
public:
	Sphere();
	int Integrate() override;
	bool CheckPointForOccupation(int i, int j, int k, float scale) override;

	float radius;
};