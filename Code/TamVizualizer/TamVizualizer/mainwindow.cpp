#include "mainwindow.h"

/*Main Function: Initializes the GLWindow object,
  Menus and Actions(buttons), sets name of the window and its size */
MainWindow::MainWindow() {
	// Initialize Debug Window
	debug = new DebugWindow();
	debug->show();

	// Initialize GLWindow
	window = new GLWindow();
	setCentralWidget(window);

	// Load Menus and Actions
	loadActions();
	loadMenus();

	// Set-up Main Window
	setWindowTitle(tr("TamVisualizer"));
	resize(500, 500);

	// Initialize File Types the program will use
	type_list << tr("JSON") << tr("CSV");

	type_option = type_list[0] + " Files(*." + type_list[0].toLower() + ")";
	for (int i = 1; i < type_list.size(); i++) {
		type_option += ";;" + type_list[i] + " Files(*." + type_list[i].toLower() + ")";
	}
}

/* Closes the DebugWindow when the MainWindow is exiting */
void MainWindow::closeEvent(QCloseEvent *event) {
	debug->close();
	event->accept();
}

/*Receives the event when the resize button is clicked,
  Strips the id of the size selected and calls the doResize
  function with the selected size as an argument*/
void MainWindow::resizeBrush() {
	QAction *action = qobject_cast<QAction *>(sender());
	int size = action->data().toInt();
	window->doResizeBrush(size);
}

/*Receives the event when the map button is clicked:
  Strips the id of the map selected and calls the doMap
  function with the selected map as an argument*/
void MainWindow::map() {
	QAction *action = qobject_cast<QAction *>(sender());
	int map = action->data().toInt();
	window->doMap(map);
}

/*Receives the event when the save button is clicked:
  Sets up the path of the application as the main path
  calls the doSave function with the current path as an argument*/
bool MainWindow::saveGesture() {
	playbackAction->setEnabled(false);
	openAction->setEnabled(false);
	saveAction->setEnabled(false);
	sizeMenu->setEnabled(false);
	mapMenu->setEnabled(false);
	displayMenu->setEnabled(false);
	clearScreenAction->setEnabled(false);
	objAction->setEnabled(false);

	bool success = false;
	QString fileName;
	QString selected;
	
	fileName = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::currentPath(), type_option, &selected);
	
	qDebug() << fileName << "   " << selected << endl;
	stringstream str;
	str << fileName.toStdString() << "   " << selected.toStdString();
	DebugWindow::println(str);

	if (fileName.isEmpty() || selected.isEmpty()){
		qDebug() << "Save Cancelled" << endl;
		stringstream str;
		str << "Save Cancelled";
		DebugWindow::println(str);
	} else {
		foreach(QString type, type_list) {
			qDebug() << selected.left(type.size()) << " TEST" << endl;
			stringstream str;
			str << selected.left(type.size()).toStdString() << " TEST";
			DebugWindow::println(str);

			if (selected.left(type.size()) == type) {
				success = window->doSaveGesture(fileName, type);
				break;
			}
		}
	}

	playbackAction->setEnabled(true);
	openAction->setEnabled(true);
	saveAction->setEnabled(true);
	sizeMenu->setEnabled(true);
	mapMenu->setEnabled(true);
	displayMenu->setEnabled(true);
	clearScreenAction->setEnabled(true);
	objAction->setEnabled(true);

	return success;
}

/*Receives the event when the open button is clicked:
  Sets up the path of the application as the main path
  calls the doOpen function with the current path as an argument*/
bool MainWindow::openGesture() {
	playbackAction->setEnabled(false);
	openAction->setEnabled(false);
	saveAction->setEnabled(false);
	sizeMenu->setEnabled(false);
	mapMenu->setEnabled(false);
	displayMenu->setEnabled(false);
	clearScreenAction->setEnabled(false);
	objAction->setEnabled(false);

	bool success = false;
	QString fileName = QFileDialog::getOpenFileName(this);

	if (!fileName.isEmpty()) {
		QFileInfo f(fileName);
		QString file_type = f.suffix().toLower();

		qDebug() << file_type << endl;
		stringstream str;
		str << file_type.toStdString();
		DebugWindow::println(str);
		foreach(QString type, type_list) {
			if (file_type == type.toLower()) {
				success = window->doOpenGesture(fileName, file_type);
				break;
			}
		}
	}

	openAction->setEnabled(true);
	saveAction->setEnabled(true);
	sizeMenu->setEnabled(true);
	mapMenu->setEnabled(true);
	displayMenu->setEnabled(true);
	playbackAction->setEnabled(true);
	clearScreenAction->setEnabled(true);
	objAction->setEnabled(true);

	return success;
}

/*Receives the event when the load OBJ button is clicked:
  Opens a prompt to select the path of the OBJ file to read, then
  calls the doLoadOBJFile function with the current path as an argument*/
bool MainWindow::loadOBJFile() {
	playbackAction->setEnabled(false);
	openAction->setEnabled(false);
	saveAction->setEnabled(false);
	sizeMenu->setEnabled(false);
	mapMenu->setEnabled(false);
	displayMenu->setEnabled(false);
	clearScreenAction->setEnabled(false);
	objAction->setEnabled(false);

	bool success = false;
	QString fileName = QFileDialog::getOpenFileName(this);
	if (!fileName.isEmpty()) {
		QFileInfo f(fileName);
		QString file_type = f.suffix().toLower();

		qDebug() << file_type << endl;
		stringstream str;
		str << file_type.toStdString();
		DebugWindow::println(str);

		if (file_type == "obj")
			success = window->doLoadOBJFile(fileName.toStdString());
	}

	openAction->setEnabled(true);
	saveAction->setEnabled(true);
	sizeMenu->setEnabled(true);
	mapMenu->setEnabled(true);
	displayMenu->setEnabled(true);
	playbackAction->setEnabled(true);
	clearScreenAction->setEnabled(true);
	objAction->setEnabled(true);

	return success;
}

void MainWindow::doPlayback(){
	playbackAction->setEnabled(false);
	openAction->setEnabled(false);
	saveAction->setEnabled(false);
	sizeMenu->setEnabled(false);
	mapMenu->setEnabled(false);
	displayMenu->setEnabled(false);
	objAction->setEnabled(false);

	window->playback();

	openAction->setEnabled(true);
	saveAction->setEnabled(true);
	sizeMenu->setEnabled(true);
	mapMenu->setEnabled(true);
	displayMenu->setEnabled(true);
	playbackAction->setEnabled(true);
	objAction->setEnabled(true);
}


/*Changes the type of display*/
void MainWindow::changeDisplay() {
	QAction *action = qobject_cast<QAction *>(sender());
	int type = action->data().toInt();
	window->getProcessorThread()->setProcess(toDS[type]);
}


/*Initializes all the actions(buttons)*/
void MainWindow::loadActions() {
	for (int i = 20; i <= 100; i= i+5) {
		string sizes = to_string(i);
		char const *pchar = sizes.c_str();
		QAction *action = new QAction(tr(pchar), this);
		action->setData(i);
		connect(action, SIGNAL(triggered()), this, SLOT(resizeBrush()));
		sizeActions.append(action);
	}

	for (int i = 1; i <= 3; ++i) {
		string sizes = to_string(i);
		char const *pchar = sizes.c_str();
		QAction *action = new QAction(tr("Map %1").arg(pchar) , this);
		action->setData(i);
		connect(action, SIGNAL(triggered()), this, SLOT(map()));
		mapActions.append(action);
	}

	playbackAction = new QAction(tr("&Playback"), this);
	connect(playbackAction, SIGNAL(triggered()), this, SLOT(doPlayback()));

	saveAction = new QAction(tr("&Save"), this);
	connect(saveAction, SIGNAL(triggered()), this, SLOT(saveGesture()));

	openAction = new QAction(tr("&Open"), this);
	connect(openAction, SIGNAL(triggered()), this, SLOT(openGesture()));

	clearScreenAction = new QAction(tr("&Clear Screen"), this);
	clearScreenAction->setShortcut(tr("Ctrl+L"));
	connect(clearScreenAction, SIGNAL(triggered()), window, SLOT(clearScreen()));

	/*------------ PLACE TO ADD NEW DISPLAY SETTINGS ------------*/
	int value = 0;

	QAction *disp_action = new QAction(tr("No Display (Fingers Only)"), this);
	disp_action->setData(value++);
	connect(disp_action, SIGNAL(triggered()), this, SLOT(changeDisplay()));
	displayActions.append(disp_action);

	disp_action = new QAction(tr("Finger Connection (Averaging)"), this);
	disp_action->setData(value++);
	connect(disp_action, SIGNAL(triggered()), this, SLOT(changeDisplay()));
	displayActions.append(disp_action);

	disp_action = new QAction(tr("Finger Mapping (M.S.T.)"), this);
	disp_action->setData(value++);
	connect(disp_action, SIGNAL(triggered()), this, SLOT(changeDisplay()));
	displayActions.append(disp_action);

	disp_action = new QAction(tr("Circular Connection (Est.)"), this);
	disp_action->setData(value++);
	connect(disp_action, SIGNAL(triggered()), this, SLOT(changeDisplay()));
	displayActions.append(disp_action);

	disp_action = new QAction(tr("Shortest Mapping (S.H.P.)"), this);
	disp_action->setData(value++);
	connect(disp_action, SIGNAL(triggered()), this, SLOT(changeDisplay()));
	displayActions.append(disp_action);

	disp_action = new QAction(tr("OpenGL Cube Display (Finger Only)"), this);
	disp_action->setData(value++);
	connect(disp_action, SIGNAL(triggered()), this, SLOT(changeDisplay()));
	displayActions.append(disp_action);
	/*---------------------------- END ---------------------------*/

	/* For the purpose of loading  in your own OBJ models*/
	objAction = new QAction(tr("&Load OBJ"), this);
	connect(objAction, SIGNAL(triggered()), this, SLOT(loadOBJFile()));
}

/*initializes the Menus*/
void MainWindow::loadMenus() {
	sizeMenu = new QMenu(tr("&Brush Size"), this);
	foreach(QAction *action, sizeActions)
		sizeMenu->addAction(action);

	mapMenu = new QMenu(tr("&Map"), this);
	foreach(QAction *action, mapActions)
		mapMenu->addAction(action);

	displayMenu = new QMenu(tr("&Display"), this);
	foreach(QAction *action, displayActions)
		displayMenu->addAction(action);

	menuBar()->addAction(openAction);
	menuBar()->addAction(saveAction);
	menuBar()->addMenu(sizeMenu);
	//menuBar()->addMenu(mapMenu);
	menuBar()->addMenu(displayMenu);
	menuBar()->addAction(playbackAction);
	menuBar()->addAction(clearScreenAction);
	//menuBar()->addAction(objAction);			/* For the purpose of loading  in your own OBJ models*/
}
