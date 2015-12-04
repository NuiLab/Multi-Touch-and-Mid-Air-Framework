#include "glwindow.h"

GLWindow::GLWindow()
{
	setWindowTitle(tr("LeapGL"));
	resize(SCREEN_SIZE, SCREEN_SIZE);
	input = new InputHandler();
}

GLWindow::~GLWindow()
{
	delete input;
}

/* Initializes the OpenGL Context*/
void GLWindow::initializeGL()
{
	TAM::VisualizerHandler<TAM::Shape>::initGL(NULL, 1.0f, 1.0f, 1.0f, 1.0f);
	timer.start(1000 / fps, Qt::TimerType::PreciseTimer, this);
}

/* This is where all the drawing is handled*/
void GLWindow::paintGL()
{
	isDrawing = true;

	std::list<InputHandler *> dummy;
	dummy.push_back(input);
	TAM::VisualizerHandler<InputHandler>::paintGL(&GLWindow::drawScreenGL, dummy);

	isDrawing = false;
}

void GLWindow::resizeGL(int width, int height)
{
	TAM::VisualizerHandler<TAM::Shape>::resizeGL(NULL, width, height);
}

/* Gathers the updated hand object from the InputHandler */
void GLWindow::drawScreenGL(std::list<InputHandler *> shapes){	
	shapes.front()->getResults()->draw();	
}

/* Timer for setting a frame-per-second drawing rate */
void GLWindow::timerEvent(QTimerEvent *) 
{	
	if (!isDrawing){
		update();
	}
}