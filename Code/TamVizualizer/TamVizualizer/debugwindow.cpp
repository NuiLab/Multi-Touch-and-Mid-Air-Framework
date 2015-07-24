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
	// Set up Debug Window
	scrollarea = new QScrollArea;
	debugText = new QTextEdit();
	scrollarea->setWidget(debugText);
	scrollarea->setWidgetResizable(true);
	scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
	scrollarea->ensureVisible(0, debugText->height());
	setCentralWidget(scrollarea);
	setWindowTitle(tr("TAM Debugger"));
	resize(500, 200);

	// Print to the DebugWindow any text that should have been printed before initialization
	if (prev != nullptr || !prev.isEmpty()){
		debugText->setText(prev);
		prev.clear();
	}
	exists = true;
}

/* Prints to the DebugWindow */
void DebugWindow::print(const stringstream &message){
	QString mess = tr(message.str().c_str());
	if (!exists){
		prev += mess;
	} else {
		debugText->append(mess + "\n");
		scrollarea->ensureVisible(0, debugText->height());
	}
}

/* Prints to the DebugWindow in a new-line*/
void DebugWindow::println(const stringstream &message){
	stringstream newline;
	newline << message.str() << endl;
	DebugWindow::print(newline);
}