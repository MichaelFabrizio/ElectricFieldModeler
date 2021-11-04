#include "Shapes/RPrism.h"
#include <math.h>


RPrism::RPrism() : Shape{ 2 }, length(0), width(0), height(0), theta(0), phi(0) {}

int RPrism::Integrate()
{
	return length + width + height;
}

bool RPrism::CheckPointForOccupation(int i, int j, int k, float scale)
{
	//float xAdjusted = cos(theta) * (cos(phi) * (float)((i - x) / scale) + sin(phi) * (float)((j - y) / scale)) - (float)((k - z) / scale) * sin(theta);
	//float yAdjusted = -sin(phi) * (float)((i - x) / scale) + cos(phi) * (float)((j - y) / scale);
	//float zAdjusted = sin(theta) * (cos(phi) * (float)((i - x) / scale) + sin(phi) * (float)((j - y) / scale)) + (float)((k - z) / scale) * cos(theta);

	float xAdjusted = cos(theta) * (cos(phi) * ((float)i / scale - x) + sin(phi) * ((float)j / scale - y)) - ((float)k / scale - z) * sin(theta);
	float yAdjusted = -sin(phi) * ((float)i / scale - x) + cos(phi) * ((float)j / scale - y);
	float zAdjusted = sin(theta) * (cos(phi) * ((float)i / scale - x) + sin(phi) * ((float)j / scale - y)) + ((float)k / scale - z) * cos(theta);

	if (0 <= xAdjusted && xAdjusted <= length && 0 <= yAdjusted && yAdjusted <= width && 0 <= zAdjusted && zAdjusted <= height)
		return true;

	return false;
}