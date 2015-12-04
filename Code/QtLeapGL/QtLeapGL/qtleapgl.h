#ifndef QTLEAPGL_H
#define QTLEAPGL_H

#include <QtWidgets/QMainWindow>
#include "ui_qtleapgl.h"

class QtLeapGL : public QMainWindow
{
	Q_OBJECT

public:
	QtLeapGL(QWidget *parent = 0);
	~QtLeapGL();

private:
	Ui::QtLeapGLClass ui;
};

#endif // QTLEAPGL_H
