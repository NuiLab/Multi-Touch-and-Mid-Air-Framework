#include "mainwindow.h"
#include "drawArea.h"

#include <QtGui>
#include <cstdlib>
#include "QFileDialog"
#include "QByteArray"
#include "qaction"
#include "qmessagebox"
#include "qmenu"
#include "qmenubar"
#include "debug.h"

/*Main function
Initializes the draw area object, menus and actions(buttons), 
sets name of the window and size
*/
MainWindow::MainWindow()
{
	drawArea = new DrawArea();
	setCentralWidget(drawArea);
	loadActions();
	loadMenus();

	setWindowTitle(tr("TamVisualizer"));
	resize(500, 500);

}

/*Receives the event when the resize button is clicked,
strips the id of the size selected and calls the doResize function
with the selected size as an argument*/
void MainWindow::resizeBrush()
{
	QAction *action = qobject_cast<QAction *>(sender());
	int size = action->data().toInt();
	drawArea->doResizeBrush(size);
}

/*Receives the event when the map button is clicked,
strips the id of the map selected and calls the doMap function
with the selected map as an argument*/
void MainWindow::map()
{
	QAction *action = qobject_cast<QAction *>(sender());
	int map = action->data().toInt();
	drawArea->doMap(map);
}

/*Receives the event when the save button is clicked,
sets up the path of the application as the main path
calls the doSave function with the current path as an argument*/
bool MainWindow::saveGesture()
{
	QString path = QDir::currentPath() + "/untitled." + "csv";

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
		path,
		tr("%1 Files (*.%2);; All Files(*)").arg("CSV").arg("csv"));
	if (fileName.isEmpty()) {
		return false;
	}
	else {
		return drawArea->doSaveGesture(fileName);
	}
}

/*Receives the event when the open button is clicked,
sets up the path of the application as the main path
calls the doOpen function with the current path as an argument*/
bool MainWindow::openGesture()
{
	QString path = QDir::currentPath();
	QString fileName = QFileDialog::getOpenFileName(this);
	if (fileName.isEmpty()) {
		return false;
	}
	else {
		return drawArea->doOpenGesture(fileName);
	}
}

/*Initializes all the actions(buttons)*/
void MainWindow::loadActions()
{
	for (int i = 5; i <= 100; i= i+5)
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
	connect(playbackAction, SIGNAL(triggered()), drawArea, SLOT(playback()));

	saveAction = new QAction(tr("&Save"), this);
	connect(saveAction, SIGNAL(triggered()), this, SLOT(saveGesture()));

	openAction = new QAction(tr("&Open"), this);
	connect(openAction, SIGNAL(triggered()), this, SLOT(openGesture()));

	clearScreenAction = new QAction(tr("&ClearScreen"), this);
	clearScreenAction->setShortcut(tr("Ctrl+L"));
	connect(clearScreenAction, SIGNAL(triggered()), drawArea, SLOT(clearScreen()));
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

	menuBar()->addAction(openAction);
	menuBar()->addAction(saveAction);
	menuBar()->addMenu(sizeMenu);
	menuBar()->addMenu(mapMenu);
	menuBar()->addAction(playbackAction);
	menuBar()->addAction(clearScreenAction);
	
}

