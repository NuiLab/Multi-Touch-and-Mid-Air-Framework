#include "tamvisualization.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	TamVisualization w;
	w.show();
	return a.exec();
}
