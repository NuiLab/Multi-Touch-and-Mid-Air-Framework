#pragma once
#include "api.h" 
#define SCREEN_SIZE 800 

class Wrapper
{
public:
	virtual void draw() = 0;
};

class Hand : public Wrapper
{
private:
	float x[5], y[5];	/* Holds the particular coordinates of this shape */
	int count;
public:
	Hand(float x, float y);	
	void draw() override;
	void setX(float x);
	void setY(float y);
};

class Helpers
{
public:
	static void mapper(float x, float y, float z, float w, float *new_x, float *new_y, float *new_z, float *new_w);
};
