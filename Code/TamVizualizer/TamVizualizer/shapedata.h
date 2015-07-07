#ifndef GLSHAPE_H
#define GLSHAPE_H

#include <QtOpenGL>
#include <qquaternion.h>
//#include <glm/gtc/quaternion.hpp>
//#include <glm/gtx/quaternion.hpp>
#include "globaldata.h"

class GLSpace {
public:
	static void frustum(float half_width, float half_height, float v_near, float v_far);
	static void screenSize(int width, int height);

	static void calculateScreenPosition(float screen_x, float screen_y, float world_z, float &world_x, float &world_y);
	static void calculateScreenLength(float screen_length, float world_z, float &world_length);
	static void generateColor(int id);

	static float half_width, half_height, v_near, v_far;
	static int screen_width, screen_height;
};

class TAMShape {
public:
	virtual void draw() = 0;
};

class Line : public TAMShape {
public:
	Line(float x1, float y1, float x2, float y2, int color, float thick = 40) :
		x1(x1), y1(y1), x2(x2), y2(y2), thick(thick), color(color){}
	void draw() override;
private:
	float x1, y1, x2, y2, thick;
	int color;
};

class Circle : public TAMShape {
public:
	Circle(float x, float y, float radius, int color, bool doFill = false) :
		x(x), y(y), radius(radius), color(color), doFill(doFill) {}
	void draw() override;
private:
	float x, y, radius;
	int color;
	bool doFill;
};

class Finger : public TAMShape {
public:
	Finger(float x, float y, float size, int color) :
		x(x), y(y), size(size), color(color){}
	void draw() override;
private:
	float x, y, size;
	int color;
};

class SimpleCube : public TAMShape {
public:
	SimpleCube(float x, float y, float size) :
		x(x), y(y), size(size) {}
	void draw() override;
private:
	float x, y, size;
};

class WorldBox : public TAMShape {
public:
	WorldBox(QQuaternion quaternion, float size) :
		screen_x(GLSpace::screen_width / 2.0f), screen_y(GLSpace::screen_height / 2.0f),
		size(size), quaternion(quaternion) {}
	WorldBox(QQuaternion quaternion, float size, float screen_x, float screen_y) :
		screen_x(screen_x), screen_y(screen_y), size(size), quaternion(quaternion) {}
	void draw() override;
private:
	float screen_x, screen_y, size;
	QQuaternion quaternion;
	//float quaternion;
};

#endif