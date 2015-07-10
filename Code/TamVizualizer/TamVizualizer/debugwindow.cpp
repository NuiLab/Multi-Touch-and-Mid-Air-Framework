#include "debugwindow.h"


bool DebugWindow::exists = false; 
QString DebugWindow::prev;
QScrollArea *DebugWindow::scrollarea;
QTextEdit *DebugWindow::debugText;

/* Main function
Initializes the draw area object, menus and actions(buttons),
sets name of the window and size
*/
DebugWindow::DebugWindow() {
	scrollarea = new QScrollArea;
	debugText = new QTextEdit();
	scrollarea->setWidget(debugText);
	scrollarea->setWidgetResizable(true);
	//scrollarea->resize(500, 200);
	scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
	scrollarea->ensureVisible(0, debugText->height());
	setCentralWidget(scrollarea);
	//setCentralWidget(debugText);

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

void DebugWindow::print(const stringstream &message){
	QString mess = tr(message.str().c_str());
	if (!exists){
		prev += mess;
	} else {
		debugText->append(mess + "\n");
		scrollarea->ensureVisible(0, debugText->height());
	}
}

void DebugWindow::println(const stringstream &message){
	stringstream newline;
	newline << message.str() << endl;
	DebugWindow::print(newline);
}