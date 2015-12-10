#include "knvis.h"
#include "GLWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	GLWindow wa;
	wa.showMaximized();
	return a.exec();
}
