#pragma once
#include "myWrapper.h"
#include <QOpenGLWidget>
#include <Kinect.h>
#include <qbasictimer.h>


class GLWindow : public QOpenGLWidget
{
public:
	GLWindow();
	~GLWindow();
	static void draw(std::list<myWrapper*> line);
	void initializer();
	void kinectUpdate();

protected:
	void paintGL();
	void resizeGL(int width, int height);
	void initializeGL();
	
private:
	volatile bool isDrawing;
	IBodyFrameSource *body_source;
	IBodyFrameReader* body_reader;
	HRESULT hr;
	IKinectSensor * sensor;
	ICoordinateMapper *kinectMapper;
	Joint joints[JointType_Count];
	void timerEvent(QTimerEvent *e) Q_DECL_OVERRIDE;
	QBasicTimer timer;
	int fps;
	std::list<myWrapper *> bones;
	myBody*  bodi = new myBody();
	void drawBone(Joint joint0, Joint joint1);
};

