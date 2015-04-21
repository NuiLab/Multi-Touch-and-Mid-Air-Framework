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


MainWindow::MainWindow()
{
	drawArea = new DrawArea();
	setCentralWidget(drawArea);
	loadActions();
	loadMenus();

	setWindowTitle(tr("TamVisualizer"));
	resize(500, 500);

}


void MainWindow::resizeBrush()
{
	QAction *action = qobject_cast<QAction *>(sender());
	int size = action->data().toInt();
	drawArea->doResizeBrush(size);
}
void MainWindow::map()
{
	QAction *action = qobject_cast<QAction *>(sender());
	int map = action->data().toInt();
	drawArea->doMap(map);
}
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

void MainWindow::loadMenus()
{
	sizeMenu = new QMenu(tr("&BrushSize"), this);
	foreach(QAction *action, sizeActions)
		sizeMenu->addAction(action);

	mapMenu = new QMenu(tr("&Map"), this);
	foreach(QAction *action, mapActions)
		mapMenu->addAction(action);

	menuBar()->addMenu(sizeMenu);
	menuBar()->addMenu(mapMenu);
	menuBar()->addAction(playbackAction);
	menuBar()->addAction(saveAction);
	menuBar()->addAction(clearScreenAction);
	menuBar()->addAction(openAction);
}

