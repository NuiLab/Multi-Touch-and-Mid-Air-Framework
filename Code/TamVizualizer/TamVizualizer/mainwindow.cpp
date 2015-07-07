#include "mainwindow.h"

/*Main function
Initializes the draw area object, menus and actions(buttons), 
sets name of the window and size
*/
MainWindow::MainWindow() {
	window = new GLWindow();

	setCentralWidget(window);
	loadActions();
	loadMenus();

	setWindowTitle(tr("TamVisualizer"));
	resize(500, 500);

	type_list << tr("JSON") << tr("CSV");

	type_option = type_list[0] + " Files(*." + type_list[0].toLower() + ")";
	for (int i = 1; i < type_list.size(); i++) {
		type_option += ";;" + type_list[i] + " Files(*." + type_list[i].toLower() + ")";
	}

	debug = new DebugWindow();
	debug->show();
}

void MainWindow::closeEvent(QCloseEvent *event) {
	debug->close();
	event->accept();
}

/*Receives the event when the resize button is clicked,
strips the id of the size selected and calls the doResize function
with the selected size as an argument*/
void MainWindow::resizeBrush()
{
	QAction *action = qobject_cast<QAction *>(sender());
	int size = action->data().toInt();
	window->doResizeBrush(size);
}

/*Receives the event when the map button is clicked,
strips the id of the map selected and calls the doMap function
with the selected map as an argument*/
void MainWindow::map()
{
	QAction *action = qobject_cast<QAction *>(sender());
	int map = action->data().toInt();
	window->doMap(map);
}

/*Receives the event when the save button is clicked,
sets up the path of the application as the main path
calls the doSave function with the current path as an argument*/
bool MainWindow::saveGesture()
{
	/* EDIT: (05/26):
	Added the choice between file types with some abstraction.
	*/

	//QString path = QDir::currentPath();

	QString fileName;
	QString selected;
	
	fileName = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::currentPath(), type_option, &selected);
	//tr("%1 Files (*.%2);; %3 Files(*. %4);; All Files(*)").arg(JSON_STR).arg(JSON_STR).arg("CSV").arg("csv")
	
	//qDebug() << fileName << "   " << selected << endl;
	DebugWindow::println(fileName + "   " + selected);

	if (fileName.isEmpty() || selected.isEmpty()){
		DebugWindow::println("Save Cancelled");
		//qDebug() << "Save Cancelled" << endl;
		return false;
	}

	foreach (QString type, type_list) {
		//QString check = tr("%1 Files (*.%2)").arg(type).arg(type.toLower());

		DebugWindow::println(selected.left(type.size()) + " TEST");
		//qDebug() << selected.left(type.size()) << " TEST" << endl;
		if (selected.left(type.size()) == type) {
			return window->doSaveGesture(fileName, type);
		}
	}
	return false;
}

/*Receives the event when the open button is clicked,
sets up the path of the application as the main path
calls the doOpen function with the current path as an argument*/
bool MainWindow::openGesture()
{
	/* EDIT: (05/26):
	Added the ability to open different file type.
	*/

	QString fileName = QFileDialog::getOpenFileName(this);
	if (fileName.isEmpty()) {
		return false;
	} else {
		QFileInfo f(fileName);
		QString file_type = f.suffix().toLower();

		//qDebug() << file_type << endl;
		DebugWindow::println(file_type);
		foreach(QString type, type_list) {
			if (file_type == type.toLower()) {
				return window->doOpenGesture(fileName, file_type);
			}
		}
		return false;
	}
}

/*Changes the type of display*/
void MainWindow::changeDisplay()
{
	QAction *action = qobject_cast<QAction *>(sender());
	int type = action->data().toInt();
	window->setDisplay(toDS[type]);
}


/*Initializes all the actions(buttons)*/
void MainWindow::loadActions()
{
	for (int i = 20; i <= 100; i= i+5)
	{
		string sizes = to_string(i);
		char const *pchar = sizes.c_str();
		QAction *action = new QAction(tr(pchar), this);
		action->setData(i);
		connect(action, SIGNAL(triggered()), this, SLOT(resizeBrush()));
		sizeActions.append(action);
	}

	for (int i = 1; i <= 3; ++i)
	{
		string sizes = to_string(i);
		char const *pchar = sizes.c_str();
		QAction *action = new QAction(tr("Map %1").arg(pchar) , this);
		action->setData(i);
		connect(action, SIGNAL(triggered()), this, SLOT(map()));
		mapActions.append(action);
	}

	playbackAction = new QAction(tr("&PlayBack"), this);
	connect(playbackAction, SIGNAL(triggered()), window, SLOT(playback()));

	saveAction = new QAction(tr("&Save"), this);
	connect(saveAction, SIGNAL(triggered()), this, SLOT(saveGesture()));

	openAction = new QAction(tr("&Open"), this);
	connect(openAction, SIGNAL(triggered()), this, SLOT(openGesture()));

	clearScreenAction = new QAction(tr("&ClearScreen"), this);
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

}

/*initializes the menus*/
void MainWindow::loadMenus()
{
	sizeMenu = new QMenu(tr("&BrushSize"), this);
	foreach(QAction *action, sizeActions)
		sizeMenu->addAction(action);

	mapMenu = new QMenu(tr("&Map"), this);
	foreach(QAction *action, mapActions)
		mapMenu->addAction(action);

	//NEW!

	displayMenu = new QMenu(tr("&Display"), this);
	foreach(QAction *action, displayActions)
		displayMenu->addAction(action);

	menuBar()->addAction(openAction);
	menuBar()->addAction(saveAction);
	menuBar()->addMenu(sizeMenu);
	menuBar()->addMenu(mapMenu);
	menuBar()->addMenu(displayMenu);
	menuBar()->addAction(playbackAction);
	menuBar()->addAction(clearScreenAction);

}

