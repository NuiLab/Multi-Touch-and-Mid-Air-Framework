#include "debugwindow.h"


bool DebugWindow::exists = false; 
QString DebugWindow::prev;
QTextEdit *DebugWindow::debugText;

/* Main function
Initializes the draw area object, menus and actions(buttons),
sets name of the window and size
*/
DebugWindow::DebugWindow() {
	debugText = new QTextEdit();
	setCentralWidget(debugText);

	setWindowTitle(tr("TAM Debugger"));
	resize(500, 200);
	/*
	debugText->setText(tr("Testing . . . \n"));
	for (int i = 0; i < 50; i++) {
		debugText->append("Testing 1 2 3 . . . \n");
	}*/
	if (prev != nullptr || !prev.isEmpty()){
		debugText->setText(prev);
		prev.clear();
	}
	exists = true;
}

void DebugWindow::print(const QString &message) {
	if (!exists){
		prev += message;
	} else {
		debugText->append(message);
	}
	qDebug() << message;
}

void DebugWindow::println(const QString &message){
	if (!exists){
		prev += message;
	} else {
		debugText->append(message + "\n");
	}
	qDebug() << message << endl;
}