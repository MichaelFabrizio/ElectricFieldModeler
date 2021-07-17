#include "Shapes/Sphere.h"

Sphere::Sphere() : Shape{ 3 }, radius(1) {}

int Sphere::Integrate()
{
	return 25;
}

bool Sphere::CheckPointForOccupation(int i, int j, int k, float scale)
{
	return false;
}