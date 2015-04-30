#ifndef DEBUG_H
#define DEBUG_H

#include <QtWidgets/QMainWindow>
#include <qlabel.h>

class Debug : public QMainWindow
{
	Q_OBJECT

public:
	Debug();

	/*Function to start the debug monitor, WINDOWS ONLY*/
	void showWin32Console();

protected:

private:
	QLabel *imageLabel = new QLabel;
};

#endif // DEBUG_H