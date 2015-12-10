#pragma once
#include "api.h"
class myWrapper
{
public:

	virtual void draw() = 0;
};

class myBody : public myWrapper {

private:
	float x, y, endX, endY;
public:
	void draw() override;
	void setX(float x);
	void setY(float y);
	void setendX(float z);
	void setendY(float w);

};
static void mapper(float x, float y, float z, float w, float *new_x, float *new_y, float *new_z, float *new_w);
