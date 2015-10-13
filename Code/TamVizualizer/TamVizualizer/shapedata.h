#ifndef GLSHAPE_H
#define GLSHAPE_H
#define _USE_MATH_DEFINES

#include <glew.h>
#include <QGLWidget>
#include <qquaternion.h>
#include "globaldata.h"
#include "debugwindow.h"
#include <qdebug.h>
#include <time.h>
#include <string>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.hpp>
#include <chrono>

static const short X = 0;
static const short Y = 1;
static const short Z = 2;
static auto t_start = std::chrono::high_resolution_clock::now();
/* Class used solely for assistance in resizing and placing shapes on screen based on the Frustum settings
(Not a fully tested feature for the various frustum setting, only the current one)
 */
class GLSpace {
public:
	// GL Ratio Constants (Only for current frustum setting)
	static const float MODEL_DEPTH_SCREEN_WIDTH_RATIO;// = 16;
	static const float MODEL_DIST_TRANSLATE_RATIO;// = 0.00625f;  // (1 / 160) translate ratio

	static void frustum(float half_width, float half_height, float v_near, float v_far);
	static void screenSize(int width, int height);

	static void calculateScreenPosition(float screen_x, float screen_y, float world_z, float &world_x, float &world_y);
	static void calculateScreenLength(float screen_length, float world_z, float &world_length);
	static void getGLScreenCoordinates(float screen_x, float screen_y, float &world_x, float &world_y);
	static void generateColor(int id);
	static void generateColor(int id, float &r, float &g, float &b);	
	/* Initialize all the lighting necessary for the OpenGL Environment */
	static void initGLLighting();

	static float half_width, half_height, v_near, v_far;
	static int screen_width, screen_height;

private:
	static bool verified;

	/* Verify values are proper */
	static void verify();
};

/* Holds a set of coordinates for a given shape */
class ShapeCoordinates
{
public:
	ShapeCoordinates(float x, float y) : x(x), y(y){}
	float getXCoordinate();
	float getYCoordinate();
private:
	float x, y;
};

/* Interface for which the window will use to draw all kinds of shapes created */
class TAMShape {
public:
	virtual void draw() = 0;
	virtual list<ShapeCoordinates> getCoordinates() = 0;
};

/* Shape to draw line in front of camera given two (x,y) coordinates */
class Line : public TAMShape {
public:
	float x1, y1, x2, y2, thick;
	int color;

	Line(float x1, float y1, float x2, float y2, int color, float thick = 10) :
		x1(x1), y1(y1), x2(x2), y2(y2), thick(thick), color(color){}
	void draw() override;
	list<ShapeCoordinates> getCoordinates() override;
};

/* Shape to draw circle in front of camera given one (x,y) coordinate and its radius */
class Circle : public TAMShape {
public:
	float x, y, radius;
	int color;
	bool doFill;

	Circle(float x, float y, float radius, int color, bool doFill = false) :
		x(x), y(y), radius(radius), color(color), doFill(doFill) {}
	void draw() override;
	list<ShapeCoordinates> getCoordinates() override;
};

/* Shape used to draw the design for when touch is detected */
class Finger : public TAMShape {
public:
	float x, y, size;
	int color;

	Finger(float x, float y, float size, int color) :
		x(x), y(y), size(size), color(color){}
	void draw() override;
	list<ShapeCoordinates> getCoordinates() override;
};

/* A Simple Cube. This was merely for testing purposes */
class SimpleCube : public TAMShape {
public:
	float x, y, size;

	SimpleCube(float x, float y, float size) :
		x(x), y(y), size(size) {}
	void draw() override;
	list<ShapeCoordinates> getCoordinates() override;
};

/* A Box with X-Y-Z Axis Arrows, to display 3D-rotations to the user
(Not developed for the current lighting enabled) */
class WorldBox : public TAMShape {
public:
	WorldBox(QQuaternion quaternion, float size, float screen_x = (GLSpace::screen_width / 2.0f), float screen_y = (GLSpace::screen_height / 2.0f)) :
		quaternion(quaternion), size(size), screen_x(screen_x), screen_y(screen_y){}
	void draw() override;
	list<ShapeCoordinates> getCoordinates() override;
private:
	float screen_x, screen_y, size;
	QQuaternion quaternion;
	//float quaternion;
};

/* An OBJ Parser and renderer to draw in your own OBJ models in the application
NOTE: This is self-made so it runs only in certain types of OBJ files.
The file must only use triangles as faces, and cannot have vertex textures in the file.
The parser ignores vertex normals currently and generates its own normals.
Color is set randomly in the initialization and cannot be changed */
class OBJModel : public TAMShape {
public:
	OBJModel(string path);
	bool exists();
	void draw() override;
	list<ShapeCoordinates> getCoordinates() override;
private:
	// Boolean to test if model loaded successfully
	bool isWorking;

	// Translation Mode Constants
	const short ROTATE = 0;
	const short TRANSLATE = 1;
	const short SCALE = 2;
	const float ROTATE_OFFSET = 10.0f;
	const float SCALE_OFFSET = 0.5f;

	// OBJ Parsing Data
	vector<float> mVertices[3];
	vector<float> mNormals[3];
	vector<float> mFaces[3];
	float minBound[3];
	float maxBound[3];
	float mCenter[3];
	float model_distance;

	// Transformation Data
	float mTransform[3][2]; // mTransform[Transform Mode][X-Y-Axis]
	short int transfMode = ROTATE;
	float offset = ROTATE_OFFSET;

	// Functions
	void generateNormals();
	void extractOBJdata(string fp);
};
#endif