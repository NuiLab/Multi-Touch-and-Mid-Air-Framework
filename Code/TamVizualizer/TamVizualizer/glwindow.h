#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QtOpenGL>
#include <QGLWidget>
#include <ctime>

#include <fstream>
#include <sstream>

#include "globaldata.h"
#include "processorthread.h"
#include "shapedata.h"

# define PI_L          3.141592653589793238462643383279502884L /* pi */

using namespace std;

class GLWindow : public QOpenGLWidget {
	Q_OBJECT // must include this if you use Qt signals/slots
public:
	GLWindow(QWidget *parent = NULL);
	~GLWindow();

	void doResizeBrush(int i);	/*Changes the global brushSize attribute*/
	void doMap(int map);	/*Changes the global mapping attribute*/
	void setDisplay(DisplaySetting action);	/*Set the display mode for the window*/

	bool doSaveGesture(QString fileName, QString fileType);	/*Starts the save action by opening a save dialog native to the OS*/
	bool doOpenGesture(QString fileName, QString fileType);	/*Starts the open action by opening an open dialog native to the OS*/


	/*Slots used for communication between DrawArea and MainWindow*/
	public slots:
		void playback();
		void clearScreen();

protected:
	//QList<QColor> idColors;			/* A list of color objects*/


	bool event(QEvent *event);			/*The main function that gets triggered when a touch event happens*/

	void initializeGL();					/* Set up the rendering context, define display lists etc. */
	void paintGL();							/* Draws the scene onto the GLWindow*/
	void resizeGL(int width, int height);	/* setup viewport, projection etc. for the GLWindow*/

private:
	const float view_half_width = 0.2;
	const float view_near = 0.5;
	const float view_far = 30;

	int brushSize = 50;		/*Global size of the brush*/
	double mapping = 1;		/*Global value for mapping function*/
	int init_time = 0;		/*Global value of the starting of recording time*/
	DisplaySetting display_type = DisplaySetting::NONE;	/*Global number setting for the type of display*/
	bool playback_mode = false;

	//float angular_speed = 0;
	//float angular_pos = 0;

	float mouseX, mouseY;	/*Positions of the Mouse for OpenGL to use*/


	//QImage image;								/*Global image object to draw*/
	QList<touch_data> touch_list;				/*Global list of all the touch data*/
	//QList<QList<touch_data*>> finger_tracker;	/*touch data organized by the finger id*/
	QHash<int, int> fingers;					/*Global position of all 10 fingers currently*/


    //QBasicTimer timer;

	ProcessorThread *process;
	void sendDataToProcessThread();
	void updateData(touch_data data);				/*Update the touch data for the 10 fingers*/
	void drawScreenGL(QList<TAMShape *> shapes);

	//void timerEvent(QTimerEvent *e) Q_DECL_OVERRIDE;

	/*
	void drawLineGL(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, int color);
	void drawCircleGL(GLfloat x, GLfloat y, GLfloat radius, int color, bool doFill);
	void drawCubeGL(GLfloat x, GLfloat y, GLfloat size);
	void calculateScreenPosition(GLfloat screen_x, GLfloat screen_y, GLfloat world_z,
													GLfloat &world_x, GLfloat &world_y);
	void calculateScreenLength(GLfloat screen_length, GLfloat world_z, GLfloat &world_length);
	void generateColor(int id); */

	//void drawScreenGL();						/*Draws the screen along with the finger and hand positions on screen*/
	//void drawFingerGL(const touch_data &data);	/*Draws the fingers on the screen given the touch data*/
	//void drawCircularConnectionGL();			/*Draws the Circular Connection Display*/
	//void drawFingerMappingGL();					/*Draws the Finger Mapping*/
	//void drawAverageConnectionGL();				/*Draws the connection to the average of all the finger positions*/
	//void drawShortestMappingGL();				/*Draws the Connection of the shortest hamiltonian path to all the finger positions*/

	

	/* Shortest Hamiltonian Path given adjacency matrix */
	//QList<int> getShortestHamiltonianPath(QList< QList<int> > dist, int &res);
	/*Calculates circle given 3 touch data points*/
	//bool getCircleEff(touch_data data1, touch_data data2, touch_data data3, float &centerX, float &centerY, float &radius);
	/*Calculates circle given 3 touch data points*/
	//bool getCircle(touch_data data1, touch_data data2, touch_data data3, float &centerX, float &centerY, float &radius);
};
#endif // GLWINDOW_H