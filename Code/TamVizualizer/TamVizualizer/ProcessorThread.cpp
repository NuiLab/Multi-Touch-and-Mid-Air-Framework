#include "processorthread.h"

ProcessorThread::ProcessorThread(){
	running = true;
	start();
}

ProcessorThread::~ProcessorThread(){
	running = false;

	//update_m.lock();
	update = true;
	//update_m.unlock();
	wait();
}

void ProcessorThread::setProcess(DisplaySetting new_setting){
	setting = new_setting;
	//update_m.unlock();
}

void ProcessorThread::setFingers(const QList<touch_data> &fingers){
	finger_m.lock();
	finger_data.clear();
	foreach (touch_data data, fingers){
		finger_data << data;
	}
	finger_m.unlock();

	//update_m.unlock();
	update = true;
}

QList<TAMShape *> ProcessorThread::getResults(){
	result_m.lock();
	QList<TAMShape *> current_results;
	foreach(TAMShape * shape, results){
		current_results << shape;
	}
	result_m.unlock();

	return current_results;
}

void ProcessorThread::run(){
	float angular_pos = 0;

	while (running){
		if (!update) continue;
		// Fetch current data
		finger_m.lock();
		QList<touch_data> current_data;
		foreach(touch_data data, finger_data){
			current_data << data;
		}
		finger_m.unlock();
		QList<TAMShape *> current_result;

		/*
		QQuaternion q1(cos(angular_pos / 2.0f), sin(angular_pos / 2.0f), 0, 0);
		QQuaternion q2(cos(angular_pos), 0, sin(angular_pos), 0);
		QQuaternion q3(cos(angular_pos * 3 / 2.0f), 0, 0, sin(angular_pos * 3 / 2.0f));
		QQuaternion q = q1 * q2 * q3;
		angular_pos += 0.5;
		if (angular_pos > 360) angular_pos -= 360;*/
		QQuaternion q;
		current_result << new WorldBox(q, 50);

		if (setting != CUBE){
			//touch_data prev;
			//prev.id = -1;
			foreach(touch_data data, current_data){
				current_result << new Finger(data.x, data.y, brushSize, data.id);
				//if(prev.id > 0) current_result << new Line(data.x, data.y, prev.x, prev.y, 60);
				//prev = data;
			}
		} else {
			foreach(touch_data data, current_data){
				current_result << new SimpleCube(data.x, data.y, brushSize / 2.0f);
			}
		}

		switch (setting){
		case AVG:	// O(N)
			operationFingerAveraging(current_data, current_result);		break;
		/*{
			current_result << new SimpleCube(0, 0, 1);
			current_result << new Line(0, 0, 20, 20, 40, 100);
			current_result << new Circle(100, 100, 50, 20, false);

			if (current_data.size() < 2) break;

			// Best Fit Circle for all points (work-in-progress)
			int avgX = 0, avgY = 0, count = current_data.size();
			//float centerX = 0, centerY = 0, radius = 5, avgR = 0;

			foreach(touch_data data, current_data){
				avgX += data.x;
				avgY += data.y;
			}
			avgX /= count;
			avgY /= count;

			touch_data data;
			foreach(touch_data data, current_data){
				current_result << new Line(data.x, data.y, avgX, avgY, 60);
			}
			break;
		}*/
		case CIRCLE: 	// O(N^3)
			operationCircularConnection(current_data, current_result);	break;
		case MST: 	// O(N^2)
			operationShortestMapping(current_data, current_result);		break;
		case SHP: 	// O(N^2 * 2^N)
			operationShortestPath(current_data, current_result);		break;
		}

		// Update results
		result_m.lock();
		/*while (!results.isEmpty()){
			delete results.first();
		}*/
		results.clear();
		foreach(TAMShape* shape, current_result){
			results << shape;
		}
		result_m.unlock();

		//qDebug() << "Running . . ." << endl;

		// Hold until next update
		//update_m.lock();
		//while (update_m.tryLock());
		//update_m.unlock();
		update = false;
	}
	running = true;
}

void ProcessorThread::operationFingerAveraging(const QList<touch_data> &input, QList<TAMShape *> &output){
	if (input.size() < 2) return;

	// Best Fit Circle for all points (work-in-progress)
	int avgX = 0, avgY = 0, count = input.size();
	//float centerX = 0, centerY = 0, radius = 5, avgR = 0;

	foreach(touch_data data, input){
		avgX += data.x;
		avgY += data.y;
	}
	avgX /= count;
	avgY /= count;

	touch_data data;
	foreach(touch_data data, input){
		output << new Line(data.x, data.y, avgX, avgY, 60);
	}
}

void ProcessorThread::operationShortestMapping(const QList<touch_data> &input, QList<TAMShape *> &output){
	if (input.size() <= 1) return;

	// Finger Connecting Logic (M.S.T.)
	long distance, minI, minJ, minD;
	touch_data p1, p2;
	QList<int> removed, unselected;

	for (int index = 0; index < input.size(); index++){
		unselected << index;
	}

	removed << unselected.takeFirst();	// Random vertex
	while (!unselected.isEmpty()) {
		minD = 10000000L;
		foreach(int i, removed){
			p1 = input[i];

			foreach(int j, unselected) {
				p2 = input[j];
				distance = (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y);

				if (distance < minD) {
					minI = i;
					minJ = j;
					minD = distance;
				}
			}
		}

		p1 = input[minI];
		p2 = input[minJ];

		output << new Line(p1.x, p1.y, p2.x, p2.y, 50);

		unselected.removeOne(minJ);
		removed << minJ;
	}
}

void ProcessorThread::operationCircularConnection(const QList<touch_data> &input, QList<TAMShape *> &output){
	if (input.size() < 3) return;

	// Best Fit Circle for all points (work-in-progress)
	int avgX = 0, avgY = 0, count = 0;
	float centerX = 0, centerY = 0, radius = 5, avgR = 0;

	foreach(touch_data data1, input){
		foreach(touch_data data2, input){
			if (data1.id <= data2.id) continue;
			foreach(touch_data data3, input) {
				if (data2.id <= data3.id) continue;

				if (Algorithm::getCircumcenter(data1, data2, data3, 100, centerX, centerY, radius)) {
					avgX += centerX;
					avgY += centerY;
					avgR += radius;
					count++;
				}
			}
		}
	}

	if (count == 0) return;

	avgX /= count;
	avgY /= count;
	avgR /= count;

	// Get Circle
	output << new Circle(avgX, avgY, avgR, 100);

	foreach(touch_data data, input) {
		output << new Line(data.x, data.y, avgX, avgY, data.id);
	}
}

void ProcessorThread::operationShortestPath(const QList<touch_data> &input, QList<TAMShape *> &output){
	if (input.size() < 2) return;

	int count = input.size();
	touch_data node1, node2;
	QList< QList<int> > adj_matrix;
	QList<int> point_map;
	const int MAX = 1 << 30;
	int temp;// , m, n;

	//m = 0;
	for (int i = 0; i < input.size(); i++){
		node1 = input[i];

		QList<int> add;
		adj_matrix.append(add);

		//n = 0;
		for (int j = 0; j < input.size(); j++){
			if (i == j) {
				adj_matrix[i].append(MAX);
			} else if (i > j) {
				adj_matrix[i].append(adj_matrix[j][i]);
			} else {
				node2 = input[j];
				temp = (node1.x - node2.x)*(node1.x - node2.x) + (node1.y - node2.y)*(node1.y - node2.y);
				adj_matrix[i].append(temp);
			}
			//n++;
		}
		//m++;
	}

	/*qDebug() << "Made it this far with the following results:" << endl;
	QString message;
	foreach(QList<int> row, adj_matrix) {
	message = "";
	foreach(int col, row) {
	message += tr(std::to_string(col).c_str()) + ',';
	}
	qDebug() << message << endl;
	}*/

	int res;
	QList<int> order = Algorithm::getShortestHamiltonianPath(adj_matrix, res);


	/*qDebug() << tr(std::to_string(res).c_str()) << endl;

	qDebug() << "Made it FURTHER! With the following results:" << endl;
	message = "";
	foreach(int num, order){
	message += tr(std::to_string(num).c_str()) + ',';
	}
	qDebug() << message << endl;*/

	touch_data prev = input[order[0]];
	touch_data curr;
	for (int i = 1; i < order.size(); i++) {
		curr = input[order[i]];
		output << new Line(prev.x, prev.y, curr.x, curr.y, 70);
		prev = curr;
	}
}
