#include "processorthread.h"

/* Initialize and start the thread */
ProcessorThread::ProcessorThread(){
	running = true;
	start();
}

/* Stop the threading process*/
ProcessorThread::~ProcessorThread(){
	running = false;
	update = true;
	wait();
}

/* Feed the thread what type of display setting the window is using */
void ProcessorThread::setProcess(DisplaySetting new_setting){
	setting = new_setting;
}

void ProcessorThread::setBrushSize(int size){
	brushSize = size;
}

/* Feed the thread the touch-screen input data*/
void ProcessorThread::setFingers(const QList<touch_data> &fingers){
	finger_m.lock();
	finger_data.clear();

	foreach (touch_data data, fingers){
		if (data.x < 0 || data.y < 0) {
			finger_data.clear();
			finger_m.unlock();
			return;
		}

		finger_data << data;
	}
	finger_m.unlock();

	update = true;
}

/* Load a new model for the purpose of loading  in your own OBJ models */
bool ProcessorThread::addOBJModel(string path){
	OBJModel *model = new OBJModel(path);
	if (model->exists())
		obj_models << model;
	update = true;

	return model->exists();
}

/* Returns the calculated results to be drawn */
QList<TAMShape *> ProcessorThread::getResults(){
	result_m.lock();
	QList<TAMShape *> current_results;
	foreach(TAMShape * shape, results){
		current_results << shape;
	}
	result_m.unlock();

	return current_results;
}

/* The running process */
void ProcessorThread::run(){
	float angular_pos = 0;

	while (running) {
		// Don't proceed if there is no updated touch-screen input
		if (!update) continue;

		// Fetch and copy current touch-screen input data
		finger_m.lock();
		QList<touch_data> current_data;
		foreach(touch_data data, finger_data){
			current_data << data;
		}
		finger_m.unlock();

		// Variable to store the results to draw on scren
		QList<TAMShape *> current_result;

		// Testing the World Box
		//QQuaternion q;
		//current_result << new WorldBox(q, 20);

		// Draw all OBJ Models that have been loaded
		for (TAMShape* model : obj_models){
			current_result << model;
		}

		if (setting == CUBE){
			// Draw Cubes instead of Fingers
			// Used initially to display the power of OpenGL
			foreach(touch_data data, current_data){
				current_result << new SimpleCube(data.x, data.y, brushSize / 2.0f);
			}
		} else {
			// Draw finger-shapes for the touch-screen data recorded
			foreach(touch_data data, current_data){
				current_result << new Finger(data.x, data.y, brushSize, data.id);
			}
		}

		// Draw based on the display setting chosen by the user
		switch (setting){
		case AVG:	// O(N)
			operationFingerAveraging(current_data, current_result);		break;
		case CIRCLE: 	// O(N^3)
			operationCircularConnection(current_data, current_result);	break;
		case MST: 	// O(N^2)
			operationShortestMapping(current_data, current_result);		break;
		case SHP: 	// O(N^2 * 2^N)
			operationShortestPath(current_data, current_result);		break;
		}

		// Update results calculated
		result_m.lock();
		results.clear();
		foreach(TAMShape* shape, current_result){
			results << shape;
		}
		result_m.unlock();

		// Update Complete
		update = false;
	} // End-While
	running = true;
}

/* Function to calculate for the "Finger Averaging" Display Setting
   Time Complexity: O(N)
   Algorithm: Linearly scan to add all numbers and divide it by the total count
   This is a simple implementation of finding the average position of all vertices*/
void ProcessorThread::operationFingerAveraging(const QList<touch_data> &input, QList<TAMShape *> &output){
	if (input.size() < 2) return;
	int avgX = 0, avgY = 0, count = input.size();

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

/* Function to calculate for the "Shortest Finger Mapping" Display Setting
   Time Complexity: O(N^2)
   Algorithm: Prim's Algorithm using Naiive Implementation (Two For-loops)
   This is to find the Minimum Spanning Tree connection of the finger data*/
void ProcessorThread::operationShortestMapping(const QList<touch_data> &input, QList<TAMShape *> &output){
	if (input.size() <= 1) return;
	long distance, minI, minJ, minD;
	touch_data p1, p2;
	QList<int> selected, unselected;

	// Initialize unselected vertices
	for (int index = 0; index < input.size(); index++){
		unselected << index;
	}

	// Begin with a random vertex
	// Find shortest connection between a selected vertex and unselected vertex
	selected << unselected.takeFirst();	// Random vertex
	while (!unselected.isEmpty()) {
		minD = 10000000L;	// Initialize minimum distance to infinity (or an absurdly high number)
		foreach(int i, selected){
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

		// Get the two vertices
		p1 = input[minI];	// Get selected vertex
		p2 = input[minJ];	// Get unselected vertex

		// Draw a line to connect the shortest mapping found
		output << new Line(p1.x, p1.y, p2.x, p2.y, 50);

		unselected.removeOne(minJ);	// Remove from unselected
		selected << minJ;			// Add to selected
	}
}

/* Function to calculate for the "Circular Connection" Display Setting
   Time Complexity: O(N^3)
   Algorithm: Average of all mathematical computation of circumcenter for every unique 3-vertices pairing
   Aimed for a simple solution to finding the Best-Fit for drawing a circle that comes closest as possible
   to circumscribing (connecting) to all the vertices.*/
void ProcessorThread::operationCircularConnection(const QList<touch_data> &input, QList<TAMShape *> &output){
	/* Look at "Least Square Circle Fitting" to improve circle fitting algorithm performance:
		http://www.dtcenter.org/met/users/docs/write_ups/circle_fit.pdf */

	if (input.size() < 3) return;
	int avgX = 0, avgY = 0, count = 0;
	float centerX = 0, centerY = 0, radius = 5, avgR = 0;

	// Find 3 unique vertices pairing and add it's circumcenter to the total
	foreach(touch_data data1, input) {
		foreach(touch_data data2, input) {
			if (data1.id <= data2.id) continue;
			foreach(touch_data data3, input) {
				if (data2.id <= data3.id) continue;

				if (Algorithm::getCircumcenter(data1, data2, data3, 1000, centerX, centerY, radius)) {
					avgX += centerX;
					avgY += centerY;
					avgR += radius;
					count++;
				}
			}
		}
	}

	if (count == 0) return;	// No circle is good enough to fit to all vertices

	// Average up the results
	avgX /= count;
	avgY /= count;
	avgR /= count;

	// Draw Circumscribed Circle (circle that obtains a good enough circular connection)
	output << new Circle(avgX, avgY, avgR, 100);

	// Draw Lines to connect the fingers to the circumcenter
	foreach(touch_data data, input) {
		output << new Line(data.x, data.y, avgX, avgY, data.id);
	}
}

/* Function to calculate for the "Shortest Hamiltonion Path" Display Setting
   Time Complexity: O(N^2 * 2^N)
   Algorithm: See class Algorithm for more details.
   Solution to this problem is known to be NP-Complete, and therefore has a large computation time
   for even the smallest set of data. This display setting is set for the sole purpose of testing
   to see if computing in this thread will in any way slow down the UI thread. */
void ProcessorThread::operationShortestPath(const QList<touch_data> &input, QList<TAMShape *> &output){
	if (input.size() < 2) return;

	int count = input.size();
	touch_data node1, node2;
	QList< QList<int> > adj_matrix;
	QList<int> point_map;
	const int MAX = 1 << 30;
	int temp;

	// Convert touch-screen input data to an Adjacency Matrix for the algorithm to use
	for (int i = 0; i < input.size(); i++){
		node1 = input[i];

		QList<int> add;
		adj_matrix.append(add);

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
		}
	}

	// Calculate the Shortest Hamiltonian Path
	int res;
	QList<int> order = Algorithm::getShortestHamiltonianPath(adj_matrix, res);

	// Get the path from the algorithm's result
	touch_data prev = input[order[0]];
	touch_data curr;
	for (int i = 1; i < order.size(); i++) {
		curr = input[order[i]];

		// Draw Line of the Shortest Hamiltonian Path
		output << new Line(prev.x, prev.y, curr.x, curr.y, 70);

		prev = curr;
	}
}
