#include <qthread.h>
#include "globaldata.h"
#include "shapedata.h"


class ProcessorThread : public QThread
{
	Q_OBJECT
public:
	ProcessorThread();
	~ProcessorThread();


	void setProcess(DisplaySetting setting);
	void setFingers(const QList<touch_data> &fingers);
	QList<TAMShape *> getResults();
protected:
	void run() override;
private:
	QList<touch_data> finger_data;
	QList<TAMShape *> results;
	QMutex finger_m, result_m, update_m;
	volatile DisplaySetting setting;
	volatile bool running, update;

	int brushSize = 50;
	
	void operationFingerAveraging(const QList<touch_data> &input, QList<TAMShape *> &output);
	void operationShortestMapping(const QList<touch_data> &input, QList<TAMShape *> &output);
	void operationCircularConnection(const QList<touch_data> &input, QList<TAMShape *> &output);
	void operationShortestPath(const QList<touch_data> &input, QList<TAMShape *> &output);
};