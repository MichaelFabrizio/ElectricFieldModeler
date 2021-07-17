#pragma once
#include "Shapes/Sphere.h"

class SphereParameterMenu
{
public:
	SphereParameterMenu();
	void Spawn(float xOffset, float yOffset);
	Sphere GenerateObject();
	void ResetObject();

	bool ObjectPendingCreation;
private:

	float width_;
	float height_;
	char xbuf_[16];
	char ybuf_[16];
	char zbuf_[16];
	char radiusbuf_[16];
	char namebuf_[32];
};