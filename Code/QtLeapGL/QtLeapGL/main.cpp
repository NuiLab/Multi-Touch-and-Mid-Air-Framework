#include <QtWidgets/QApplication>
#include "glwindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	GLWindow w;
	w.show();
	
	return a.exec();
}
