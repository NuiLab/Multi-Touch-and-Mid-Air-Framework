#ifndef DRAWAREA_H
#define DRAWAREA_H

#include <cstdlib>
#include <qcolor.h>
#include <qimage.h>
#include <qpoint.h>
#include <qwidget.h>
#include "debug.h"

using namespace std;

class DrawArea : public QWidget
{
	Q_OBJECT

public:
	DrawArea(QWidget *parent = 0);

	/*The structure to hold the data of a touch input*/
	struct touchData
		{
			long long x;
			long long y;
			int id;
			time_t timeStamp;
		};

	/*Changes the global brushSize attribute*/
	void doResizeBrush(int i);

	/*Changes the global mapping attribute*/
	void doMap(int map);

	/*Starts the save action by opening a save dialog native to the OS*/
	bool doSaveGesture(QString fileName);

	/*Starts the open actionn by opening an open dialog native to the OS*/
	bool doOpenGesture(QString fileName);

	/*Slots used for communication between DrawArea and MainWindow*/
	public slots:
	void playback();
	void clearScreen();

protected:
	/*The main function that gets triggered when a touch event happens*/
	bool event(QEvent *event);

	/*A supplementary function that does the 2D painting*/
	void paintEvent(QPaintEvent *event);

	/*A supplementary function for controlling window size*/
	void resizeEvent(QResizeEvent *event);

	/* A list of color objects*/
	QList<QColor> idColors;
	
private:
	/*Global size of the brush*/
	int brushSize = 20;

	/*Global value for mapping function*/
	double mapping = 1;

	/*Global immage object to draw*/
	QImage image;


	/*A supplementary function for controlling window size*/
	void resizeImage(QImage *image, const QSize &newSize);

	/*Global list of all the touchPoints*/
	QList<touchData> thePoints;

	/*Supplementary function to display the touch points in the debug monitor*/
	void printthedata();
};
#endif