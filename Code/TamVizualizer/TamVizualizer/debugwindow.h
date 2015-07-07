#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QTextEdit>
#include "glwindow.h"

class DebugWindow : public QMainWindow {
	Q_OBJECT

public:
	DebugWindow();
	static void print(const QString& message);	
	static void println(const QString& message);
	
private:
	static QString prev;
	static bool exists;
	//QScrollArea *scrollArea = new QScrollArea;
	static QTextEdit * debugText;
};

#endif // DEBUGWINDOW_H
