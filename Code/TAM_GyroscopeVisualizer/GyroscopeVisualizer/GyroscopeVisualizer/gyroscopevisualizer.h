#ifndef GYROSCOPEVISUALIZER_H
#define GYROSCOPEVISUALIZER_H

#include <QtWidgets/QMainWindow>
#include <QMessageBox>

#include "glwindow.h"
#include "AviationVisualizer.h"
#include "ui_gyroscopevisualizer.h"

class GyroscopeVisualizer : public QMainWindow
{
	Q_OBJECT

public:
	GyroscopeVisualizer(QWidget *parent = 0);
	~GyroscopeVisualizer();

	GLWindow* openGLWindow;
	AviationVisualizer* gyroGauges;

	public slots:
	void showErrorMsg(QString msg);
};

#endif // GYROSCOPEVISUALIZER_H
