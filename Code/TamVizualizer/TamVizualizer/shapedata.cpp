#include "shapedata.h"


float GLSpace::half_width;
float GLSpace::half_height;
float GLSpace::v_near;
float GLSpace::v_far;
int GLSpace::screen_height;
int GLSpace::screen_width;

const float GLSpace::MODEL_DEPTH_SCREEN_WIDTH_RATIO = 16;
const float GLSpace::MODEL_DIST_TRANSLATE_RATIO = 0.00625f;  // (1 / 160) translate ratio
bool GLSpace::verified = false;

/*	Sets up the frustum for the camera, and will help to determine screen position / length calculations later on
	Code to verify float value: http://www.cplusplus.com/reference/cmath/isfinite/	*/
void GLSpace::frustum(float window_half_width, float window_half_height, float view_near, float view_far) {
	half_width = window_half_width;
	half_height = window_half_height;
	v_near = view_near;
	v_far = view_far;
	verify();
}

/* Sets the screen size used for screen position / length calculations */
void GLSpace::screenSize(int width, int height){
	screen_width = width;
	screen_height = height;
	verify();
}

/* Verify that all values are proper */
void GLSpace::verify(){
	//verified = fpclassify(half_width) == FP_NORMAL && fpclassify(half_height) == FP_NORMAL && fpclassify(v_near) == FP_NORMAL
	//	&& fpclassify(v_far) == FP_NORMAL && half_width > 0 && half_height > 0 && v_near > 0 && v_far > v_near && screen_width > 0 && screen_height > 0;
	verified = isfinite(half_width) && isfinite(half_height) && isfinite(v_near) && isfinite(v_far) && isfinite(half_height)
		&& half_width > 0 && half_height > 0 && v_near > 0 && v_far > v_near && screen_width > 0 && screen_height > 0;
}

/* Used to calculate where to draw in OpenGL space based on screen coordinates given and the position of the model in the z-axis (how far away it is) */
void GLSpace::calculateScreenPosition(GLfloat screen_x, GLfloat screen_y, GLfloat world_z, GLfloat &world_x, GLfloat &world_y) {
	if (!verified) throw 0;
	world_x = (world_z / v_near)*(2.0f*half_width*(screen_x / screen_width) - half_width);
	world_y = (world_z / v_near)*(2.0f*half_width*((screen_height - screen_y) / screen_height) - half_width)*(screen_height / (float)screen_width);
}

/* Used to calculate how large something is in OpenGL space based on the length interpreted on screen and the position of the length in the z-axis (how far away it is) */
void GLSpace::calculateScreenLength(GLfloat screen_length, GLfloat world_z, GLfloat &world_length) {
	if (!verified) throw 0;
	world_length = (world_z / v_near)*(2.0f*half_width*(screen_length / screen_width));
}

/* Used to generate a random color in OpenGL. Simple random generator based on an integer and modulus with primes */
void GLSpace::generateColor(int id){
	float r, g, b;
	r = (((id + 1) * 07) % 19) / 20.0f;
	g = (((id + 3) * 11) % 19) / 20.0f;
	b = (((id + 5) * 13) % 19) / 20.0f;

	glColor3f(r, g, b);
}

/* Initialize all the lighting necessary for the OpenGL Environment*/
void GLSpace::initGLLighting() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	srand((unsigned int)time(NULL));
	GLfloat mat_diffuse[4] = { (rand() % 256) / 255.0f, (rand() % 256) / 255.0f, (rand() % 256) / 255.0f, 0.0 };
	GLfloat mat_specular[4] = { 0.55, 0.55, 0.55, 1.0 };
	GLfloat mat_shininess[1] = { 128 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	GLfloat light0_ambient[4] = { 0.1745, 0.01175, 0.01175, 1.0 };
	GLfloat light0_color[4] = { 0.75, 0.75, 0.75, 1.0 };
	GLfloat light_position[4] = { 0, 0, 1, 0.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_color);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_color);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}





/* Draw a Line in OpenGL */
void Line::draw(){
	if (!(isfinite(x1) && isfinite(y1) && isfinite(x2) && isfinite(y2) && isfinite(thick) && (x1 != x2 || y1 != y2))) return;

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
}

/* Draw a Circle in OpenGL */
void Circle::draw(){
	if (!(isfinite(x) && isfinite(y) && isfinite(radius) && radius > 0)) return;

	static const GLfloat twicePi = 2.0f * PI_L;

	/* Credit for Drawing Circle: https://gist.github.com/strife25/803118 */
	float centerX, centerY, centerZ = -GLSpace::v_near;
	float world_radius;
	GLSpace::calculateScreenPosition(x, y, GLSpace::v_near, centerX, centerY);
	GLSpace::calculateScreenLength(radius, GLSpace::v_near, world_radius);

	int i;
	int triangleAmount = 20; //# of triangles used to draw circle

	glPushMatrix();
	glLoadIdentity();

	GLSpace::generateColor(color);

	if (doFill){
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(centerX, centerY, centerZ); // center of circle
	}
	else {
		glBegin(GL_LINE_LOOP);
	}

	for (i = 0; i <= triangleAmount; i++) {
		glVertex3f(centerX + (world_radius * cos(i * twicePi / triangleAmount)),
			centerY + (world_radius * sin(i * twicePi / triangleAmount)), centerZ);
	}
	glEnd();
	glPopMatrix();
}

/* Draw in OpenGL where the finger has touched on screen (for touch-screen devices) */
void Finger::draw() {
	if (!(isfinite(x) && isfinite(y) && isfinite(size) && size > 0)) return;

	float brush_size = size * 3 / 4.0f;

	glLineWidth(brush_size / 5.0f);
	Circle circle1(x, y, brush_size, color);
	Circle circle2(x, y, (brush_size * 2) / 3.0f, color);
	circle1.draw();
	circle2.draw();
}

/* Draw a Cube in OpenGL */
void SimpleCube::draw(){
	if (!(isfinite(x) && isfinite(y) && isfinite(size))) return;

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


/* Draw a Box with Axis Arrows in OpenGL (currently set to auto-rotate) */
void WorldBox::draw() {
	static float angular_pos = 0;
	float centerX, centerY, centerZ = -(GLSpace::v_near + GLSpace::v_far) / 2.0f;
	float world_size;
	GLSpace::calculateScreenPosition(screen_x, screen_y, -centerZ, centerX, centerY);
	GLSpace::calculateScreenLength(size, -centerZ, world_size);

	glPushMatrix();
	glLoadIdentity();

	glTranslatef(centerX, centerY, centerZ);
	glScalef(world_size, world_size, world_size);

	/* TODO: Fix Code to work with Quaternions, currently auto-rotates...
	QVector4D quat = quaternion.toVector4D();
	float denom = sqrt(1.0 - (quat.w()*quat.w()));
	glRotatef(2*acos(quat.w()), quat.x() / denom, quat.y() / denom, quat.z() / denom);
	*/
	
	glRotatef(angular_pos, 1.0, 0.0, 0.0); //rotate 15 degress around x-axis
	glRotatef(2*angular_pos, 0.0, 1.0, 0.0); //rotate 15 degress around y-axis
	glRotatef(3*angular_pos, 0.0, 0.0, 1.0); //rotate 15 degress around z-axis
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


/* Initialize Model Data from the OBJ File Path given */
OBJModel::OBJModel(string fp) {
	stringstream str;
	str << "Reading File: " << fp;
	DebugWindow::println(str);

	// Open OBJ file
	ifstream inOBJ;
	inOBJ.open(fp);
	if (!inOBJ.good()) {
		str.clear();
		str << "FILE DOES NOT EXIST OR CANNOT BE OPENED" << fp;
		DebugWindow::println(str);

		isWorking = false;
		return;
	}

	// Initialize Data
	for (int i = 0; i < 3; i++){
		mVertices[i].clear();
		mFaces[i].clear();
		minBound[i] = 0;
		maxBound[i] = 0;
		mCenter[i] = 0;
		mTransform[i][X] = 0;
		mTransform[i][Y] = 0;
	}

	// Ignore Normals
	bool hasNormals = false;
	//vector<float> normalTemp[3];

	// Read OBJ file (Parse Data)
	float temp;
	while (!inOBJ.eof()) {
		// Read next line in file
		string line;
		getline(inOBJ, line);
		char *l = new char[line.size() + 1];
		memcpy(l, line.c_str(), line.size() + 1);

		// Parse the line
		strtok(l, " ");
		string type = line.substr(0, 2);
		if (type.compare("v ") == 0) {   // Vertex Data
			// Get x-y-z vertex data
			for (int i = 0; i < 3; i++){
				temp = atof(strtok(NULL, " /"));
				mVertices[i].push_back(temp);

				// Calculate bounding box of model
				if (minBound[i] > temp) minBound[i] = temp;
				if (maxBound[i] < temp) maxBound[i] = temp;
			}
		} else if (type.compare("vn") == 0) {    // Faces Data
			hasNormals = true;
			/* Get three vertex indices of the face
			for (int i = 0; i < 3; i++){
				normalTemp[i].push_back(atof(strtok(NULL, " ")));
			}*/
		} else if (type.compare("f ") == 0) {    // Faces Data
			// Get three vertex indices of the face
			for (int i = 0; i < 3; i++) {
				mFaces[i].push_back(atof(strtok(NULL, " /")));
				if (hasNormals) strtok(NULL, " ");
			}
		}

		delete[] l; // Clean up
	}
	inOBJ.close();

	if (mVertices[0].empty() || mFaces[0].empty()){
		isWorking = false;
		return;
	}

	bool has_normals = true;
	for (int i = 0; i < 3; i++) {
		// Compute Center of the Model and Proper Model Distance
		mCenter[i] = (minBound[i] + maxBound[i]) / 2;

		// Test if vertex normal data were properly recorded
		if (has_normals){
			has_normals = mNormals[i].size() == mVertices[i].size();
		}
	}
	model_distance = (GLSpace::MODEL_DEPTH_SCREEN_WIDTH_RATIO * ((maxBound[Z] - minBound[Z]) / 2));

	// Generate Normals if no normal data exists
	if (!has_normals) {
		str.clear();
		str << ". . . Generating Normals . . . " << fp;
		DebugWindow::println(str);
		generateNormals();
	}
	str.clear();
	str << "SUCCESSFUL PARSING" << fp;
	DebugWindow::println(str);
}

/* Check if the model loaded successfully */
bool OBJModel::exists(){ return isWorking; }

/* Generates the Vertex Normal Vectors of the Model */
void OBJModel::generateNormals() {
	float xValue = 0, yValue = 0, zValue = 0;
	float length;
	int vn[3];
	float vector1[3];
	float vector2[3];
	const long vCount = mVertices[0].size();
	long fCount = mFaces[0].size();
	vector<double> surfaceNormals[3];
	//double surfaceNormals[vCount][3]; //Cannot initialize arrays with variables

	// Clear Previous Normals
	mNormals[X].clear();
	mNormals[Y].clear();
	mNormals[Z].clear();
	surfaceNormals[X].assign(vCount, 0);
	surfaceNormals[Y].assign(vCount, 0);
	surfaceNormals[Z].assign(vCount, 0);

	// Accumulate Surface Normals
	for (long f = 0; f < fCount; f++) {
		//Gather the three surface (face) vertices
		for (int v = 0; v < 3; v++) {
			vn[v] = mFaces[v].at(f) - 1;
		}

		//Calculate Surface Vectors 1 and 2
		vector1[X] = mVertices[X].at(vn[0]) - mVertices[X].at(vn[1]);
		vector1[Y] = mVertices[Y].at(vn[0]) - mVertices[Y].at(vn[1]);
		vector1[Z] = mVertices[Z].at(vn[0]) - mVertices[Z].at(vn[1]);
		vector2[X] = mVertices[X].at(vn[2]) - mVertices[X].at(vn[1]);
		vector2[Y] = mVertices[Y].at(vn[2]) - mVertices[Y].at(vn[1]);
		vector2[Z] = mVertices[Z].at(vn[2]) - mVertices[Z].at(vn[1]);

		//Cross Product of Surface Vectors 1 and 2 (to generate Surface Normal)
		xValue = vector2[Y] * vector1[Z] - vector2[Z] * vector1[Y];
		yValue = vector2[Z] * vector1[X] - vector2[X] * vector1[Z];
		zValue = vector2[X] * vector1[Y] - vector2[Y] * vector1[X];

		// Sum it up for each vertex of the face
		for (int v = 0; v < 3; v++) {
			surfaceNormals[X][vn[v]] += xValue;
			surfaceNormals[Y][vn[v]] += yValue;
			surfaceNormals[Z][vn[v]] += zValue;
		}
	}

	//Normalize into the Vertex Normal Vectors
	for (long v = 0; v < vCount; v++) {
		xValue = surfaceNormals[X][v];
		yValue = surfaceNormals[Y][v];
		zValue = surfaceNormals[Z][v];

		// Normalizing
		length = sqrt(xValue*xValue + yValue*yValue + zValue*zValue);
		xValue /= length;
		yValue /= length;
		zValue /= length;

		// Final Vertex Normal stored (in parallel to mVertices)
		mNormals[X].push_back(xValue);
		mNormals[Y].push_back(yValue);
		mNormals[Z].push_back(zValue);
	}
}

/* Draw the OBJ Model in OpenGL (set to auto-rotate on screen)
  NOTE: mTransform arrays are used to help apply transformations to the model.
  currently not in use, as there is nothing that needs to modify the transformation
  outside of this method. Future implementations might have the user ransform the object */
void OBJModel::draw(){
	static float auto_rotate = 0;

	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);

	// Move Model to Front of Camera
	glTranslatef(-mCenter[X], -mCenter[Y], -mCenter[Z] - model_distance);

	// Apply recorded transformations
	glTranslatef(mTransform[TRANSLATE][X], -mTransform[TRANSLATE][Y], 0.0f);

	// Apply auto-rotation
	glRotatef(auto_rotate, 1.0, 0.0, 0.0);		//glRotatef(mTransform[ROTATE][Y], 1.0f, 0.0f, 0.0f);
	glRotatef(2 * auto_rotate, 0.0, 1.0, 0.0);	//glRotatef(mTransform[ROTATE][X], 0.0f, 1.0f, 0.0f);
	glRotatef(3 * auto_rotate, 0.0, 0.0, 1.0);	// [Previous code above not needed at this time]
	auto_rotate += 0.5;
	if (auto_rotate > 360) auto_rotate -= 360;
	glScalef(1 - mTransform[SCALE][Y], 1 - mTransform[SCALE][Y], 1 - mTransform[SCALE][Y]);

	// Begin creating model based on model data stored
	glBegin(GL_TRIANGLES);
	long v, fCount = mFaces[0].size();
	for (long f = 0; f < fCount; f++){
		for (int p = 0; p < 3; p++){
			v = mFaces[p].at(f) - 1;
			glNormal3f(mNormals[X].at(v)*(1 + mTransform[SCALE][Y]),
				mNormals[Y].at(v)*(1 + mTransform[SCALE][Y]),
				mNormals[Z].at(v)*(1 + mTransform[SCALE][Y]));
			glVertex3f(mVertices[X].at(v), mVertices[Y].at(v), mVertices[Z].at(v));
		}
	}
	glEnd();

	glPopMatrix();
}