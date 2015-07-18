#ifndef __Aviation_Visualizer_H__
#define __Aviation_Visualizer_H__

#include <QtCore>
#include <QtGui>

#include "qFlightInstruments.h"


class AviationVisualizer : public QWidget
{
	Q_OBJECT

	public:
		AviationVisualizer(QWidget *parent = NULL);
		virtual ~AviationVisualizer();

		virtual int setupLayout(void);
		virtual bool setPRYA(double pitch, double roll, double yaw, double accel, double Ax, double Ay, double Az);

	public slots:
		void setGyro(double pitch, double roll, double yaw, double accel, double Ax, double Ay, double Az);

	protected:
		void keyPressEvent(QKeyEvent *event);
		void mousePressEvent(QMouseEvent *event);
		void resizeEvent(QResizeEvent *event);

	protected:
		QADI                *m_ADI;
		QCompass            *m_Compass;
		QAccel				*m_Accel;
		QKeyValueListView   *m_infoList;
};

#endif
