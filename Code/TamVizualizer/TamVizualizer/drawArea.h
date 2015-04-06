#ifndef DRAWAREA_H
#define DRAWAREA_H

#include <cstdlib>
#include <qcolor.h>
#include <qimage.h>
#include <qpoint.h>
#include <qwidget.h>

using namespace std;

class DrawArea : public QWidget
{
	Q_OBJECT

public:
	DrawArea(QWidget *parent = 0);
	struct touchData
		{
			long long x;
			long long y;
			int id;
			double timeStamp;
		};

	void doResizeBrush(int i);
	void doRecognize(int recognizer);
	bool doSaveGesture(QString fileName);

	public slots:
	void playback();
	void clearScreen();

protected:
	bool event(QEvent *event);
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
	QList<QColor> idColors;
	
private:
	int brushSize = 5;
	QImage image;
	void resizeImage(QImage *image, const QSize &newSize);
	QList<touchData> thePoints;
	void printthedata();
};
#endif