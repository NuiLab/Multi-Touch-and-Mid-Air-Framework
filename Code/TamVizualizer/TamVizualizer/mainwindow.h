#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "glwindow.h"
#include "debugwindow.h"

/* The Main Window that is presented to the user */
class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow();

	/* List of all file types to export from */
	QList<QString> type_list;
	QString type_option;

protected:
	/* slots for the communication between MainWindow and GLWindow */
	public slots:
		void resizeBrush();		/* Calls GLWindow to change the size of the brush (a.k.a. Finger-shape) */
		bool saveGesture();		/* Calls GLWindow to save current recording of touch screen input  */
		bool openGesture();		/* Calls GLWindow to open playback recordings of touch screen data  */
		void map();				/* Calls GLWindow to change mapping [FEATURE DISABLED] */
		void changeDisplay();	/* Calls GLWindow to change the display setting */
		bool loadOBJFile();		/* Calls GLWindow for the purpose of loading  in your own OBJ models*/


protected:
	/* What to do when the MainWindow is exiting */
	void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private:
	GLWindow *window;		/* GLWindow that the MainWindow displays to the user */
	DebugWindow *debug;		/* Debug Window used to replace the Console (To be further developed)*/

	/*Menus for the GUI*/
	QMenu *sizeMenu;
	QMenu *mapMenu;
	QMenu *displayMenu;

	/*Actions or Buttons for the GUI*/
	QList<QAction *> sizeActions;		/* List to change the brush size (a.k.a. Finger-shape)*/
	QList<QAction *> mapActions;		/* List to display the different mapping settings for the display [FEATURE DISABLED] */
	QList<QAction *> displayActions;	/* List to change how the finger-input data is displayed */
	QAction *playbackAction;		/* Button-action to playback current recording */
	QAction *saveAction;			/* Button-action to save curren recording */
	QAction *openAction;			/* Button-action to open a saved recording */
	QAction *clearScreenAction;		/* Button-action to clear screen and recorded data */
	QAction *objAction;				/* Button-action to load OBJ Files */


	
	void loadActions();			/*Functions to itialize the Button Actions*/
	void loadMenus();			/*Functions to load and display the Menus on the Window*/
};

#endif // MAINWINDOW_H
