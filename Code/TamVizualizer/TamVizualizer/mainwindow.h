#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "drawArea.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(DrawArea::touchData data);

protected:
	public slots:
	void resizeBrush();
	bool saveGesture();
	void recognizer();

private:
	DrawArea *drawArea;
	void loadActions();
	void loadMenus();

	QMenu *sizeMenu;
	QMenu *recognizerMenu;

	QList<QAction *> sizeActions;
	QList<QAction *> recognizerActions;
	QAction *playbackAction;
	QAction *saveAction;
	QAction *clearScreenAction;
};

#endif // MAINWINDOW_H
