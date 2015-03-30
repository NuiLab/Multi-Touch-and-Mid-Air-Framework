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


MainWindow::MainWindow(DrawArea::touchData data)
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
	qDebug() << "Here";
	qDebug() << size;
	doResizeBrush(size);
}
void MainWindow::recognizer()
{
	//TODO
}
void MainWindow::saveGesture()
{
	QMessageBox::about(this, tr("About TAM"), tr("things"));
	qDebug() << "saveGesture";
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
		QAction *action = new QAction(tr("Recognizer %1").arg(pchar) , this);
		connect(action, SIGNAL(triggered()), this, SLOT(recognizer()));
		recognizerActions.append(action);
	}

	playbackAction = new QAction(tr("&PlayBack"), this);
	connect(playbackAction, SIGNAL(triggered()), drawArea, SLOT(playback()));

	saveAction = new QAction(tr("&Save"), this);
	qDebug() << "Here???";
	connect(saveAction, SIGNAL(triggered()), this, SLOT(saveGesture()));
}

void MainWindow::loadMenus()
{
	sizeMenu = new QMenu(tr("&BrushSize"), this);
	foreach(QAction *action, sizeActions)
		sizeMenu->addAction(action);

	recognizerMenu = new QMenu(tr("&Recognizer"), this);
	foreach(QAction *action, recognizerActions)
		recognizerMenu->addAction(action);

	menuBar()->addMenu(sizeMenu);
	menuBar()->addMenu(recognizerMenu);
	menuBar()->addAction(playbackAction);
	menuBar()->addAction(saveAction);
}

void MainWindow::doResizeBrush(int i)
{
	qDebug() << i;
}