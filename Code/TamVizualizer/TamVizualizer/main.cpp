#include "mainwindow.h"
#include "drawArea.h"
#include "debug.h"
#include <ctime>
#include <cstdlib>
#include <QtWidgets/QApplication>
#include <Windows.h>
#include <conio.h>

using namespace std;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	/*Create a debug object and call the functions to staer the debug monitor*/
	Debug d ;
	d.showWin32Console();
	AllocConsole();

	/*Create a MainWindow object and maximize the window*/
	MainWindow w;
	w.showMaximized();
	
	return a.exec();
}
