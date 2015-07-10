#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QTextEdit>
#include "glwindow.h"

class DebugWindow : public QMainWindow {
	Q_OBJECT

public:
	DebugWindow();
	static void print(const stringstream &message);	
	static void println(const stringstream &message);
	
private:
	static QString prev;
	static bool exists;
	//QScrollArea *scrollArea = new QScrollArea;
	static QScrollArea *scrollarea;
	static QTextEdit *debugText;
	static QList<QString*> stringlist;
};

#endif // DEBUGWINDOW_H
