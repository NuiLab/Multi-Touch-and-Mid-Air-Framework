#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QtOpenGL>
#include <QGLWidget>

using namespace std;

class GLWindow : public QOpenGLWidget 
{
	Q_OBJECT 

public:
	GLWindow(QWidget *parent = NULL);
	~GLWindow();

	static QMatrix4x4 quatToMat(float w, float x, float y, float z);

	virtual void setData(float x, float y, float z, float w);

	public slots:
		void setGLData(float x, float y, float z, float w);

	protected:

		void initializeGL();					
		void paintGL();							
		void resizeGL(int width, int height);	

	private:
		float q_w;
		float q_x; 
		float q_y; 
		float q_z;
};
#endif 