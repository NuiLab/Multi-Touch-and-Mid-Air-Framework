#include <QtWidgets/QApplication>
#include "glwindow.h"
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <string>
#include <conio.h>
#include <Windows.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	GLWindow w;
	w.show();	
	
	return a.exec();
}
