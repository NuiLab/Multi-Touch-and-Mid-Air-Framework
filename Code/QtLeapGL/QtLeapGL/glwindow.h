#pragma once
#include "Leap.h"
#include "InputHandler.h"
#include "Wrapper.h"
#include <QOpenGLWidget>
#include <qbasictimer.h>



class GLWindow : public QOpenGLWidget
{
public:
	GLWindow();
	~GLWindow();
protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);

private:
	volatile bool isDrawing;	/* Used to prevent multiple calls to update */

	InputHandler *input;
	static void drawScreenGL(std::list<InputHandler *> shapes);
	QBasicTimer timer;
	int fps;

	/* Timer for setting a frame-per-second drawing rate */
	void timerEvent(QTimerEvent *e) Q_DECL_OVERRIDE;
};

