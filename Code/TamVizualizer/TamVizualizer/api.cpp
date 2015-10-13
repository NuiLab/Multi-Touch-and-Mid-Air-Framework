#include "api.h"

/*Constructor initializes private variables to its respective value*/

void TAM::Coordinates::setThread(ProcessorThread *proc) {

	process = proc;

}

/*Receives screen width and height and returns the OpenGL coordinates of the shapes to be drawn*/

list<ShapeCoordinates> TAM::Coordinates::getGLCoordinates(int screen_width, int screen_height) {

	QList<TAMShape *> res = process->getResults();

	list<ShapeCoordinates> coordinate_list;

	foreach(TAMShape *shape, res) {

		foreach(ShapeCoordinates coord, shape->getCoordinates()) {

			int tmp_screen;

			tmp_screen = screen_width / 2;

			float x = (coord.getXCoordinate() - tmp_screen) / tmp_screen;

			tmp_screen = screen_height / 2;

			float y = -((coord.getYCoordinate() - tmp_screen) / tmp_screen);

			coordinate_list.push_back(ShapeCoordinates(x, y));

		}

	}

	return coordinate_list;

}

/*Returns a list of ShapeCoordinates with the pixel position of the shapes to be drawn */

list<ShapeCoordinates> TAM::Coordinates::getPixelCoordinates() {

	QList<TAMShape *> res = process->getResults();

	list<ShapeCoordinates> coordinate_list;

	foreach(TAMShape *shape, res) {

		foreach(ShapeCoordinates coord, shape->getCoordinates()) {

			coordinate_list.push_back(coord);

		}

	}

	return coordinate_list;

}

void TAM::Playback::start(QList<touch_data> &list, QHash<int, int> &hash, void(*function)(), void(QWidget::* glupdate)()) {

	QList<touch_data> touch_list = list;

	QHash<int, int> fingers = hash;

	int beginTime = clock();

	int time;

	int index = 0;

	foreach(touch_data data, touch_list) {

		time = clock();

		if (data.time - (time - beginTime) > 0) {

			qDebug() << " SLEEP FOR " << (data.time - (time - beginTime)) << endl;

			stringstream str;

			str << " SLEEP FOR " << (data.time - (time - beginTime));

			DebugWindow::println(str);

			// Sleep for the duration until the next touch-event happened

			QThread::yieldCurrentThread();

			qApp->processEvents();

			QThread::msleep(data.time - (time - beginTime));

		}

		// Update Data (without updating the touch_list being read)

		if (data.x < 0 || data.y < 0) {

			fingers.remove(data.id);

		}

		else {

			fingers[data.id] = index;

		}

		// Send data to proccess thread for calculation

		function();

		//sendDataToProcessThread();

		// Update current frame

		//glupdate();

		index++;

		// Stop when playback mode closes

		//if (!playback_mode) break;

	}

}