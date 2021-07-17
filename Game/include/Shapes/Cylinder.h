#pragma once
#include "Shape.h"

class Cylinder : public Shape
{
public:
	Cylinder();
	int Integrate() override;
	bool CheckPointForOccupation(int i, int j, int k, float scale) override;

	float radius_;
	float height_;
	float theta_;
	float phi_;
};