#include "glwindow.h"

/*	Simple Example: http://sorj.de/?p=192
	Modified to work with the current project.
	Simple test of OpenGL capabilities.
*/

/*Main constructor for the DrawArea object
Sets upt the windows touch event connection and
the click event connections also initializes a list
of ten colors*/
GLWindow::GLWindow(QWidget *parent) : QOpenGLWidget(parent) {
	setAttribute(Qt::WA_StaticContents);
	setAttribute(Qt::WA_AcceptTouchEvents);		// Allow Touch Screen Detection
	setAttribute(Qt::WA_MouseTracking);			// Allow Mouse Detection

	process = new ProcessorThread();

	timer.start(1000 / fps, Qt::TimerType::PreciseTimer, this);
}

GLWindow::~GLWindow(){
	delete process;
}

/*Changes the global brushSize variable to the selected size i*/
void GLWindow::doResizeBrush(int i)
{
	brushSize = i;
	//qDebug() << "BRUSH SIZE: " << i;
	DebugWindow::println("BRUSH SIZE: " + i);
}

/*Changes the global map variable to the selected map fuunction*/
void GLWindow::doMap(int map) {
	mapping = map;
	
	switch (map)
	{
	case 1:
		mapping = 1;
		//qDebug() << "MAPPING FUNCTION SELECTED IS 1";
		break;
	case 2:
		mapping = 2;
		//qDebug() << "MAPPING FUNCTION SELECTED IS 2";
		break;
	case 3:
		mapping = 0.5;
		//qDebug() << "MAPPING FUNCTION SELECTED IS 1/2";
		break;
	default:
		mapping = 1;
	}
	DebugWindow::println("MAPPING FUNCTION SELECTED IS " + tr(to_string(mapping).c_str()) + ".");
}

/*Set the display mode for the window*/
void GLWindow::setDisplay(DisplaySetting action) {
	display_type = action;
	/*DisplaySetting setting;
	switch (action){
	case 0: setting = DisplaySetting::NONE; break;
	case 1: setting = DisplaySetting::AVG; break;
	case 2: setting = DisplaySetting::MST; break;
	case 3: setting = DisplaySetting::CIRCLE; break;
	case 4: setting = DisplaySetting::SHP; break;
	case 5: setting = DisplaySetting::CUBE; break;
	}*/
	process->setProcess(action);
}





/*	Sets up the OpenGL rendering context, defines display lists, etc.
	Gets called once before the first time resizeGL() or paintGL() is called.
*/
void GLWindow::initializeGL(){
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);


	// Use QBasicTimer because its faster than QTimer
	//timer.start(12, this);
}

/*	Sets up the OpenGL viewport, projection, etc. Gets called whenever the widget has been resized
	(and also when it is shown for the first time because all newly created widgets get a resize event automatically).
*/
void GLWindow::resizeGL(int width, int height){
	glViewport(0, 0, (GLint)width, (GLint)height);

	/* create viewing cone with near and far clipping planes */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float screen_ratio = (float)height / (float)width;
	glFrustum(	-view_half_width, view_half_width, 
				-view_half_width * screen_ratio, view_half_width * screen_ratio,
				view_near, view_far);
	//glOrtho(-5, 5, -5.0f * screen_ratio, 5.0f * screen_ratio, 0.5, 30.0);

	//Initialize GLShape Frustum and Screen Size
	GLSpace::frustum(view_half_width, view_half_width * screen_ratio, view_near, view_far);
	GLSpace::screenSize(width, height);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/*
* Renders the OpenGL scene. Gets called whenever the widget needs to be updated.
*/
void GLWindow::paintGL(){
	isDrawing = true;

	QPainter painter(this);
	painter.beginNativePainting();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//delete color and depth buffer
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	//drawCubeGL(mouseX, mouseY, 1.0f);
	//glLineWidth(10);
	//drawLineGL(mouseX, mouseY, 20, 20, 100);
	//drawCircleGL(mouseX, mouseY, 0.01, 100, true);
	//drawCircleGL(20, 20, 0.01, 100, true);

	QList<TAMShape *> results = process->getResults();
	if (results.isEmpty() && playback_mode) {
		//qDebug() << "ERROR: Results are Empty" << endl;
		DebugWindow::println("ERROR: Results are Empty");
	}
	else {
		if(playback_mode)
			//qDebug() << "Drawing Results. # of shapes: " << results.size() << endl;
			DebugWindow::println("Drawing Results. # of shaptes: " + results.size());
		drawScreenGL(results);
	}


	painter.endNativePainting();

	//update();
	isDrawing = false;
}

/*
void GLWindow::drawLineGL(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, int color){
	float xA, yA, xB, yB;
	calculateScreenPosition(x1, y1, view_near, xA, yA);
	calculateScreenPosition(x2, y2, view_near, xB, yB);

	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_LINES);
	generateColor(color);
	glVertex3f(xA, yA, -view_near);	//x1 y1
	glVertex3f(xB, yB, -view_near);	//x2 y2
	glEnd();
	glPopMatrix();
}

void GLWindow::drawCircleGL(GLfloat x, GLfloat y, GLfloat radius, int color, bool doFill){
	float centerX, centerY, centerZ = -view_near;
	calculateScreenPosition(x, y, view_near, centerX, centerY);

	int i;
	int triangleAmount = 20; //# of triangles used to draw circle

	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * PI_L;

	glPushMatrix();
	glLoadIdentity();
	//glTranslatef(0.0f, 0.0f, -20.0f); //move along z-axis

	if (doFill){
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(centerX, centerY, centerZ); // center of circle
	}
	else {
		glBegin(GL_LINE_LOOP);
	}
	generateColor(color);
	for (i = 0; i <= triangleAmount; i++) {
		glVertex3f(centerX + (radius * cos(i * twicePi / triangleAmount)),
			centerY + (radius * sin(i * twicePi / triangleAmount)), centerZ);
	}
	glEnd();
	glPopMatrix();
}

void GLWindow::drawCubeGL(GLfloat x, GLfloat y, GLfloat size){
	float centerX, centerY, centerZ = -20.0f;
	calculateScreenPosition(x, y, -centerZ, centerX, centerY);

	glPushMatrix();
	glLoadIdentity();
	//glTranslatef(0.0f, 0.0f, -20.0f); //move along z-axis
	glTranslatef(centerX, centerY, centerZ);
	glScalef(size, size, size);
	glRotatef(30.0, 0.0, 1.0, 0.0); //rotate 30 degress around y-axis
	glRotatef(angular_pos, 1.0, 0.0, 0.0); //rotate 15 degress around x-axis

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

void GLWindow::calculateScreenPosition(GLfloat screen_x, GLfloat screen_y, GLfloat world_z, GLfloat &world_x, GLfloat &world_y){
	world_x = (world_z / view_near)*(2.0f*view_half_width*(screen_x / width()) - view_half_width);
	world_y = (world_z / view_near)*(2.0f*view_half_width*((height() - screen_y) / height()) - view_half_width)*(height() / (float)width());
}

void GLWindow::calculateScreenLength(GLfloat screen_length, GLfloat world_z, GLfloat &world_length){
	world_length = (world_z / view_near)*(2.0f*view_half_width*(screen_length / width()));
}

void GLWindow::generateColor(int id){
	float r, g, b;
	r = (((id + 1) * 07) % 19) / 20.0f;
	g = (((id + 3) * 11) % 19) / 20.0f;
	b = (((id + 5) * 13) % 19) / 20.0f;
	//qDebug() << "Color chosen: (" << r << ',' << g << ',' << b << ')' << endl;
	glColor3f(r, g, b);
}*/


void GLWindow::drawScreenGL(QList<TAMShape *> shapes){
	foreach(TAMShape* shape, shapes){
		shape->draw();
	}
}

void GLWindow::updateData(touch_data data){
	if (data.x < 0 || data.y < 0) {
		fingers.remove(data.id);
	}
	else {
		//angular_speed = 1.0;
		fingers[data.id] = touch_list.size();
	}
	touch_list << data;
}

/*
void GLWindow::drawScreenGL(){
	touch_data data;
	//glEnable(GL_DEPTH_TEST);

	foreach(int index, fingers){
		data = touch_list[index];
		if(display_type != 5) drawFingerGL(data);
		else drawCubeGL(data.x, data.y, brushSize / 100.0f);
	}

	switch (display_type) {
	case 1:	drawCircularConnectionGL(); break;
	case 2: drawFingerMappingGL();		break;
	case 3: drawAverageConnectionGL(); break;
	case 4: drawShortestMappingGL();	break;
	}
}

void GLWindow::drawFingerGL(const touch_data &data){
	float size = brushSize*0.02f*0.01f;

	generateColor(data.id);
	glLineWidth(brushSize / 10.0f);
	drawCircleGL(data.x, data.y, size, data.id, false);
	drawCircleGL(data.x, data.y, (size*2)/3.0f, data.id, false);
}

void GLWindow::drawCircularConnectionGL() {
	if (fingers.size() < 3) return;

	// Best Fit Circle for all points (work-in-progress)
	int avgX = 0, avgY = 0, count = 0;
	float centerX = 0, centerY = 0, radius = 5, avgR = 0;

	foreach(int i, fingers){
		foreach(int j, fingers){
			if (i <= j) continue;
			foreach(int k, fingers){
				if (j <= k) continue;

				if (getCircleEff(touch_list[i], touch_list[j], touch_list[k], centerX, centerY, radius)) {
					if (radius*radius > (width()*width() + height()*height())) continue;
					avgX += centerX;
					avgY += centerY;
					avgR += radius;
					count++;
				}
			}
		}
	}

	if (count == 0) return;

	avgX /= count;
	avgY /= count;
	avgR /= count;

	if (avgX < 0 || avgY < 0 || avgR > 100){
		qDebug() << "Large Data: " << avgX << ' ' << avgY << ' ' << avgR << endl;
	}

	//generateColor(100);
	glLineWidth(brushSize / 5.0f);
	float rad;
	calculateScreenLength(avgR,-0.5,rad);
	drawCircleGL(avgX, avgY, rad, 100, false);

	touch_data data;
	foreach(int index, fingers){
		data = touch_list[index];

		drawLineGL(data.x, data.y, avgX, avgY, data.id);
	}
}

void GLWindow::drawFingerMappingGL(){
	if (fingers.size() <= 1) return;

	// Finger Connecting Logic (M.S.T.)
	long distance, minI, minJ, minD;
	touch_data p1, p2;
	QList<int> removed, unselected;

	foreach(int index, fingers){
		unselected << index;
	}

	removed << unselected.takeFirst();	// Random vertex
	while (!unselected.isEmpty()) {
		minD = 10000000L;
		foreach(int i, removed){
			p1 = touch_list[i];

			foreach(int j, unselected) {
				p2 = touch_list[j];
				distance = (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y);

				if (distance < minD) {
					minI = i;
					minJ = j;
					minD = distance;
				}
			}
		}

		p1 = touch_list[minI];
		p2 = touch_list[minJ];

		drawLineGL(p1.x, p1.y, p2.x, p2.y, 50);

		unselected.removeOne(minJ);
		removed << minJ;
	}
}

void GLWindow::drawAverageConnectionGL(){
	if (fingers.size() < 2) return;

	// Best Fit Circle for all points (work-in-progress)
	int avgX = 0, avgY = 0, count = fingers.size();
	//float centerX = 0, centerY = 0, radius = 5, avgR = 0;

	foreach(int i, fingers){
		avgX += touch_list[i].x;
		avgY += touch_list[i].y;
	}
	avgX /= count;
	avgY /= count;

	touch_data data;
	foreach(int index, fingers){
		data = touch_list[index];
		drawLineGL(data.x, data.y, avgX, avgY, 60);
	}
}

void GLWindow::drawShortestMappingGL(){
	if (fingers.size() < 2) return;

	int count = fingers.size();
	touch_data node1, node2;
	QList< QList<int> > adj_matrix;
	QList<int> point_map;
	const int MAX = 1 << 30;
	int temp, m, n;

	m = 0;
	foreach(int i, fingers){
		node1 = touch_list[i];

		point_map.append(i);

		QList<int> add;
		adj_matrix.append(add);

		n = 0;
		foreach(int j, fingers){
			if (m == n) {
				adj_matrix[m].append(MAX);
			}
			else if (m > n) {
				adj_matrix[m].append(adj_matrix[n][m]);
			}
			else {
				node2 = touch_list[j];
				temp = (node1.x - node2.x)*(node1.x - node2.x) + (node1.y - node2.y)*(node1.y - node2.y);
				adj_matrix[m].append(temp);
			}
			n++;
		}
		m++;
	}

	int res;
	QList<int> order = getShortestHamiltonianPath(adj_matrix, res);

	touch_data prev = touch_list[point_map[order[0]]];
	touch_data curr;
	for (int i = 1; i < order.size(); i++) {
		curr = touch_list[point_map[order[i]]];
		drawLineGL(prev.x, prev.y, curr.x, curr.y, 70);
		prev = curr;
	}
}

*/

/*Opens a file stream to read the touchpoints,
reads the data one line at a time and then
reads the four elements contained on the line
while adding this touch pounts objects into the touchpoint list*/
bool GLWindow::doOpenGesture(QString fileName, QString fileType)
{
	/* EDIT: (05/26):
	Added choice of JSON file detection. Not fully implemented.
	*/

	//qDebug() << "OPENING A FILE";
	DebugWindow::println("OPENING A FILE");
	touch_list.clear();

	if (fileType.toLower() == "json") {
		QFile loadFile(fileName);

		if (!loadFile.open(QIODevice::ReadOnly)) {
			qWarning("Couldn't open save file.");
			return false;
		}

		QByteArray saveData = loadFile.readAll();

		QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

		QJsonObject json = loadDoc.object();

		touch_list.clear();
		QJsonArray pointsArray = json["POINTS"].toArray();
		for (int i = 0; i < pointsArray.size(); i++) {
			QJsonObject dataElem = pointsArray[i].toObject();
			touch_data data;
			data.x = dataElem["X"].toInt();
			data.y = dataElem["Y"].toInt();
			data.id = dataElem["ID"].toInt();
			data.time = dataElem["TM"].toInt();

			touch_list.append(data);
		}
	}
	else if (fileType.toLower() == "csv") {
		ifstream file;
		file.open(fileName.toStdString());
		string line;

		while (getline(file, line))
		{
			touch_data data;
			istringstream s(line);
			string field;
			getline(s, field, ',');
			data.x = stoi(field);
			getline(s, field, ',');
			data.y = stoi(field);
			getline(s, field, ',');
			data.id = stoi(field);
			getline(s, field, ',');
			data.time = stoi(field);
			touch_list.append(data);
		}
	}
	playback();
	return true;
}



/*Opens a file stream to save the touch points into
goes trough the list of touch points and writes them in order*/
bool GLWindow::doSaveGesture(QString fileName, QString fileType)
{
	/* EDIT: (05/26):
	Added choice of JSON file detection. Not fully implemented.
	*/

	//qDebug() << "SAVING THE FILE TO " << fileName;
	DebugWindow::println("SAVING THE FILE TO " + fileName);

	if (fileType.toLower() == "json") {
		/* EDITS: Links below
		http://doc.qt.io/qt-5/json.html
		http://doc.qt.io/qt-5/qtcore-json-savegame-example.html
		*/
		QFile saveFile(fileName);

		if (!saveFile.open(QIODevice::WriteOnly)) {
			//qDebug() << "Couldn't open save file.";
			DebugWindow::println("Couldn't open save file");
			return false;
		}

		QJsonObject json;
		QJsonArray pointArray;
		foreach(touch_data data, touch_list) {
			QJsonObject dataStruct;
			dataStruct["X"] = data.x;
			dataStruct["Y"] = data.y;
			dataStruct["ID"] = data.id;
			dataStruct["TM"] = data.time;
			pointArray.append(dataStruct);
		}
		json["POINTS"] = pointArray;

		QJsonDocument saveDoc(json);
		saveFile.write(saveDoc.toJson());
	}
	else if (fileType.toLower() == "csv") {
		ofstream file;
		file.open(fileName.toStdString());

		foreach(touch_data data, touch_list) {
			file << data.x << "," << data.y << "," << data.id << "," << data.time << endl;
		}
	}
	else {
		//qDebug() << "ERROR: File Type Invalid" << endl;
		DebugWindow::println("ERROR: File Type Invalid");
		return false;
	}
	clearScreen();
	return true;
}


/*Goes through the list of stored points and draws the results on the screen*/
void GLWindow::playback() {
	//qDebug() << "ON PLAYBACK"; 
	DebugWindow::println("ON PLAYBACK");
	playback_mode = true;

	int beginTime = clock();
	int time;
	int index = 0;
	foreach(touch_data data, touch_list) {

		time = clock();
		if (data.time - (time - beginTime) > 0) {
			//qDebug() << "  SLEEP FOR " << (data.time - (time - beginTime)) << endl;
			DebugWindow::println("SLEEP FOR" + (data.time - beginTime));


			// Pause between touch data 
			QThread::yieldCurrentThread();
			qApp->processEvents();

			QThread::msleep(data.time - (time - beginTime));
		}

		// Update
		if (data.x < 0 || data.y < 0)	fingers.remove(data.id);
		else {
			fingers[data.id] = index;
			//angular_speed = 1.0;
		}

		// Send data to proccess thread for calculation
		sendDataToProcessThread();

		index++;
	}

	//Refresh
	playback_mode = false;
	clearScreen();
}










/*
bool GLWindow::getCircleEff(touch_data data1, touch_data data2, touch_data data3, float &centerX, float &centerY, float &radius){
	long long temp1, temp2, temp3;
	float denom;
	temp1 = data2.y - data3.y;
	temp2 = data3.y - data1.y;
	temp3 = data1.y - data2.y;

	denom = 2 * (data1.x*temp1 + data2.x*temp2 + data3.x*temp3);

	if (denom == 0) return false;

	long long sqlen1, sqlen2, sqlen3;
	sqlen1 = data1.x*data1.x + data1.y*data1.y;
	sqlen2 = data2.x*data2.x + data2.y*data2.y;
	sqlen3 = data3.x*data3.x + data3.y*data3.y;

	centerX = (sqlen1*temp1 + sqlen2*temp2 + sqlen3*temp3) / denom;
	centerY = (sqlen1*(data3.x - data2.x) + sqlen2*(data1.x - data3.x) + sqlen3*(data2.x - data1.x)) / denom;

	temp1 = centerX - data1.x;
	temp2 = centerY - data1.y;
	radius = sqrt(temp1*temp1 + temp2*temp2);
	return true;
}

bool GLWindow::getCircle(touch_data data1, touch_data data2, touch_data data3, float &centerX, float &centerY, float &radius){
	float rslope, tslope;// , centerX, centerY;

	if (data1.x == data3.x || data2.x == data3.x) return false;
	rslope = (data1.y - data3.y) / (float)(data1.x - data3.x);
	tslope = (data2.y - data3.y) / (float)(data2.x - data3.x);
	if (rslope != 0 && rslope != tslope) {
		// X = ( r*t*(y2-y1) + r*(x3+x2) - t(x1+x3) ) / (2*(r-t))
		centerX = rslope*tslope*(data2.y - data1.y);
		centerX += rslope*(data3.x + data2.x);
		centerX -= tslope*(data3.x + data1.x);
		centerX /= 2 * (rslope - tslope);

		// y = (- 1/r) ( X - ((x1 + x3) / 2) ) + ((y1 + y3) / 2)
		centerY = ((data3.x + data1.x) / 2.0) - centerX;
		centerY /= rslope;
		centerY += (data3.y + data1.y) / 2.0;

		radius = sqrt((centerX - data1.x)*(centerX - data1.x) + (centerY - data1.y)*(centerY - data1.y));
		return true;
	}
	else return false;
}

QList<int> GLWindow::getShortestHamiltonianPath(QList< QList<int> > dist, int &res){
	// Algorithm retrieved and converted:
	// https://sites.google.com/site/indy256/algo/shortest_hamiltonian_path

	const int n = dist.size();
	const int MAX = 1 << 30;
	const int MAX_N = (1 << n);

	//int dp[1 << n][n];
	QList< QList<int> > dp;
	QList<int> order;

	for (int i = 0; i < MAX_N; i++){
		QList<int> add;
		dp.append(add);
		for (int j = 0; j < n; j++){
			dp[i].append(MAX);
		}
	}

	for (int i = 0; i < n; i++){
		order.append(-1);
		dp[(1 << i)][i] = 0;
	}

	int temp;
	for (int mask = 0; mask < 1 << n; mask++) {
		for (int i = 0; i < n; i++) {
			if ((mask & (1 << i)) != 0) {
				for (int j = 0; j < n; j++) {
					temp = dp[mask ^ (1 << i)][j] + dist[j][i];
					if ((mask & (1 << j)) != 0 && temp < dp[mask][i]) {
						dp[mask][i] = temp;//Math.min(dp[mask][i], dp[mask ^ (1 << i)][j] + dist[j][i]);
					}
				}
			}
		}
	}

	res = MAX;
	for (int i = 0; i < n; i++) {
		temp = dp[(1 << n) - 1][i];
		if (temp < res){
			res = temp;
		}
		//res = Math.min(res, dp[(1 << n) - 1][i]);
	}

	// reconstruct path
	int cur = (1 << n) - 1;
	//int order = new int[n];
	int last = -1;
	for (int i = n - 1; i >= 0; i--) {
		int bj = -1;
		for (int j = 0; j < n; j++) {
			if ((cur & 1 << j) != 0
				&& (bj == -1
				|| dp[cur][bj] + (last == -1 ? 0 : dist[bj][last]) > dp[cur][j] + (last == -1 ? 0 : dist[j][last]))) {
				bj = j;
			}
		}
		order[i] = bj;
		cur ^= (1 << bj);
		last = bj;
	}
	return order;
}
*/

/*Draws a clear immage the size of the draw area
effectively clearing the screen*/
void GLWindow::clearScreen()
{
	if (!playback_mode){
		//qDebug() << "CLEARING THE SCREEN";
		DebugWindow::println("CLEARING THE SCREEN");

		//image.fill(qRgb(235, 235, 235));
		init_time = 0;
		touch_list.clear();
		fingers.clear();

		//update();
	}
}



void GLWindow::sendDataToProcessThread(){
	QList<touch_data> list;
	foreach(int i, fingers){
		list << touch_list[i];
	}
	process->setFingers(list);
}



/*The function that does all the input/output,
this function gets triggered by a touch event,
saves the data of the touch event as a touch point
and adds the touch point to a list, then draws the
output depending on the mapping function*/
bool GLWindow::event(QEvent *event) {
	int time = clock();
	touch_data data;

	switch (event->type()) {
	case QEvent::MouseButtonPress:
	case QEvent::MouseMove: 
	case QEvent::MouseButtonRelease:
	{		
		QMouseEvent *mouse_data = static_cast<QMouseEvent *>(event);	
		mouseX = mouse_data->x();
		mouseY = mouse_data->y();

		touch_data mouse;
		mouse.x = mouseX;
		mouse.y = mouseY;
		mouse.time = time;
		mouse.id = -1;

		//updateData(mouse);
		sendDataToProcessThread();
		break;
	}
	case QEvent::TouchBegin:
	case QEvent::TouchUpdate:
	case QEvent::TouchEnd:
	{
		if (touch_list.isEmpty()){
			init_time = time;
		}

		QTouchEvent *touch = static_cast<QTouchEvent *>(event);
		QList<QTouchEvent::TouchPoint> touchPoints = static_cast<QTouchEvent *>(event)->touchPoints();
		foreach(const QTouchEvent::TouchPoint &touchPoint, touchPoints)
		{
			switch (touchPoint.state())
			{
			case Qt::TouchPointStationary:
				continue;
			case Qt::TouchPointReleased:
				data.x = -1;
				data.y = -1;
				data.id = touchPoint.id();
				data.time = time - init_time;//time(NULL);

				updateData(data);
				sendDataToProcessThread();
				break;
			case Qt::TouchPointPressed:
			case Qt::TouchPointMoved:
			default:
				data.x = touchPoint.pos().rx() * mapping;
				data.y = touchPoint.pos().ry() * mapping;
				data.id = touchPoint.id();
				data.time = time - init_time;//time(NULL);

				//EDIT: Update info created
				updateData(data);

				// EDIT: Encapsulate the drawing of touch events into one function
				//drawScreen();
				sendDataToProcessThread();
			
				break;
			}
		}
		break;
	}
	default:
		return QWidget::event(event);
	}
	return true;
}


void GLWindow::timerEvent(QTimerEvent *) {
	/*// Decrease angular speed (friction)
	angular_speed *= 0.98;

	// Stop rotation when speed goes below threshold
	if (angular_speed < 0.01) {
		angular_speed = 0.0;
	}
	else {
		angular_pos += 3*angular_speed;
		if (angular_pos > 360.0) angular_pos -= 360.0;
	}*/
	if(!isDrawing) update();
}