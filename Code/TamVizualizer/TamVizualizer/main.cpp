#include "mainwindow.h"
#include "debug.h"
#include "debugwindow.h"
#include <ctime>
#include <cstdlib>
#include <QtWidgets/QApplication>
#include <Windows.h>
#include <conio.h>
#include "nativeunittesting.h"

using namespace std;

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);

	// Create a Window's Console (to use for debugging purposes)
	Debug d;
	d.showWin32Console();

	/*
	if (!UnitTest::run()){
		qDebug() << "A Test Failed...";
	}*/

	// Create a MainWindow object and maximize the window
	MainWindow w;
	w.showMaximized();

	return a.exec();
}
