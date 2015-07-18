#include "gyroscopevisualizer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	GyroscopeVisualizer w;
	w.show();

	return a.exec();
}
