#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "drawArea.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

protected:
	public slots:
	void resizeBrush();
	bool saveGesture();
	bool openGesture();
	void map();

private:
	DrawArea *drawArea;
	void loadActions();
	void loadMenus();

	QMenu *sizeMenu;
	QMenu *mapMenu;

	QList<QAction *> sizeActions;
	QList<QAction *> mapActions;
	QAction *playbackAction;
	QAction *saveAction;
	QAction *openAction;
	QAction *clearScreenAction;
};

#endif // MAINWINDOW_H
