#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	vector < vector <int> > grid1(2, vector<int>(4));

	grid1[0][0] = 10;
	grid1[0][1] = 10;

	grid1[1][0] = 20;
	grid1[1][1] = 20;

	grid1[2][0] = 30;
	grid1[2][1] = 30;

	grid1[3][0] = 40;
	grid1[3][1] = 40;

	MainWindow window(&grid1);
	window.showMaximized();
	return app.exec();
}
