#include <QtGui>
#include <qthread.h>
#include "qprinter.h"
#include "drawArea.h"
#include "qprintdialog.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <Windows.h>
#include <ctime>
#include <iomanip>

using namespace std;

/*Main constructor for the DrawArea object
Sets upt the windows touch event connection and 
the click event connections also initializes a list
of ten colors*/
DrawArea::DrawArea(QWidget *parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_AcceptTouchEvents);
	setAttribute(Qt::WA_StaticContents);
	idColors
		<< QColor("red")
		<< QColor("yellow")
		<< QColor("blue")
		<< QColor("green")
		<< QColor("orange")
		<< QColor("brown")
		<< QColor("pink")
		<< QColor("cyan")
		<< QColor("purple")
		<< QColor("black");
}

/*Supplementary function to display th elist of touch points
into the debug monitor*/
void DrawArea::printthedata()
{
	foreach(touchData data, thePoints)
	{
		qDebug() << "POINT: \n";
		qDebug() << "X: " << data.x;
		qDebug() << "Y: " << data.y;
		qDebug() << "ID: " << data.id;
		qDebug() << "TIME STAMP: " << ctime(&data.timeStamp);
	}
}

/*Changes the global brushSize variable to the selected size i*/
void DrawArea::doResizeBrush(int i)
{
	brushSize = i;
	qDebug() << "BRUSH SIZE: " << i;
}

/*Changes the global map variable to the selected map fuunction*/
void DrawArea::doMap(int map)
{
	switch (map)
	{
	case 1:
		mapping = 1;
		qDebug() << "MAPPING FUNCTION SELECTED IS 1";
		break;
	case 2:
		mapping = 2;
		qDebug() << "MAPPING FUNCTION SELECTED IS 2";
		break;
	case 3:
		mapping = 0.5;
		qDebug() << "MAPPING FUNCTION SELECTED IS 1/2";
		break;
	default: 
		mapping = 1;
	}
}

/*Opens a file stream to save the touch points into
goes trough the list of touch points and writes them in order*/
bool DrawArea::doSaveGesture(QString fileName)
{
	qDebug() << "SAVING THE FILE";
	ofstream file;
	file.open(fileName.toStdString());
	foreach(touchData data, thePoints)
	{
		file << data.x << "," << data.y << "," << data.id << "," <<data.timeStamp <<endl;
	}
	return true;
}

/*Opens a file stream to read the touchpoints, 
reads the data one line at a time and then
reads the four elements contained on the line
while adding this touch pounts objects into the touchpoint list*/
bool DrawArea::doOpenGesture(QString fileName)
{
	qDebug() << "OPPENING A FILE";
	thePoints.clear();
	
	ifstream file;
	file.open(fileName.toStdString());
	string line;

	while (getline(file,line))
	{
		touchData data;
		istringstream s(line);
		string field;
		getline(s, field, ',');
		data.x = stoi(field);
		getline(s, field, ',');
		data.y = stoi(field);
		getline(s, field, ',');
		data.id = stoi(field);
		getline(s, field, ',');
		data.timeStamp = stoi(field);
		thePoints.append(data);
		
	}playback();
	return true;
}

/*Goes trough the touchPoint list and extracts the X and Y axis
and the id then draws a rectangle the size of the brush size
until the list is empty*/
void DrawArea::playback()
{
	qDebug() << "ON PLAYBACK";
	clearScreen();
	foreach(touchData data, thePoints)
	{
		QRectF rect = QRect(data.x, data.y, brushSize, brushSize);
		if (rect.isEmpty()) {
			qreal diameter = qreal(50) * 1;
			rect.setSize(QSizeF(diameter, diameter));
		}

		QPainter painter(&image);
		painter.setPen(Qt::NoPen);
		painter.setBrush(idColors.at(data.id % idColors.count()));
		painter.drawEllipse(rect);
		painter.end();
		QThread::yieldCurrentThread();
		qApp->processEvents();

		QThread::msleep(5);
		bool b = this->updatesEnabled();
		int rad = 2;
		update(rect.toRect().adjusted(-rad, -rad, +rad, +rad));
	}
	thePoints.clear();
}

/*Draws a clear immage the size of the draw area
effectively clearing the screen*/
void DrawArea::clearScreen()
{
	qDebug() << "CLEARING THE SCREEN";
	image.fill(qRgb(255, 255, 255));
	update();
}

/*Supplementary function that draws the image into the screen*/
void DrawArea::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	const QRect rect = event->rect();
	painter.drawImage(rect.topLeft(), image, rect);
}

/*Supplementary function that triggers an imageResize*/
void DrawArea::resizeEvent(QResizeEvent *event)
{
	if (width() > image.width() || height() > image.height())
	{
		int newWidth = qMax(width() + 128, image.width());
		int newHeight = qMax(height() + 128, image.height());
		resizeImage(&image, QSize(newWidth, newHeight));
		update();
	}
	QWidget::resizeEvent(event);
}

/*Supplementary function that resizes the image*/
void DrawArea::resizeImage(QImage *image, const QSize &newSize)
{
	if (image->size() == newSize)
		return;
	QImage newImage(newSize, QImage::Format_RGB32);
	newImage.fill(qRgb(255, 255, 255));
	QPainter painter(&newImage);
	painter.drawImage(QPoint(0, 0), *image);
	*image = newImage;
}

/*The function that does all the input/output,
this function gets triggered by a touch event,
saves the data of the touch event as a touch point 
and adds the touch point to a list, then draws the 
output depending on the mapping function*/
bool DrawArea::event(QEvent *event)
{
	switch (event->type())
	{
	case QEvent::TouchBegin:
	case QEvent::TouchUpdate:
	case QEvent::TouchEnd:
		{
			QTouchEvent *touch = static_cast<QTouchEvent *>(event);
			QList<QTouchEvent::TouchPoint> touchPoints = static_cast<QTouchEvent *>(event)->touchPoints();
			foreach(const QTouchEvent::TouchPoint &touchPoint, touchPoints)
			{
				switch (touchPoint.state())
				{
				case Qt::TouchPointStationary:
					continue;
				default:
				{
					QRectF rect = QRect(touchPoint.pos().rx() * mapping, touchPoint.pos().ry() * mapping, brushSize, brushSize);
					if (rect.isEmpty())
					{
						qreal diameter = qreal(brushSize);
						rect.setSize(QSize(diameter, diameter));
					}
					touchData data;
					data.x = touchPoint.pos().rx() * mapping;
					data.y = touchPoint.pos().ry() * mapping;
					data.id = touchPoint.id();
					data.timeStamp = time(NULL);
					qDebug() << "POINT: \n";
					qDebug() << "X: " << data.x;
					qDebug() << "Y: " << data.y;
					qDebug() << "ID: " << data.id;
					qDebug() << "TIME STAMP: " << ctime(&data.timeStamp);
					thePoints << data;
					QPainter painter(&image);
					painter.setPen(Qt::NoPen);
					painter.setBrush(idColors.at(touchPoint.id() % idColors.count()));
					painter.drawEllipse(rect);
					painter.end();

					int rad = 2;
					update(rect.toRect().adjusted(-rad, -rad, +rad, +rad));
				}
				break;
				}
			}
			break;
		}
	default:
		return QWidget::event(event);
	}
	return true;
}