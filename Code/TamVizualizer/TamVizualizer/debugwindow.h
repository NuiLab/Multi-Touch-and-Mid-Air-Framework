#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QTextEdit>
#include <fstream>
#include <sstream>
#include <qscrollarea.h>

using namespace std;

/* Class to build a DebugWindow that will replace the Console.
   This is so that a console can appear in OS's other than Windows */
class DebugWindow : public QMainWindow {
	Q_OBJECT

public:
	DebugWindow();

	/* Print to the Debug Window (just like a Console)*/
	static void print(const stringstream &message);	
	static void println(const stringstream &message);
	
private:
	static QString prev;				/* Where printed results are stored before the window is initialized */
	static bool exists;					/* Checks if the DebugWindow exists */
	static QScrollArea *scrollarea;		/* ScrollArea to set auto-scroll to the very bottom */
	static QTextEdit *debugText;		/* The Text to be displayed on the DebugWindow*/
};

#endif // DEBUGWINDOW_H
