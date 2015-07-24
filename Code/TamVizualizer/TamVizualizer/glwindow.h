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
#include "debugwindow.h"

using namespace std;

/* The GLWindow is the Window that will:
   Draw to the screen, take in touch / mouse events, 
   render OpenGL 3D graphics, and so on. */
class GLWindow : public QOpenGLWidget {
	Q_OBJECT	// Must include this if you use Qt signals/slots

public:
	GLWindow(QWidget *parent = NULL);
	~GLWindow();

	/* Changes the global brushSize attribute */
	void doResizeBrush(int i);
	/* Changes the global mapping attribute */
	void doMap(int map);
	/* Set the display mode for the window */
	void setDisplay(DisplaySetting action);	
	/* Starts the save action by opening a save dialog native to the OS */
	bool doSaveGesture(QString fileName, QString fileType);
	/* Starts the open action by opening an open dialog native to the OS */
	bool doOpenGesture(QString fileName, QString fileType);
	/* For the purpose of loading  in your own OBJ models */
	bool doLoadOBJFile(string path);

	/* Slots used for communication between GLWindow and MainWindow */
	public slots:
		void playback();
		void clearScreen();

protected:
	/*The main function that gets triggered when a touch event happens*/
	bool event(QEvent *event);
	/* Set up the rendering context, define display lists etc. */
	void initializeGL();
	/* Draws the scene onto the GLWindow*/
	void paintGL();
	/* setup viewport, projection etc. for the GLWindow*/
	void resizeGL(int width, int height);

private:
	/* Global Data for Frustum */
	/*	WARNING: these constant values must NOT be changed, as the methods
		that they are used for have not been fully tested (GLSpace methods).
		These values are the only guaranteed values to work at the moment,
		until the GLSpace methods are fully tested */
	const float view_half_width = 0.2;
	const float view_near = 0.5;
	const float view_far = 30;
	double mapping = 1;				/*Global value for mapping function*/
	int init_time = 0;				/*Global value of the starting of recording time*/

	DisplaySetting display_type = DisplaySetting::NONE;	/*Global number setting for the type of display*/
	float mouseX, mouseY;								/*Positions of the Mouse for OpenGL to use*/
	volatile bool playback_mode = false;				/* Check if the window is in playback-mode*/

	QList<touch_data> touch_list;				/*Global list of all the touch data*/
	QHash<int, int> fingers;					/*Global position of all 10 fingers currently*/

	/* Variables to set a drawing-speed (FPS) */
	QBasicTimer timer;			/* Timer to set frames per second */
	volatile bool isDrawing;	/* Used to prevent multiple calls to update */
	int fps;					/* Frames per second to update the timer */

	ProcessorThread *process;	/* Processor Thread to run the calculations on */
	
	/* Send finger data to the ProcessorThread */
	void sendDataToProcessThread();
	/* Update the touch data for multiple fingers */
	void updateData(touch_data data);
	/* Draw the list of TAMShapes to the screen */
	void drawScreenGL(QList<TAMShape *> shapes);

	/* Timer for setting a frame-per-second drawing rate */
	void timerEvent(QTimerEvent *e) Q_DECL_OVERRIDE;
};
#endif // GLWINDOW_H