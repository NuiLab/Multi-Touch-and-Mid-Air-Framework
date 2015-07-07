#include "debugwindow.h"

/* Main function
Initializes the draw area object, menus and actions(buttons),
sets name of the window and size
*/
DebugWindow::DebugWindow() {
	setCentralWidget(debugText);

	setWindowTitle(tr("TAM Debugger"));
	resize(500, 200);

	debugText->setText(tr("Testing . . . \n"));
	for (int i = 0; i < 50; i++) {
		debugText->append("Testing 1 2 3 . . . \n");
	}
}