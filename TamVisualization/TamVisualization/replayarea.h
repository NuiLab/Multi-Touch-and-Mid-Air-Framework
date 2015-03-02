#ifndef REPLAYAREA_H
#define REPLAYAREA_H

#include <qcolor.h>
#include <qimage.h>
#include <qpoint.h>
#include <qwidget.h>

class ReplayArea : public QWidget
{
	Q_OBJECT

public:
	ReplayArea(QWidget *parent = 0);

	bool openImage(const QString &filename);
	bool saveImage(const QString &filename, const char *fileFormat);

	bool isModified() const { return modified; }

	public slots:
	void clearImage();
	void print();
	void play();

protected:
	void paintEvent(QpaintEvent *event);
	void resizeEvent(QResizeEvent *event);
	bool event(Qevent *event);

private:
	void resizeImage(QImage *image, const QSize &newSize);

	bool modified;
	QList<QColor> myPenColors;
	QImage image;
};

#endif