#include <QtGui>
#include <qthread.h>
#include "qprinter.h"
#include "drawArea.h"
#include "qprintdialog.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <Windows.h>
#include <ctime>

using namespace std;

void DrawArea::printthedata()
{
	foreach(touchData data, thePoints)
	{
		qDebug() << data.x;
		qDebug() << data.y;
		qDebug() << data.id;
		qDebug() << data.timeStamp;
	}
}

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

void DrawArea::doResizeBrush(int i)
{
	brushSize = i;
	qDebug() << i;
}

void DrawArea::doRecognize(int recognizer)
{
	printthedata();
	qDebug() << recognizer;
}

bool DrawArea::doSaveGesture(QString fileName)
{
	ofstream file;
	file.open(fileName.toStdString());
	file << "test" << endl;
	//DO THE SAVE
	return true;
}

void DrawArea::playback()
{
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

		QThread::msleep(10);
		bool b = this->updatesEnabled();
		int rad = 2;
		update(rect.toRect().adjusted(-rad, -rad, +rad, +rad));
	}thePoints.clear();
	qDebug() << "In Playback";
	//TODO
}

void DrawArea::clearScreen()
{
	image.fill(qRgb(255, 255, 255));
	update();
}

void DrawArea::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	const QRect rect = event->rect();
	painter.drawImage(rect.topLeft(), image, rect);
}

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
				qDebug() << "Per point loop";
				switch (touchPoint.state())
				{
				case Qt::TouchPointStationary:
					qDebug() << "At stationary";
					continue;
				default:
				{
					QRectF rect = touchPoint.rect();
					if (rect.isEmpty())
					{
						qreal diameter = qreal(brushSize);
						rect.setSize(QSize(diameter, diameter));
					}
					touchData data;
					data.x = touchPoint.pos().rx();
					data.y = touchPoint.pos().ry();
					data.id = touchPoint.id();
					data.timeStamp = time(0);
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