#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "drawArea.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

	/*slots for the communication between MainWindow and DrawArea*/
protected:
	public slots:
	void resizeBrush();
	bool saveGesture();
	bool openGesture();
	void map();

private:
	DrawArea *drawArea;
	/*Functions to itialize the Menus and Actions*/
	void loadActions();
	void loadMenus();

	/*Menus for the GUI*/
	QMenu *sizeMenu;
	QMenu *mapMenu;

	/*Actions or Buttons for the GUI*/
	QList<QAction *> sizeActions;
	QList<QAction *> mapActions;
	QAction *playbackAction;
	QAction *saveAction;
	QAction *openAction;
	QAction *clearScreenAction;
};

#endif // MAINWINDOW_H
