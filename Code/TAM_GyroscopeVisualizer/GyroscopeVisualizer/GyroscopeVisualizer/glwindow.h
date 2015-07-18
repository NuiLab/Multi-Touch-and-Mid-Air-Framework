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

	virtual void setData(float x, float y, float z, float w);
	
	public slots:
		void setGLData(float x, float y, float z, float w);

	protected:

		void initializeGL();					
		void paintGL();							
		void resizeGL(int width, int height);	

	private:
		float angle_w;
		float angle_x; 
		float angle_y; 
		float angle_z;
};
#endif 