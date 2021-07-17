#pragma once
#include "Shapes/RPrism.h"

class RPrismParameterMenu
{
public:
	RPrismParameterMenu();
	void Spawn(float xOffset, float yOffset);
	RPrism GenerateObject();
	void ResetObject();

	bool ObjectPendingCreation;
private:
	float width_;
	float height_;
	char xbuf_[16];
	char ybuf_[16];
	char zbuf_[16];
	char lengthbuf_[16];
	char widthbuf_[16];
	char heightbuf_[16];
	char voltagebuf_[16];
	char namebuf_[32];
	float theta_;
	float phi_;
};