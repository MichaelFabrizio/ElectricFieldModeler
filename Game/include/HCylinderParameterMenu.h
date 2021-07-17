#pragma once
#include "Shapes/HCylinder.h"

class HCylinderParameterMenu
{
public:
	HCylinderParameterMenu();
	void Spawn(float xOffset, float yOffset);
	HCylinder GenerateObject();
	void ResetObject();

	bool ObjectPendingCreation;
private:
	bool attachEmpty_;
	float width_;
	float height_;
	char xbuf_[16];
	char ybuf_[16];
	char zbuf_[16];
	char radiusinnerbuf_[16];
	char radiusouterbuf_[16];
	char heightbuf_[16];
	char voltagebuf_[16];
	char namebuf_[32];
	float theta_;
	float phi_;

	char xbufempty_[16];
	char ybufempty_[16];
	char zbufempty_[16];
	char radiusbufempty_[16];
	char heightbufempty_[16];
	float thetaempty_;
	float phiempty_;
};