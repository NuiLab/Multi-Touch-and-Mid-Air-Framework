#include "glwindow.h"

/*	Simple OpenGL Example: http://sorj.de/?p=192	(Modified to work with the project's goal) */

/* Main constructor for the GLWindow object.
   Sets up the windows touch event connection, the click event
   connections, the processor thread, and the display timer*/
GLWindow::GLWindow(QWidget *parent) : QOpenGLWidget(parent) {
	setAttribute(Qt::WA_StaticContents);
	setAttribute(Qt::WA_AcceptTouchEvents);		// Allow Touch Screen Detection
	setAttribute(Qt::WA_MouseTracking);			// Allow Mouse Detection

	process = new ProcessorThread();
}

/* To be certain that the thread closes properly */
GLWindow::~GLWindow() {
	delete process;
	playback_mode = false;
}

/*Changes the global brushSize variable to the selected size i*/
void GLWindow::doResizeBrush(int size) {
	process->setBrushSize(size);
	qDebug() << "BRUSH SIZE: " << size;
	stringstream str;
	str << "BRUSH SIZE" << size;
	DebugWindow::println(str);
}

/*Changes the global map variable to the selected map fuunction*/
void GLWindow::doMap(int map) {
	mapping = map;
	
	switch (map) {
	case 1: mapping = 1;	break;
	case 2: mapping = 2;	break;
	case 3: mapping = 0.5;	break;
	default:mapping = 1;
	}

	qDebug() << "MAPPING FUNCTION SELECTED IS " << mapping << '.' << endl;
	stringstream str;
	str << "MAPPING FUNCTION SELECTED IS" << mapping;
	DebugWindow::println(str);
}

/* Set the display mode for the window*/
void GLWindow::setDisplay(void(*function)(DisplaySetting, ProcessorThread*), DisplaySetting action, ProcessorThread* proc) {
	function(action, proc);
}

/* Returns pointer to ProcessorThread */
ProcessorThread* GLWindow::getProcessorThread() {
	return process;
}











/*	Sets up the OpenGL rendering context, defines display lists, etc.
	Gets called once before the first time resizeGL() or paintGL() is called.*/
void GLWindow::initializeGL() {
	TAM::VisualizerHandler<TAMShape>::initGL(NULL, 1.0, 1.0, 1.0, 1.0f);
	timer.start(1000 / fps, Qt::TimerType::PreciseTimer, this);
}

/*	Sets up the OpenGL viewport, projection, etc. Gets called whenever the widget has been resized
	(and also when it is shown for the first time because all newly created widgets get a resize event automatically). */
void GLWindow::resizeGL(int width, int height){
	float screen_ratio = TAM::VisualizerHandler<TAMShape>::resizeGL(NULL, width, height);
	//Initialize GLShape Frustum and Screen Size
	GLSpace::frustum(view_half_width, view_half_width * screen_ratio, view_near, view_far);
	GLSpace::screenSize(width, height);
}

/* Renders the OpenGL scene. Gets called whenever the widget needs to be updated.*/
void GLWindow::paintGL() {	

	isDrawing = true;	// Let others know that you are drawing

	// Retrieve results from processor thread
	std::list<TAMShape *> results = process->getResults();

	// Print results if in playback mode (For testing purposes)
	if (playback_mode){
		qDebug() << "Drawing Results. # of shapes: " << results.size() << endl;
		stringstream str;
		str << "Drawing Results. # of shapes: " << results.size();
		DebugWindow::println(str);
	}

	/* This is just for testing */
	/*foreach(ShapeCoordinates x, vis->coord.getGLCoordinates(GLSpace::screen_width, GLSpace::screen_height)) {
		cout << "Hello coordinates\n";
		cout << "X: " << x.getXCoordinate() << endl;
		cout << "Y: " << x.getYCoordinate() << endl;
	}*/

	TAM::VisualizerHandler<TAMShape>::paintGL(&GLWindow::drawScreenGL, results);

	isDrawing = false;	// Let others know you stopped drawing
}

/* Draw the list of TAMShapes to the screen*/
void GLWindow::drawScreenGL(std::list<TAMShape *> shapes){
	foreach(TAMShape* shape, shapes){
		shape->draw();
	}
}










/* Opens a file to read the touchpoint data, then replays 
   the results. It reads the data differently depending on
   if it's a JSON file or a CSV file. */
bool GLWindow::doOpenGesture(QString fileName, QString fileType) {
	qDebug() << "OPENING A FILE";
	stringstream str;
	str << "OPENING A FILE";
	DebugWindow::println(str);
	touch_list.clear();

	// JSON FILE DETECTED
	if (fileType.toLower() == "json") {
		QFile loadFile(fileName);

		if (!loadFile.open(QIODevice::ReadOnly)) {
			qWarning("Couldn't open file.");
			qDebug() << "Couldn't open file" << endl;
			stringstream str;
			str << "Couldn't open file";
			DebugWindow::println(str);
			return false;
		}

		QByteArray saveData = loadFile.readAll();
		QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
		QJsonObject json = loadDoc.object();

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

	// CSV FILE DETECTED
	} else if (fileType.toLower() == "csv") {
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

	// Playback the recorded results from the opened file
	playback();
	return true;
}

/* Saves the recorded touch data to either a JSON or CSV file.
   Goes through the list of touch points and writes them in order */
bool GLWindow::doSaveGesture(QString fileName, QString fileType) {
	/* EDITS: Links below used to create JSON File
		http://doc.qt.io/qt-5/json.html
		http://doc.qt.io/qt-5/qtcore-json-savegame-example.html
	*/

	qDebug() << "SAVING THE FILE TO " << fileName;
	stringstream str;
	str << "SAVING FILE TO " << fileName.toStdString();
	DebugWindow::println(str);

	// SAVE AS JSON FILE
	if (fileType.toLower() == "json") {
		QFile saveFile(fileName);

		if (!saveFile.open(QIODevice::WriteOnly)) {
			qDebug() << "Couldn't open save file.";
			stringstream str;
			str << "Couldn't open save file";
			DebugWindow::println(str);
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


	// SAVE AS CSV FILE
	} else if (fileType.toLower() == "csv") {
		ofstream file;
		file.open(fileName.toStdString());

		foreach(touch_data data, touch_list) {
			file << data.x << "," << data.y << "," << data.id << "," << data.time << endl;
		}
	} else {
		qDebug() << "ERROR: File Type Invalid" << endl;
		stringstream str;
		str << "ERROR: File Type Invalid";
		DebugWindow::println(str);
		return false;
	}

	// Clear the saved results
	clearScreen();
	return true;
}

/* Opens an OBJ file to be loaded into the processor thread for drawing */
bool GLWindow::doLoadOBJFile(string path) {
	qDebug() << "LOADING OBJ FILE";
	stringstream str;
	str << "LOADING OBJ FILE";
	DebugWindow::println(str);

	return process->addOBJModel(path);
}










/* Goes through the list of stored points and plays back the results on the screen */
void GLWindow::playback() {
	qDebug() << "ON PLAYBACK";
	stringstream str;
	str << "ON PLAYBACK";
	DebugWindow::println(str);

	playback_mode = true;	//Lets others know that the program's in playback mode

	int beginTime = clock();
	int time;
	int index = 0;
	foreach(touch_data data, touch_list) {
		time = clock();
		if (data.time - (time - beginTime) > 0) {
			qDebug() << "  SLEEP FOR " << (data.time - (time - beginTime)) << endl;
			stringstream str;
			str << "  SLEEP FOR " << (data.time - (time - beginTime));
			DebugWindow::println(str);

			// Sleep for the duration until the next touch-event happened
			QThread::yieldCurrentThread();
			qApp->processEvents();
			QThread::msleep(data.time - (time - beginTime));
		}

		// Update Data (without updating the touch_list being read)
		if (data.x < 0 || data.y < 0) {
			fingers.remove(data.id);
		} else {
			fingers[data.id] = index;
		}

		// Send data to proccess thread for calculation
		sendDataToProcessThread();

		// Update current frame
		update();
		index++;

		// Stop when playback mode closes
		if (!playback_mode) break;
	}

	// Let others know you stopped playback mode
	playback_mode = false;
	clearScreen();
}

/* Clear the recorded data*/
void GLWindow::clearScreen() {
	if (playback_mode){
		playback_mode = false;
		return;
	}

	qDebug() << "CLEARING THE SCREEN";
	stringstream str;
	str << "CLEARING THE SCREEN" << endl;
	DebugWindow::println(str);

	init_time = 0;
	fingers.clear();
	touch_list.clear();
	sendDataToProcessThread();
}

/* Update the touch data for multiple fingers*/
void GLWindow::updateData(touch_data data){
	if (data.x < 0 || data.y < 0) {
		fingers.remove(data.id);
	} else {
		fingers[data.id] = touch_list.size();
	}
	touch_list << data;
}

/* Send finger data (touch screen data) to the ProcessorThread*/
void GLWindow::sendDataToProcessThread() {
	QList<touch_data> list;
	foreach(int i, fingers) {
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
	static bool mousePressed = false;

	if (playback_mode)
		return QWidget::event(event);

	int time = clock();
	touch_data data;

	switch (event->type()) {
	/*
	case QEvent::MouseButtonPress:
		mousePressed = true;
	case QEvent::MouseMove:
	{		
		if (!mousePressed) break;

		// Gather mouse position data
		QMouseEvent *mouse_data = static_cast<QMouseEvent *>(event);	
		mouseX = mouse_data->x();
		mouseY = mouse_data->y();

		// Send mouse data as finger-data
		touch_data mouse;
		mouse.x = mouseX;
		mouse.y = mouseY;
		mouse.time = time;
		mouse.id = -1;
		updateData(mouse);

		// Update the process thread
		sendDataToProcessThread();
		break;
	}
	case QEvent::MouseButtonRelease:
	{
		// Send mouse data to trigger deletion of the released mouse
		touch_data mouse;
		mouse.x = -1;	// Triggers
		mouse.y = -1;	// Triggers
		mouse.time = time;
		mouse.id = -1;
		updateData(mouse);

		// Update the process thread
		sendDataToProcessThread();

		mousePressed = false;
		break;
	}*/
	case QEvent::TouchBegin:
	case QEvent::TouchUpdate:
	case QEvent::TouchEnd:
	{
		// Initialize beginning time if not initialized
		if (touch_list.isEmpty()) {
			init_time = time;
		}

		QTouchEvent *touch = static_cast<QTouchEvent *>(event);
		QList<QTouchEvent::TouchPoint> touchPoints = static_cast<QTouchEvent *>(event)->touchPoints();
		foreach(const QTouchEvent::TouchPoint &touchPoint, touchPoints) {
			switch (touchPoint.state()) {
			case Qt::TouchPointStationary: continue;
			case Qt::TouchPointReleased:
				// Send Finger data to trigger deletion of the released finger
				data.x = -1;	// Triggers
				data.y = -1;	// Triggers
				data.id = touchPoint.id();
				data.time = time - init_time;

				// Update Input Data
				updateData(data);
				sendDataToProcessThread();
				break;
			case Qt::TouchPointPressed:
			case Qt::TouchPointMoved:
			default:
				// Gather touch-screen input data
				data.x = touchPoint.pos().rx() * mapping;
				data.y = touchPoint.pos().ry() * mapping;
				data.id = touchPoint.id();
				data.time = time - init_time;//time(NULL);

				// Update Input Data
				updateData(data);
				sendDataToProcessThread();
			
				break;
			}
		}
		break;
	}
	default: // Default widget event otherwise
		return QWidget::event(event);
	}
	return true;
}

/* Timer for setting a frame-per-second drawing rate */
void GLWindow::timerEvent(QTimerEvent *) {
	// If you're no longer drawing, draw next frame
	// If you're in playback mode, let the playback call when to draw next frame

	if (!isDrawing && !playback_mode){
		update();
	}
}