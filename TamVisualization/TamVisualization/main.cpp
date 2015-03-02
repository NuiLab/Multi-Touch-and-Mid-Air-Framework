#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	vector < vector <int> > grid1(4, vector<int>(2));

	grid1[0][0] = 100;
	grid1[0][1] = 100;

	grid1[1][0] = 200;
	grid1[1][1] = 200;

	grid1[2][0] = 300;
	grid1[2][1] = 300;

	grid1[3][0] = 400;
	grid1[3][1] = 400;

	MainWindow window(&grid1);
	window.showMaximized();
	return app.exec();
}
