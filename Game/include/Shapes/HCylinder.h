#pragma once
#include "Shape.h"
#include "Cylinder.h"

class HCylinder : public Shape
{
public:
	HCylinder();
	bool CheckPointForOccupation(int i, int j, int k, float scale) override;
	int Integrate() override;

	float innerRadius;
	float outerRadius;
	float height;
	float theta;
	float phi;
	bool emptyAttachment;

	Cylinder empty;
};