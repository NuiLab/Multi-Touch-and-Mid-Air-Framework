#include <stdio.h>
#include <stdlib.h>

#include <QtCore>
#include <QtGui>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QSizePolicy>

#include "AviationVisualizer.h"

AviationVisualizer::AviationVisualizer(QWidget *parent) : QWidget(parent)
{
	// Setup layout
	setupLayout();

	// Set default info
	m_infoList->getData()["roll"] = QString("%1").arg(m_ADI->getRoll());
	m_infoList->getData()["pitch"] = QString("%1").arg(m_ADI->getPitch());
	m_infoList->getData()["yaw"] = QString("%1").arg(m_Compass->getYaw());
	m_infoList->getData()["acceleration"] = QString("%1").arg(m_Accel->getAccel());
	m_infoList->getData()["acceleration X"] = QString("%1").arg(m_Accel->getAxisX());
	m_infoList->getData()["acceleration Y"] = QString("%1").arg(m_Accel->getAxisY());
	m_infoList->getData()["acceleration Z"] = QString("%1").arg(m_Accel->getAxisZ());
	m_infoList->listReload();

	// Set window minimum size
	this->setMinimumSize(800, 600);

	// Set focus to this window
	this->setFocus();

	// Window title
	setWindowTitle("Gyroscope Visualizer");
}

AviationVisualizer::~AviationVisualizer()
{

}

int AviationVisualizer::setupLayout(void)
{
	// Top panel
	QWidget *wTopPanel = new QWidget(this);
	QHBoxLayout *hl = new QHBoxLayout(wTopPanel);
	wTopPanel->setLayout(hl);
	wTopPanel->setFocusPolicy(Qt::NoFocus);

	m_ADI = new QADI(this);
	m_Compass = new QCompass(this);
	m_Accel = new QAccel(this);

	hl->addWidget(m_ADI, 0, Qt::AlignTop | Qt::AlignHCenter);
	hl->addWidget(m_Compass, 0, Qt::AlignTop | Qt::AlignHCenter);
	hl->addWidget(m_Accel, 0, Qt::AlignTop | Qt::AlignHCenter);
	hl->setMargin(0);
	hl->setSpacing(4);

	// Bottom panel
	m_infoList = new QKeyValueListView(this);

	// Overall layout
	QVBoxLayout *vl = new QVBoxLayout(this);
	this->setLayout(vl);

	vl->addWidget(wTopPanel, 0);
	vl->addWidget(m_infoList, 2, 0);

	return 0;
}


void AviationVisualizer::keyPressEvent(QKeyEvent *event)
{
	int     key;
	double  v;

	key = event->key();

	if (key == Qt::Key_Up) {
		v = m_ADI->getPitch();
		m_ADI->setPitch(v + 1.0);
	}
	else if (key == Qt::Key_Down) {
		v = m_ADI->getPitch();
		m_ADI->setPitch(v - 1.0);
	}
	else if (key == Qt::Key_Left) {
		v = m_ADI->getRoll();
		int a = m_Accel->getAccel();
		m_Accel->setData(a - 1.0);
		m_ADI->setRoll(v - 1.0);
	}
	else if (key == Qt::Key_Right) {
		v = m_ADI->getRoll();
		int a = m_Accel->getAccel();
		m_Accel->setData(a + 1.0);
		m_ADI->setRoll(v + 1.0);
	}
	else if (key == Qt::Key_A) {
		v = m_Compass->getYaw();
		m_Compass->setYaw(v + 1.0);
	}
	else if (key == Qt::Key_D) {
		v = m_Compass->getYaw();
		m_Compass->setYaw(v - 1.0);
	}
	else if (key == Qt::Key_W) {
		v = m_Compass->getAlt();
		m_Compass->setAlt(v + 1.0);
	}
	else if (key == Qt::Key_S) {
		v = m_Compass->getAlt();
		m_Compass->setAlt(v - 1.0);
	}
	else if (key == Qt::Key_J) {
		v = m_Compass->getH();
		m_Compass->setH(v + 1.0);
	}
	else if (key == Qt::Key_K) {
		v = m_Compass->getH();
		m_Compass->setH(v - 1.0);
	}

	m_infoList->getData()["roll"] = QString("%1").arg(m_ADI->getRoll());
	m_infoList->getData()["pitch"] = QString("%1").arg(m_ADI->getPitch());
	m_infoList->getData()["yaw"] = QString("%1").arg(m_Compass->getYaw());
	m_infoList->getData()["acceleration"] = QString("%1").arg(m_Accel->getAccel());
	m_infoList->getData()["acceleration X"] = QString("%1").arg(m_Accel->getAxisX());
	m_infoList->getData()["acceleration Y"] = QString("%1").arg(m_Accel->getAxisY());
	m_infoList->getData()["acceleration Z"] = QString("%1").arg(m_Accel->getAxisZ());
	m_infoList->listReload();
}

void AviationVisualizer::mousePressEvent(QMouseEvent *event)
{
	QWidget::mousePressEvent(event);
}

void AviationVisualizer::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
}

bool AviationVisualizer::setPRYA(double pitch, double roll, double yaw, double accel, double Ax, double Ay, double Az)
{
	// Register new values
	m_ADI->setPitch(pitch);
	m_ADI->setRoll(roll);
	m_Compass->setYaw(yaw);
	m_Accel->setData(accel);
	m_Accel->setAxisAccel(Ax, Ay, Az);

	// Set modified info
	m_infoList->getData()["roll"] = QString("%1").arg(m_ADI->getRoll());
	m_infoList->getData()["pitch"] = QString("%1").arg(m_ADI->getPitch());
	m_infoList->getData()["yaw"] = QString("%1").arg(m_Compass->getYaw());
	m_infoList->getData()["acceleration"] = QString("%1").arg(m_Accel->getAccel());
	m_infoList->getData()["acceleration X"] = QString("%1").arg(m_Accel->getAxisX());
	m_infoList->getData()["acceleration Y"] = QString("%1").arg(m_Accel->getAxisY());
	m_infoList->getData()["acceleration Z"] = QString("%1").arg(m_Accel->getAxisZ());
	m_infoList->listReload();

	return true;
}

void AviationVisualizer::setGyro(double pitch, double roll, double yaw, double accel, double Ax, double Ay, double Az)
{
	this->setPRYA(pitch, roll, -yaw, accel, Ax, Ay, Az);
}