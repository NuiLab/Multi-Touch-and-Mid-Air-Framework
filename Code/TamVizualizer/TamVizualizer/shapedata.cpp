#include "shapedata.h"
float GLSpace::half_width;
float GLSpace::half_height;
float GLSpace::v_near;
float GLSpace::v_far;
int GLSpace::screen_height;
int GLSpace::screen_width;

void GLSpace::frustum(float half_width, float half_height, float v_near, float v_far) {
	GLSpace::half_width = half_width;
	GLSpace::half_height = half_height;
	GLSpace::v_near = v_near;
	GLSpace::v_far = v_far;
}

void GLSpace::screenSize(int width, int height){
	screen_width = width;
	screen_height = height;
}

void GLSpace::calculateScreenPosition(GLfloat screen_x, GLfloat screen_y, GLfloat world_z, GLfloat &world_x, GLfloat &world_y){
	world_x = (world_z / v_near)*(2.0f*half_width*(screen_x / screen_width) - half_width);
	world_y = (world_z / v_near)*(2.0f*half_width*((screen_height - screen_y) / screen_height) - half_width)*(screen_height / (float)screen_width);
}

void GLSpace::calculateScreenLength(GLfloat screen_length, GLfloat world_z, GLfloat &world_length){
	world_length = (world_z / v_near)*(2.0f*half_width*(screen_length / screen_width));
}

void GLSpace::generateColor(int id){
	float r, g, b;
	r = (((id + 1) * 07) % 19) / 20.0f;
	g = (((id + 3) * 11) % 19) / 20.0f;
	b = (((id + 5) * 13) % 19) / 20.0f;
	//qDebug() << "Color chosen: (" << r << ',' << g << ',' << b << ')' << endl;
	glColor3f(r, g, b);
}





void Line::draw(){
	float xA, yA, xB, yB;
	GLSpace::calculateScreenPosition(x1, y1, GLSpace::v_near, xA, yA);
	GLSpace::calculateScreenPosition(x2, y2, GLSpace::v_near, xB, yB);

	glPushMatrix();
	glLoadIdentity();
	glLineWidth(thick);
	glBegin(GL_LINES);
	GLSpace::generateColor(color);
	glVertex3f(xA, yA, -GLSpace::v_near);	//x1 y1
	glVertex3f(xB, yB, -GLSpace::v_near);	//x2 y2
	glEnd();
	glPopMatrix();

	//qDebug() << "Line" << endl;
}

void Circle::draw(){
	/* Credit for Drawing Circle: https://gist.github.com/strife25/803118 */
	float centerX, centerY, centerZ = -GLSpace::v_near;
	float world_radius;
	GLSpace::calculateScreenPosition(x, y, GLSpace::v_near, centerX, centerY);
	GLSpace::calculateScreenLength(radius, GLSpace::v_near, world_radius);

	int i;
	int triangleAmount = 20; //# of triangles used to draw circle

	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * PI_L;

	glPushMatrix();
	glLoadIdentity();
	//glTranslatef(0.0f, 0.0f, -20.0f); //move along z-axis

	GLSpace::generateColor(color);

	if (doFill){
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(centerX, centerY, centerZ); // center of circle
	} else {
		glBegin(GL_LINE_LOOP);
	}

	for (i = 0; i <= triangleAmount; i++) {
		glVertex3f(centerX + (world_radius * cos(i * twicePi / triangleAmount)),
			centerY + (world_radius * sin(i * twicePi / triangleAmount)), centerZ);
	}
	glEnd();
	glPopMatrix();

	//qDebug() << "Circle" << endl;
}

void Finger::draw(){
	float brush_size = size * 3 / 4.0f;// *0.02f*0.01f;

	glLineWidth(brush_size / 5.0f);
	Circle circle1(x, y, brush_size, color);
	Circle circle2(x, y, (brush_size * 2) / 3.0f, color);
	circle1.draw();
	circle2.draw();
}

void SimpleCube::draw(){
	static int angular_pos = 0;
	float centerX, centerY, centerZ = -(GLSpace::v_near + GLSpace::v_far) / 2.0f;
	float world_size;
	GLSpace::calculateScreenPosition(x, y, -centerZ, centerX, centerY);
	GLSpace::calculateScreenLength(size, -centerZ, world_size);

	glPushMatrix();
	glLoadIdentity();
	//glTranslatef(0.0f, 0.0f, -20.0f); //move along z-axis
	glTranslatef(centerX, centerY, centerZ);
	glScalef(world_size, world_size, world_size);
	glRotatef(30.0, 0.0, 1.0, 0.0); //rotate 30 degress around y-axis
	glRotatef(angular_pos, 1.0, 0.0, 0.0); //rotate 15 degress around x-axis

	angular_pos += 15;
	if (angular_pos > 360) angular_pos -= 360;

	/* create 3D-Cube */
	glBegin(GL_QUADS);

	//front
	glColor3f(1.0, 0.0, 0.0);

	glVertex3f(1.0, 1.0, 1.0);
	glVertex3f(-1.0, 1.0, 1.0);
	glVertex3f(-1.0, -1.0, 1.0);
	glVertex3f(1.0, -1.0, 1.0);


	//back
	glColor3f(0.0, 1.0, 0.0);

	glVertex3f(1.0, 1.0, -1.0);
	glVertex3f(-1.0, 1.0, -1.0);
	glVertex3f(-1.0, -1.0, -1.0);
	glVertex3f(1.0, -1.0, -1.0);


	//top
	glColor3f(0.0, 0.0, 1.0);

	glVertex3f(-1.0, 1.0, 1.0);
	glVertex3f(1.0, 1.0, 1.0);
	glVertex3f(1.0, 1.0, -1.0);
	glVertex3f(-1.0, 1.0, -1.0);


	//bottom
	glColor3f(0.0, 1.0, 1.0);

	glVertex3f(1.0, -1.0, 1.0);
	glVertex3f(1.0, -1.0, -1.0);
	glVertex3f(-1.0, -1.0, -1.0);
	glVertex3f(-1.0, -1.0, 1.0);

	//right
	glColor3f(1.0, 0.0, 1.0);

	glVertex3f(1.0, 1.0, 1.0);
	glVertex3f(1.0, -1.0, 1.0);
	glVertex3f(1.0, -1.0, -1.0);
	glVertex3f(1.0, 1.0, -1.0);


	//left
	glColor3f(1.0, 1.0, 0.0);

	glVertex3f(-1.0, 1.0, 1.0);
	glVertex3f(-1.0, -1.0, 1.0);
	glVertex3f(-1.0, -1.0, -1.0);
	glVertex3f(-1.0, 1.0, -1.0);

	glPopMatrix();
	glEnd();
}


void WorldBox::draw(){
	static float angular_pos = 0;
	float centerX, centerY, centerZ = -(GLSpace::v_near + GLSpace::v_far) / 2.0f;
	float world_size;
	GLSpace::calculateScreenPosition(screen_x, screen_y, -centerZ, centerX, centerY);
	GLSpace::calculateScreenLength(size, -centerZ, world_size);

	glPushMatrix();
	glLoadIdentity();

	glTranslatef(centerX, centerY, centerZ);
	glScalef(world_size, world_size, world_size);

	/*
	QVector4D quat = quaternion.toVector4D();
	float denom = sqrt(1.0 - (quat.w()*quat.w()));
	glRotatef(2*acos(quat.w()), quat.x() / denom, quat.y() / denom, quat.z() / denom);
	*/
	
	glRotatef(angular_pos, 1.0, 0.0, 0.0); //rotate 15 degress around x-axis
	glRotatef(2*angular_pos, 0.0, 1.0, 0.0); //rotate 15 degress around x-axis
	glRotatef(3*angular_pos, 0.0, 0.0, 1.0); //rotate 15 degress around x-axis
	angular_pos += 0.5;
	if (angular_pos > 360) angular_pos -= 360;

	/* create 3D-Cube */
	glBegin(GL_QUADS);

	float size_x = 1.2f, size_y = 0.8f, size_z = 1.6f;
	glColor3f(0.4, 0.4, 0.4);
	
	//Front Face
	glVertex3f( size_x,  size_y,  size_z);
	glVertex3f(-size_x,  size_y,  size_z);
	glVertex3f(-size_x, -size_y,  size_z);
	glVertex3f( size_x, -size_y,  size_z);
	//Back Face
	glVertex3f( size_x,  size_y, -size_z);
	glVertex3f(-size_x,  size_y, -size_z);
	glVertex3f(-size_x, -size_y, -size_z);
	glVertex3f( size_x, -size_y, -size_z);
	//Top Face
	glVertex3f(-size_x,  size_y,  size_z);
	glVertex3f( size_x,  size_y,  size_z);
	glVertex3f( size_x,  size_y, -size_z);
	glVertex3f(-size_x,  size_y, -size_z);
	//Bottom Face
	glVertex3f( size_x, -size_y,  size_z);
	glVertex3f( size_x, -size_y, -size_z);
	glVertex3f(-size_x, -size_y, -size_z);
	glVertex3f(-size_x, -size_y,  size_z);
	//Right Face
	glVertex3f( size_x,  size_y,  size_z);
	glVertex3f( size_x, -size_y,  size_z);
	glVertex3f( size_x, -size_y, -size_z);
	glVertex3f( size_x,  size_y, -size_z);
	//Left Face
	glVertex3f(-size_x,  size_y,  size_z);
	glVertex3f(-size_x, -size_y,  size_z);
	glVertex3f(-size_x, -size_y, -size_z);
	glVertex3f(-size_x,  size_y, -size_z);
	glEnd();

	glBegin(GL_QUADS);
	float arrow_size = 3.0, arrow_thick = 0.1;
	//X Axis Arrow
	glColor3f(1.0, 0.0, 0.0);

	glVertex3f(arrow_size,  arrow_thick,  arrow_thick);
	glVertex3f(arrow_size, -arrow_thick,  arrow_thick);
	glVertex3f(0		 , -arrow_thick,  arrow_thick);
	glVertex3f(0		 ,  arrow_thick,  arrow_thick);

	glVertex3f(arrow_size,  arrow_thick, -arrow_thick);
	glVertex3f(arrow_size, -arrow_thick, -arrow_thick);
	glVertex3f(0		 , -arrow_thick, -arrow_thick);
	glVertex3f(0		 ,  arrow_thick, -arrow_thick);

	glVertex3f(arrow_size,  arrow_thick,  arrow_thick);
	glVertex3f(arrow_size,  arrow_thick, -arrow_thick);
	glVertex3f(0		 ,  arrow_thick, -arrow_thick);
	glVertex3f(0		 ,  arrow_thick,  arrow_thick);

	glVertex3f(arrow_size, -arrow_thick,  arrow_thick);
	glVertex3f(arrow_size, -arrow_thick, -arrow_thick);
	glVertex3f(0		 , -arrow_thick, -arrow_thick);
	glVertex3f(0		 , -arrow_thick,  arrow_thick);

	//Y Axis Arrow
	glColor3f(0.0, 1.0, 0.0);

	glVertex3f( arrow_thick, arrow_size,  arrow_thick);
	glVertex3f(-arrow_thick, arrow_size,  arrow_thick);
	glVertex3f(-arrow_thick, 0		   ,  arrow_thick);
	glVertex3f( arrow_thick, 0		   ,  arrow_thick);

	glVertex3f( arrow_thick, arrow_size, -arrow_thick);
	glVertex3f(-arrow_thick, arrow_size, -arrow_thick);
	glVertex3f(-arrow_thick, 0		   , -arrow_thick);
	glVertex3f( arrow_thick, 0		   , -arrow_thick);

	glVertex3f( arrow_thick, arrow_size,  arrow_thick);
	glVertex3f( arrow_thick, arrow_size, -arrow_thick);
	glVertex3f( arrow_thick, 0		   , -arrow_thick);
	glVertex3f( arrow_thick, 0		   ,  arrow_thick);

	glVertex3f(-arrow_thick, arrow_size,  arrow_thick);
	glVertex3f(-arrow_thick, arrow_size, -arrow_thick);
	glVertex3f(-arrow_thick, 0		   , -arrow_thick);
	glVertex3f(-arrow_thick, 0		   ,  arrow_thick);

	//Z Axis Arrow
	glColor3f(0.0, 0.0, 1.0);

	glVertex3f( arrow_thick,  arrow_thick, arrow_size);
	glVertex3f(-arrow_thick,  arrow_thick, arrow_size);
	glVertex3f(-arrow_thick,  arrow_thick, 0		 );
	glVertex3f( arrow_thick,  arrow_thick, 0		 );

	glVertex3f( arrow_thick, -arrow_thick, arrow_size);
	glVertex3f(-arrow_thick, -arrow_thick, arrow_size);
	glVertex3f(-arrow_thick, -arrow_thick, 0		 );
	glVertex3f( arrow_thick, -arrow_thick, 0		 );

	glVertex3f( arrow_thick,  arrow_thick, arrow_size);
	glVertex3f( arrow_thick, -arrow_thick, arrow_size);
	glVertex3f( arrow_thick, -arrow_thick, 0		 );
	glVertex3f( arrow_thick,  arrow_thick, 0		 );

	glVertex3f(-arrow_thick,  arrow_thick, arrow_size);
	glVertex3f(-arrow_thick, -arrow_thick, arrow_size);
	glVertex3f(-arrow_thick, -arrow_thick, 0		 );
	glVertex3f(-arrow_thick,  arrow_thick, 0		 );
	glEnd();

	float head_thick = arrow_thick * 3 / 2.0;

	//X Arrow Head
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(3.7, 0, 0);
	glVertex3f(2.7,  head_thick,  head_thick);
	glVertex3f(2.7, -head_thick,  head_thick);
	glVertex3f(2.7, -head_thick, -head_thick);
	glVertex3f(2.7,  head_thick, -head_thick);
	glVertex3f(2.7,  head_thick,  head_thick);
	glEnd();

	//Y Arrow Head
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 3.7, 0);
	glVertex3f( head_thick, 2.7,  head_thick);
	glVertex3f(-head_thick, 2.7,  head_thick);
	glVertex3f(-head_thick, 2.7, -head_thick);
	glVertex3f( head_thick, 2.7, -head_thick);
	glVertex3f( head_thick, 2.7,  head_thick);
	glEnd();

	//Z Arrow 
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0, 3.7);
	glVertex3f( head_thick,  head_thick, 2.7);
	glVertex3f(-head_thick,  head_thick, 2.7);
	glVertex3f(-head_thick, -head_thick, 2.7);
	glVertex3f( head_thick, -head_thick, 2.7);
	glVertex3f( head_thick,  head_thick, 2.7);
	glEnd();

	glPopMatrix();
}