#include <qthread.h>
//#include "globaldata.h"
#include "shapedata.h"

class ProcessorThread : public QThread
{
	Q_OBJECT
public:
	ProcessorThread();
	~ProcessorThread();
	
	/* Feed the thread what type of display setting the window is using */
	void setProcess(DisplaySetting setting);
	/* Feed the thread the touch-screen input data*/
	void setFingers(const QList<touch_data> &fingers);
	/* Load a new model for the purpose of loading  in your own OBJ models*/
	bool addOBJModel(string path);
	/* Returns the calculated results */
	QList<TAMShape *> getResults();

protected:
	void run() override;

private:
	QList<touch_data> finger_data;		/* touch-screen input data from the GLWindow*/
	QList<TAMShape *> results;			/* caulculated results	for the GLWindow to draw*/

	QList<TAMShape *> obj_models;		/* Stored OBJ Models to avoid re-opening model and re-calculate model data*/

	QMutex finger_m, result_m;				/* Necessary mutexes for this thread*/
	volatile DisplaySetting setting;		/* The display setting set by the user*/
	volatile bool running, update;			/* Tests for if the thread is running and needs to do re-calculation */

	int brushSize = 50;		/* The brush size set by the user [Currently just hard-coded]*/
	
	/* Functions used for intensive-calculations (based on display setting) */
	void operationFingerAveraging(const QList<touch_data> &input, QList<TAMShape *> &output);
	void operationShortestMapping(const QList<touch_data> &input, QList<TAMShape *> &output);
	void operationCircularConnection(const QList<touch_data> &input, QList<TAMShape *> &output);
	void operationShortestPath(const QList<touch_data> &input, QList<TAMShape *> &output);
};