#include "GyroscopeVisualizer.h"
#include "GyroThread.h"

#include <QTabWidget>

GyroscopeVisualizer::GyroscopeVisualizer(QWidget *parent) : QMainWindow(parent)
{
	QTabWidget *tab = new QTabWidget(this);

	GyroThread *thread = new GyroThread();
	openGLWindow = new GLWindow();
	gyroGauges = new AviationVisualizer();

	tab->addTab(gyroGauges, "Aviation Dashboard");
	tab->addTab(openGLWindow, "3D Visualizer");

	setCentralWidget(tab);

	setWindowTitle("Gyroscope Visualizer");
	setMinimumSize(1000, 800);

	QObject::connect(thread, SIGNAL(gyroData(double, double, double, double, double, double, double)),
		gyroGauges, SLOT(setGyro(double, double, double, double, double, double, double)));

	QObject::connect(thread, SIGNAL(GLData(float, float, float, float)),
		openGLWindow, SLOT(setGLData(float, float, float, float)));

	QObject::connect(thread, SIGNAL(sendError(QString)), this, SLOT(showErrorMsg(QString)));

	thread->start();
}

GyroscopeVisualizer::~GyroscopeVisualizer()
{

}

void GyroscopeVisualizer::showErrorMsg(QString msg){
	QMessageBox errorMsg;

	errorMsg.setWindowTitle("Error!");
	errorMsg.setText(msg);

	errorMsg.exec();
}