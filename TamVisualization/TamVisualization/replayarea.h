#ifndef REPLAYAREA_H
#define REPLAYAREA_H

#include <qcolor.h>
#include <qimage.h>
#include <qpoint.h>
#include <qwidget.h>
#include <cstdlib>

using namespace std;

class ReplayArea : public QWidget
{
	Q_OBJECT

public:
	ReplayArea(QWidget *parent = 0);

	bool openImage(const QString &filename);
	bool saveImage(const QString &filename, const char *fileFormat);

	bool isModified() const { return modified; }
	vector<vector <int>> theCoords;
	public slots:
	void clearImage();
	void print();
	void play();

protected:
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
	bool event(QEvent *event);

private:
	void resizeImage(QImage *image, const QSize &newSize);

	bool modified;
	QList<QColor> myPenColors;
	QImage image;
	
};

#endif