#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "glwindow.h"
#include "debugwindow.h"

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow();
	~MainWindow();

	/* List of all file types to export from */
	QList<QString> type_list;
	QString type_option;

protected:
	/* slots for the communication between MainWindow and DrawArea */
	public slots:
	void resizeBrush();
	bool saveGesture();
	bool openGesture();
	void map();
	void changeDisplay();

private:
	GLWindow *window;
	DebugWindow *debug;

	/*Functions to itialize the Menus and Actions*/
	void loadActions();
	void loadMenus();

	/*Menus for the GUI*/
	QMenu *sizeMenu;
	QMenu *mapMenu;
	QMenu *displayMenu;

	/*Actions or Buttons for the GUI*/
	QList<QAction *> sizeActions;
	QList<QAction *> mapActions;
	QList<QAction *> displayActions;
	QAction *playbackAction;
	QAction *saveAction;
	QAction *openAction;
	QAction *clearScreenAction;
};

#endif // MAINWINDOW_H
