#include "mainwindow.h"
#include "drawArea.h"
#include <ctime>
#include <cstdlib>
#include <QtWidgets/QApplication>

using namespace std;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	DrawArea::touchData data;
	data.id = 0;
	data.x = 50;
	data.y = 50;
	data.timeStamp = time(0);

	MainWindow w(data);
	w.showMaximized();
	return a.exec();
}
