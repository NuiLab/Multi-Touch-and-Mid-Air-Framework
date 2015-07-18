#include <qthread.h>

class GyroThread : public QThread
{
	Q_OBJECT

	public:
		GyroThread(QWidget *parent = NULL) {}

	signals:
		void gyroData(double pitch, double roll, double yaw, double accel, double Ax, double Ay, double Az);
		void GLData(float x, float y, float z, float w);
		void sendError(QString msg);

	private:
		virtual void run();
};