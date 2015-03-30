#include <QtGui>
#include <qthread.h>
#include "qprinter.h"
#include "drawArea.h"
#include "qprintdialog.h"

DrawArea::DrawArea(QWidget *parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_StaticContents);
}