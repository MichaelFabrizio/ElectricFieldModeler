#include "Shapes/Cylinder.h"
#include "Shapes/Shape.h"
#include "MCIntegrator.h"
#include "SerializationManager.h"
#include <iostream>
#include <math.h>

Cylinder::Cylinder() : Shape{ 0 }, radius_(1), height_(1), theta_(0), phi_(0)
{
}

int Cylinder::Integrate()
{
	float rprimex = 5;
	float rprimey = 5;
	float rprimez = 20;
	return radius_ + height_;
}

bool Cylinder::CheckPointForOccupation(int i, int j, int k, float scale)
{
	//Almost positive these explicit casts can be optimized. Or at least handled in a more graceful way.
	float xAdjusted = cos(theta_) * (cos(phi_) * ((float)i / scale - x) + sin(phi_) * ((float)j / scale - y)) - ((float)k / scale - z) * sin(theta_);
	float yAdjusted = -sin(phi_) * ((float)i / scale - x) + cos(phi_) * ((float)j / scale - y);
	float zAdjusted = sin(theta_) * (cos(phi_) * ((float)i / scale - x) + sin(phi_) * ((float)j / scale - y)) + ((float)k / scale - z) * cos(theta_);

	double xSquared = pow(xAdjusted, 2.0);
	double ySquared = pow(yAdjusted, 2.0);
	float radialdistance = (float)sqrt(xSquared + ySquared);

	if (0 <= zAdjusted && zAdjusted <= height_ && fabs(radialdistance) <= radius_)
		return true;

	return false;
}