#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QTextEdit>
#include "glwindow.h"

class DebugWindow : public QMainWindow {
	Q_OBJECT

public:
	DebugWindow();

private:
	QScrollArea *scrollArea = new QScrollArea;
	QTextEdit *debugText = new QTextEdit;
};

#endif // DEBUGWINDOW_H
