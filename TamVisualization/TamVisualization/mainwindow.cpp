#include <QtGui>
#include <cstdlib>

#include "mainwindow.h"
#include "replayarea.h"
#include "QFileDialog"
#include "QByteArray"
#include "qaction"
#include "qmessagebox"
#include "qmenu"
#include "qmenubar"


using namespace std;

MainWindow::MainWindow(vector<vector<int>> *coords)
{
	replayArea = new ReplayArea();
	setCentralWidget(replayArea);
	createActions();
	createMenus();

	setWindowTitle(tr("TamVisualizer"));
	resize(500, 500);
	replayArea->theCoords = *coords;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if (maybeSave()){
		event->accept();
	}
	else {
		event->ignore();
	}
}

void MainWindow::open()
{
	if (maybeSave()) {
		QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
		if (!fileName.isEmpty())
			replayArea->openImage(fileName);
	}
}

void MainWindow::save()
{
	QAction *action = qobject_cast<QAction *>(sender());
	QByteArray fileFormat = action->data().toByteArray();
	saveFile(fileFormat);
}

void MainWindow::about()
{
	QMessageBox::about(this, tr("About TamVisualizer"), tr("<p>This are <b>words</b> and i expect them to</p> <p>be formatted correctly</p>"));
}

void MainWindow::createActions()
{
	openAct = new QAction(tr("&Open..."), this);
	openAct->setShortcut(tr("Ctrl+O"));
	connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

	foreach (QByteArray format, QImageWriter::supportedImageFormats())
	{
		QString text = tr("%1...").arg(QString(format).toUpper());
		QAction *action = new QAction(text, this);
		action->setData(format);
		connect(action, SIGNAL(triggered()), this, SLOT(save()));
		saveAsActs.append(action);
	}

	printAct = new QAction(tr("&Print..."), this);
	connect(printAct, SIGNAL(triggered()), replayArea, SLOT(print()));

	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcut(tr("Ctrl+Q"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	clearScreenAct = new QAction(tr("&Clear Screen"), this);
	clearScreenAct->setShortcut(tr("Ctrl+L"));
	connect(clearScreenAct, SIGNAL(triggered()),
		replayArea, SLOT(clearImage()));

	playAct = new QAction(tr("&Play"), this);
	connect(playAct, SIGNAL(triggered()), replayArea, SLOT(play()));

	aboutAct = new QAction(tr("&About"), this);
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
	saveAsMenu = new QMenu(tr("&Save As"), this);
	foreach(QAction *action, saveAsActs)
		saveAsMenu->addAction(action);

	fileMenu = new QMenu(tr("&File"), this);
	fileMenu->addAction(openAct);
	fileMenu->addMenu(saveAsMenu);
	fileMenu->addAction(printAct);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);

	optionMenu = new QMenu(tr("&Options"), this);
	optionMenu->addAction(clearScreenAct);
	optionMenu->addAction(playAct);

	helpMenu = new QMenu(tr("&Help"), this);
	helpMenu->addAction(aboutAct);

	menuBar()->addMenu(fileMenu);
	menuBar()->addMenu(optionMenu);
	menuBar()->addMenu(helpMenu);
}

bool MainWindow::maybeSave()
{
	if (replayArea->isModified()) {
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(this, tr("TamVisualizers"),
			tr("The image has been modified.\n"
			"Do you want to save your changes?"),
			QMessageBox::Save | QMessageBox::Discard
			| QMessageBox::Cancel);
		if (ret == QMessageBox::Save) {
			return saveFile("png");
		}
		else if (ret == QMessageBox::Cancel) {
			return false;
		}
	}
	return true;
}

bool MainWindow::saveFile(const QByteArray &fileFormat)
{
	QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
		initialPath,
		tr("%1 Files (*.%2);;All Files (*)")
		.arg(QString(fileFormat.toUpper()))
		.arg(QString(fileFormat)));
	if (fileName.isEmpty()) {
		return false;
	}
	else {
		return replayArea->saveImage(fileName, fileFormat);
	}
}