#include <QtGui>
#include <qthread.h>
#include "qprinter"
#include "replayarea.h"
#include "qprintdialog"

ReplayArea::ReplayArea(QWidget *parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_StaticContents);
	modified = false;

	myPenColors
		<< QColor("green")
		<< QColor("purple")
		<< QColor("red")
		<< QColor("blue")
		<< QColor("yellow")

		<< QColor("pink")
		<< QColor("orange")
		<< QColor("brown")
		<< QColor("grey")
		<< QColor("black");
}

bool ReplayArea::openImage(const QString &fileName)
{
	QImage loadedImage;
	if (!loadedImage.load(fileName))
		return false;

	QSize newSize = loadedImage.size().expandedTo(size());
	resizeImage(&loadedImage, newSize);
	image = loadedImage;
	modified = false;
	update();
	return true;
}

bool ReplayArea::saveImage(const QString &fileName, const char *fileFormat)
{
	QImage visibleImage = image;
	resizeImage(&visibleImage, size());

	if (visibleImage.save(fileName, fileFormat)) {
		modified = false;
		return true;
	}
	else {
		return false;
	}
}

void ReplayArea::clearImage()
{
	image.fill(qRgb(255, 255, 255));
	modified = true;
	update();
}

void ReplayArea::play()
{
	for (int x = 0; x < theCoords.size(); ++x) {
		int y = 0;
			//qDebug() << "The vector is : " << x << ", " << y;
			//qDebug() << "The Coords are: " << theCoords[x][y] << ", " << theCoords[x][y+1];
			QRectF rect = QRect(theCoords[x][y], theCoords[x][y+1], 50, 50);
			if (rect.isEmpty()) {
				qreal diameter = qreal(50) * 1;
				rect.setSize(QSizeF(diameter, diameter));
			}

			QPainter painter(&image);
			painter.setPen(Qt::NoPen);
			painter.setBrush(myPenColors.at(9 % myPenColors.count()));
			painter.drawEllipse(rect);
			painter.end();
			QThread::yieldCurrentThread();
			qApp->processEvents();
			
			QThread::msleep(100);
			bool b = this->updatesEnabled();
			modified = true;
			int rad = 2;
			update(rect.toRect().adjusted(-rad, -rad, +rad, +rad));
	}
}

void ReplayArea::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	const QRect rect = event->rect();
	painter.drawImage(rect.topLeft(), image, rect);
}

void ReplayArea::resizeEvent(QResizeEvent *event)
{
	if (width() > image.width() || height() > image.height()) {
		int newWidth = qMax(width() + 128, image.width());
		int newHeight = qMax(height() + 128, image.height());
		resizeImage(&image, QSize(newWidth, newHeight));
		update();
	}
	QWidget::resizeEvent(event);
}

void ReplayArea::resizeImage(QImage *image, const QSize &newSize)
{
	if (image->size() == newSize)
		return;
	QImage newImage(newSize, QImage::Format_RGB32);
	newImage.fill(qRgb(255, 255, 255));
	QPainter painter(&newImage);
	painter.drawImage(QPoint(0, 0), *image);
	*image = newImage;
}

void ReplayArea::print()
{
	/*To be Implemented*/
}

bool ReplayArea::event(QEvent *event)
{
	/*To be Implemented*/
	return QWidget::event(event);
}