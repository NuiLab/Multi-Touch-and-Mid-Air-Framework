#include "glwindow.h"

GLWindow::GLWindow(QWidget *parent) : QOpenGLWidget(parent) 
{
}

GLWindow::~GLWindow()
{
}

void GLWindow::setData(float x, float y, float z, float w)
{
	this->q_w = w;
	this->q_x = x;
	this->q_y = y;
	this->q_z = z;
}

void GLWindow::setGLData(float x, float y, float z, float w)
{
	this->setData(x, y, z, w);
}

QMatrix4x4 GLWindow::quatToMat(float w, float x, float y, float z)
{
	/*
	TODO: Gyroscope orientation needs to be modified to be taken "without" respect to the
	dongle, so that the corretionAngle need not be manually modified in order to get
	a correct rotation.
	*/

	QMatrix4x4 rotationMatrix;
	float correctionAngle = 180.0;

	rotationMatrix.setToIdentity();

	QQuaternion rotation(w, x, -y, -z);
	QQuaternion correction = QQuaternion::fromAxisAndAngle(0.0, 1.0, 0.0, correctionAngle);
	rotation.normalize();
	correction.normalize();
	rotation = correction * rotation;
	rotationMatrix.rotate(rotation);

	/*
	qDebug() << "(" << w << ", " << x << ", " << y << ", " << z << ")" << endl;
	*/
	return rotationMatrix;
}

void GLWindow::initializeGL()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(1, 1, 1, 0);
}

void GLWindow::resizeGL(int width, int height)
{
	glViewport(0, 0, (GLint)width, (GLint)height);

	/* create viewing cone with near and far clipping planes */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -1.0, 1.0, 5.0, 30.0);

	glMatrixMode(GL_MODELVIEW);
}

void GLWindow::paintGL()
{
	//delete color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Set up Initial Position of the gyroscope 
	glTranslatef(0.0f, 0.0f, -20.0f);		// Zoom out to view image

	// Rotate gyroscope representation
	QMatrix4x4 rotationMatrix = GLWindow::quatToMat(q_w, q_x, q_y, q_z);
	glMultMatrixf(rotationMatrix.constData());

	/* create 3D-Cube */
	glBegin(GL_QUADS);

	float size_x = 1.2f, size_y = 0.8f, size_z = 1.6f;
	glColor3f(0.4, 0.4, 0.4);

	//Front Face
	glVertex3f(size_x, size_y, size_z);
	glVertex3f(-size_x, size_y, size_z);
	glVertex3f(-size_x, -size_y, size_z);
	glVertex3f(size_x, -size_y, size_z);
	//Back Face
	glVertex3f(size_x, size_y, -size_z);
	glVertex3f(-size_x, size_y, -size_z);
	glVertex3f(-size_x, -size_y, -size_z);
	glVertex3f(size_x, -size_y, -size_z);
	//Top Face
	glVertex3f(-size_x, size_y, size_z);
	glVertex3f(size_x, size_y, size_z);
	glVertex3f(size_x, size_y, -size_z);
	glVertex3f(-size_x, size_y, -size_z);
	//Bottom Face
	glVertex3f(size_x, -size_y, size_z);
	glVertex3f(size_x, -size_y, -size_z);
	glVertex3f(-size_x, -size_y, -size_z);
	glVertex3f(-size_x, -size_y, size_z);
	//Right Face
	glVertex3f(size_x, size_y, size_z);
	glVertex3f(size_x, -size_y, size_z);
	glVertex3f(size_x, -size_y, -size_z);
	glVertex3f(size_x, size_y, -size_z);
	//Left Face
	glVertex3f(-size_x, size_y, size_z);
	glVertex3f(-size_x, -size_y, size_z);
	glVertex3f(-size_x, -size_y, -size_z);
	glVertex3f(-size_x, size_y, -size_z);
	glEnd();

	glBegin(GL_QUADS);
	float arrow_size = 3.0, arrow_thick = 0.1;
	//X Axis Arrow
	glColor3f(1.0, 0.0, 0.0);

	glVertex3f(arrow_size, arrow_thick, arrow_thick);
	glVertex3f(arrow_size, -arrow_thick, arrow_thick);
	glVertex3f(0, -arrow_thick, arrow_thick);
	glVertex3f(0, arrow_thick, arrow_thick);

	glVertex3f(arrow_size, arrow_thick, -arrow_thick);
	glVertex3f(arrow_size, -arrow_thick, -arrow_thick);
	glVertex3f(0, -arrow_thick, -arrow_thick);
	glVertex3f(0, arrow_thick, -arrow_thick);

	glVertex3f(arrow_size, arrow_thick, arrow_thick);
	glVertex3f(arrow_size, arrow_thick, -arrow_thick);
	glVertex3f(0, arrow_thick, -arrow_thick);
	glVertex3f(0, arrow_thick, arrow_thick);

	glVertex3f(arrow_size, -arrow_thick, arrow_thick);
	glVertex3f(arrow_size, -arrow_thick, -arrow_thick);
	glVertex3f(0, -arrow_thick, -arrow_thick);
	glVertex3f(0, -arrow_thick, arrow_thick);

	//Y Axis Arrow
	glColor3f(0.0, 1.0, 0.0);

	glVertex3f(arrow_thick, arrow_size, arrow_thick);
	glVertex3f(-arrow_thick, arrow_size, arrow_thick);
	glVertex3f(-arrow_thick, 0, arrow_thick);
	glVertex3f(arrow_thick, 0, arrow_thick);

	glVertex3f(arrow_thick, arrow_size, -arrow_thick);
	glVertex3f(-arrow_thick, arrow_size, -arrow_thick);
	glVertex3f(-arrow_thick, 0, -arrow_thick);
	glVertex3f(arrow_thick, 0, -arrow_thick);

	glVertex3f(arrow_thick, arrow_size, arrow_thick);
	glVertex3f(arrow_thick, arrow_size, -arrow_thick);
	glVertex3f(arrow_thick, 0, -arrow_thick);
	glVertex3f(arrow_thick, 0, arrow_thick);

	glVertex3f(-arrow_thick, arrow_size, arrow_thick);
	glVertex3f(-arrow_thick, arrow_size, -arrow_thick);
	glVertex3f(-arrow_thick, 0, -arrow_thick);
	glVertex3f(-arrow_thick, 0, arrow_thick);

	//Z Axis Arrow
	glColor3f(0.0, 0.0, 1.0);

	glVertex3f(arrow_thick, arrow_thick, arrow_size);
	glVertex3f(-arrow_thick, arrow_thick, arrow_size);
	glVertex3f(-arrow_thick, arrow_thick, 0);
	glVertex3f(arrow_thick, arrow_thick, 0);

	glVertex3f(arrow_thick, -arrow_thick, arrow_size);
	glVertex3f(-arrow_thick, -arrow_thick, arrow_size);
	glVertex3f(-arrow_thick, -arrow_thick, 0);
	glVertex3f(arrow_thick, -arrow_thick, 0);

	glVertex3f(arrow_thick, arrow_thick, arrow_size);
	glVertex3f(arrow_thick, -arrow_thick, arrow_size);
	glVertex3f(arrow_thick, -arrow_thick, 0);
	glVertex3f(arrow_thick, arrow_thick, 0);

	glVertex3f(-arrow_thick, arrow_thick, arrow_size);
	glVertex3f(-arrow_thick, -arrow_thick, arrow_size);
	glVertex3f(-arrow_thick, -arrow_thick, 0);
	glVertex3f(-arrow_thick, arrow_thick, 0);
	glEnd();

	float head_thick = arrow_thick * 3 / 2.0;

	//X Arrow Head
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(3.7, 0, 0);
	glVertex3f(2.7, head_thick, head_thick);
	glVertex3f(2.7, -head_thick, head_thick);
	glVertex3f(2.7, -head_thick, -head_thick);
	glVertex3f(2.7, head_thick, -head_thick);
	glVertex3f(2.7, head_thick, head_thick);
	glEnd();

	//Y Arrow Head
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 3.7, 0);
	glVertex3f(head_thick, 2.7, head_thick);
	glVertex3f(-head_thick, 2.7, head_thick);
	glVertex3f(-head_thick, 2.7, -head_thick);
	glVertex3f(head_thick, 2.7, -head_thick);
	glVertex3f(head_thick, 2.7, head_thick);
	glEnd();

	//Z Arrow 
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0, 3.7);
	glVertex3f(head_thick, head_thick, 2.7);
	glVertex3f(-head_thick, head_thick, 2.7);
	glVertex3f(-head_thick, -head_thick, 2.7);
	glVertex3f(head_thick, -head_thick, 2.7);
	glVertex3f(head_thick, head_thick, 2.7);

	glEnd();

	glPopMatrix();
	update();
}