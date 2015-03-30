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
			long long realX;
			long long realY;
			double timeStamp;
		};
protected:

private:
	
};
#endif