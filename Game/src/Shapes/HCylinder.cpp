#include "Shapes/HCylinder.h"
#include "Shapes/Cylinder.h"
#include "SerializationManager.h"
#include <math.h>

HCylinder::HCylinder() : Shape{ 1 }, emptyAttachment(false)
{

}

int HCylinder::Integrate()
{
	return 0;
}

bool HCylinder::CheckPointForOccupation(int i, int j, int k, float scale)
{
	float xAdjusted = cos(theta) * (cos(phi) * (float)((i - x) / scale) + sin(phi) * (float)((j - y) / scale)) - (float)((k - z) / scale) * sin(theta);
	float yAdjusted = -sin(phi) * (float)((i - x) / scale) + cos(phi) * (float)((j - y) / scale);
	float zAdjusted = sin(theta) * (cos(phi) * (float)((i - x) / scale) + sin(phi) * (float)((j - y) / scale)) + (float)((k - z) / scale) * cos(theta);

	double xSquared = pow(xAdjusted, 2.0);
	double ySquared = pow(yAdjusted, 2.0);
	float radialdistance = (float)sqrt(xSquared + ySquared);

	if (0 <= zAdjusted && zAdjusted <= height && innerRadius <= fabs(radialdistance) && fabs(radialdistance) <= outerRadius)
	{
		if (emptyAttachment)
		{
			float xAdjustedEmpty = cos(empty.theta_) * (cos(empty.phi_) * (float)((i - empty.x) / scale) + sin(empty.phi_) * (float)((j - empty.y) / scale)) - (float)((k - empty.z) / scale) * sin(empty.theta_);
			float yAdjustedEmpty = -sin(empty.phi_) * (float)((i - empty.x) / scale) + cos(empty.phi_) * (float)((j - empty.y) / scale);
			float zAdjustedEmpty = sin(empty.theta_) * (cos(empty.phi_) * (float)((i - empty.x) / scale) + sin(empty.phi_) * (float)((j - empty.y) / scale)) + (float)((k - empty.z) / scale) * cos(empty.theta_);

			double xSquaredEmpty = pow(xAdjustedEmpty, 2.0);
			double ySquaredEmpty = pow(yAdjustedEmpty, 2.0);
			float radialdistanceEmpty = (float)sqrt(xSquaredEmpty + ySquaredEmpty);

			if (0 <= zAdjustedEmpty && zAdjustedEmpty <= empty.height_ && fabs(radialdistanceEmpty) <= empty.radius_)
			{
				return false;
			}
		}
		return true;
	}

	return false;
}