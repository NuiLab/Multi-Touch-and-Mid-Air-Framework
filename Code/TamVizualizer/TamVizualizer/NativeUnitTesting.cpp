#include "nativeunittesting.h""

float UnitTest::INF = INFINITY;
float UnitTest::NEG_INF = LOCALE_SNEGINFINITY;
float UnitTest::NaN = NAN;

bool frustumTest(){
	static float norm_x = 0, norm_y = 0, norm_z = 10, norm_length = 2;
	static float world_x, world_y, world_length;
	try{
		GLSpace::calculateScreenPosition(0, 0, norm_z, world_x, world_y);
		return false;
	}
	catch (...){}
	try{
		GLSpace::calculateScreenLength(norm_length, norm_z, world_length);
		return false;
	}
	catch (...){}
	return true;
}

bool testTAMShape(TAMShape *shape){
	try {
		shape->draw();
		return false;
	} catch (...){}
	return true;
}

bool compareLine(Line *line1, Line *line2){
	return ((line1->x1 == line2->x1 && line1->y1 == line2->y1 && line1->x2 == line2->x2 && line1->y2 == line2->y2)
		|| (line1->x1 == line2->x2 && line1->y1 == line2->y2 && line1->x2 == line2->x1 && line1->y2 == line2->y1));
}

bool compareLists(QList<TAMShape *> output, QList<TAMShape *> results){
	if (output.size() != results.size())
		return false;

	QList<int> indices;
	for (int i = 0; i < results.size(); i++) {
		indices.push_back(i);
	}

	bool found;
	for (TAMShape * line : output) {
		found = false;
		for (int i = 0; i < indices.size(); i++) {
			if (compareLine((Line *)line, (Line *)results[indices[i]])){
				indices.removeAt(i);
				found = true;
				break;
			}
		}
		if (!found) return false;
	}
	return true;
}

bool UnitTest::run(){
	/* =========== GLSpace ========== */

	// Test frustum() method #1 : Basic invalid input checks
	{
		float norm_half_width = 0.2, norm_half_height = 0.8, norm_near = 0.5, norm_far = 30;
		float checks[] = { 0, -1, INF, NEG_INF, NaN };

		for (float err_val : checks){
			// Width test
			GLSpace::frustum(err_val, norm_half_height, norm_near, norm_far);
			if (!frustumTest()) throw 0;

			// Height test
			GLSpace::frustum(norm_half_width, err_val, norm_near, norm_far);
			if (!frustumTest()) throw 0;

			// Near test
			GLSpace::frustum(norm_half_width, norm_half_height, err_val, norm_far);
			if (!frustumTest()) throw 0;

			// Far Test
			GLSpace::frustum(norm_half_width, norm_half_height, norm_near, err_val);
			if (!frustumTest()) throw 0;
		}

		// Invalid Face-Near test
		GLSpace::frustum(norm_half_width, norm_half_height, norm_far, norm_near);
		if (!frustumTest()) throw 0;

		GLSpace::frustum(norm_half_width, norm_half_height, norm_far, norm_far);
		if (!frustumTest()) throw 0;
	}

	// Test frustum() method #2 : Verified near input checks
	// Test frustum() method #3 : Verified width / height input checks
	{
		//TODO: MATHEMATICAL Calculations
	/*
		float half_width_input[] = { 1 };
		float half_height_input[] = { 1 };
		float view_near_input[] = { 1 };
		float view_far_input[] = { 1 };
		float screen_x_input[] = { 1 };
		float screen_y_input[] = { 1 };
		float screen_length_input[] = { 1 };
		float world_z_input[] = { 1 };
		float world_x_checks[] = { 1 };
		float world_y_checks[] = { 1 };
		float world_length_checks[] = { 1 };
		int size = sizeof(world_x_checks);

		float world_x, world_y, world_length;

		for (int i = 0; i < size; i++){
			GLSpace::frustum(half_width_input[i], half_height_input[i], view_far_input[i], view_near_input[i]);
			GLSpace::calculateScreenPosition(screen_x_input[i], screen_y_input[i], world_z_input[i], world_x, world_y);
			GLSpace::calculateScreenLength(screen_length_input[i], world_z_input[i], world_length);

			if (world_x != world_x_checks[i] || world_y != world_y_checks[i] || world_length != world_length_checks[i])
				throw 0;
		}
		*/
	}

	// Test screenSize() method #1 : Width / Height check
	{
		/*
		float width_input[] = { 1 };
		float height_input[] = { 1 };
		float screen_x_input[] = { 1 };
		float screen_y_input[] = { 1 };
		float screen_length_input[] = { 1 };
		float world_z_input[] = { 1 };
		float world_x_checks[] = { 1 };
		float world_y_checks[] = { 1 };
		float world_length_checks[] = { 1 };
		int size = sizeof(world_x_checks);

		float world_x, world_y, world_length;

		for (int i = 0; i < size; i++) {
			GLSpace::screenSize(width_input[i], height_input[i]);
			GLSpace::calculateScreenPosition(screen_x_input[i], screen_y_input[i], world_z_input[i], world_x, world_y);
			GLSpace::calculateScreenLength(screen_length_input[i], world_z_input[i], world_length);

			if (world_x != world_x_checks[i] || world_y != world_y_checks[i] || world_length != world_length_checks[i])
				throw 0;
		}
		*/
	}

	// Test Line Constructor #1 : Basic invalid input checks
	{
		float norm_x1 = 0, norm_y1 = 100, norm_x2 = 200, norm_y2 = 300, norm_thick = 50;
		int norm_color = 0;
		float checks[] = { 0, -1, INF, NEG_INF, NaN };

		for (float err_val : checks) {
			// X1 test
			if (!testTAMShape(new Line(err_val, norm_y1, norm_x2, norm_y2, norm_color, norm_thick))) throw 0;

			// Y1 test
			if (!testTAMShape(new Line(norm_x1, err_val, norm_x2, norm_y2, norm_color, norm_thick))) throw 0;

			// X2 test
			if (!testTAMShape(new Line(norm_x1, norm_y1, err_val, norm_y2, norm_color, norm_thick))) throw 0;

			// Y2 test
			if (!testTAMShape(new Line(norm_x1, norm_y1, norm_x2, err_val, norm_color, norm_thick))) throw 0;

			// Thick test
			if (!testTAMShape(new Line(norm_x1, norm_y1, norm_x2, norm_y2, norm_color, err_val))) throw 0;
		}

		// More thick test
		if (!testTAMShape(new Line(norm_x1, norm_y1, norm_x2, norm_y2, norm_color, 0))) throw 0;
		if (!testTAMShape(new Line(norm_x1, norm_y1, norm_x2, norm_y2, norm_color, -50))) throw 0;
	}

	// Test Line Constructor #2 : More invalid input checks
	{
		float norm_x = 100, norm_y = 300, norm_thick = 50;
		int norm_color = 0;
		if (!testTAMShape(new Line(norm_x, norm_y, norm_y, norm_y, norm_color, norm_thick))) throw 0;
	}

	// Test Circle Constructor #1 : Basic invalid input checks
	{
		float norm_x = 200, norm_y = 300, norm_radius = 100;
		int norm_color = 0;
		float checks[] = { 0, -1, INF, NEG_INF, NaN };

		for (float err_val : checks) {
			// X test
			if (!testTAMShape(new Circle(err_val, norm_y, norm_radius, norm_color, false))) throw 0;

			// Y test
			if (!testTAMShape(new Circle(norm_x, err_val, norm_radius, norm_color, false))) throw 0;

			// Radius test
			if (!testTAMShape(new Circle(norm_x, norm_y, err_val, norm_color, false))) throw 0;
		}

		// More radius test
		if (!testTAMShape(new Circle(norm_x, norm_y, 0, norm_color, false))) throw 0;
		if (!testTAMShape(new Circle(norm_x, norm_y, -300, norm_color, false))) throw 0;
	}

	// Test Finger Constructor #1 : Basic invalid input checks
	{
		float norm_x = 200, norm_y = 300, norm_size = 50;
		int norm_color = 0;
		float checks[] = { 0, -1, INF, NEG_INF, NaN };

		for (float err_val : checks) {
			// X test
			if (!testTAMShape(new Finger(err_val, norm_y, norm_size, norm_color))) throw 0;

			// Y test
			if (!testTAMShape(new Finger(norm_x, err_val, norm_size, norm_color))) throw 0;

			// Size test
			if (!testTAMShape(new Finger(norm_x, norm_y, err_val, norm_color))) throw 0;
		}

		// More size test
		if (!testTAMShape(new Finger(norm_x, norm_y, 0, norm_color))) throw 0;
		if (!testTAMShape(new Finger(norm_x, norm_y, -50, norm_color))) throw 0;
	}

	// Test SimpleCube Constructor #1 : Basic invalid input checks
	{
		float norm_x = 200, norm_y = 300, norm_size = 50;
		float checks[] = { 0, -1, INF, NEG_INF, NaN };

		for (float err_val : checks) {
			// X test
			if (!testTAMShape(new SimpleCube(err_val, norm_y, norm_size))) throw 0;

			// Y test
			if (!testTAMShape(new SimpleCube(norm_x, err_val, norm_size))) throw 0;

			// Size test
			if (!testTAMShape(new SimpleCube(norm_x, norm_y, err_val))) throw 0;
		}

		// More size test
		if (!testTAMShape(new SimpleCube(norm_x, norm_y, 0))) throw 0;
		if (!testTAMShape(new SimpleCube(norm_x, norm_y, -50))) throw 0;
	}



	/* ========== Algorithm ========= */

	// Test getCircumcenter method #1 : Test same answers for swap of values
	{
		float centerX = -1, centerY = -1, radius = -1;
		float answerX, answerY, answerRadius;
		touch_data data1, data2, data3;
		srand(time(NULL));

		data1.x = rand() % 100;	data1.y = rand() % 300;	data1.time = 0;	data1.id = 0;
		data2.x = rand() % 100;	data2.y = rand() % 300;	data2.time = 0;	data2.id = 0;
		data3.x = rand() % 100;	data3.y = rand() % 300;	data3.time = 0;	data3.id = 0;

		Algorithm::getCircumcenter(data1, data2, data3, 1000, answerX, answerY, answerRadius);

		Algorithm::getCircumcenter(data1, data3, data2, 1000, centerX, centerY, radius);
		if (answerX != centerX || answerY != centerY || answerRadius != radius) throw 0;

		Algorithm::getCircumcenter(data2, data1, data3, 1000, centerX, centerY, radius);
		if (answerX != centerX || answerY != centerY || answerRadius != radius) throw 0;

		Algorithm::getCircumcenter(data2, data3, data1, 1000, centerX, centerY, radius);
		if (answerX != centerX || answerY != centerY || answerRadius != radius) throw 0;

		Algorithm::getCircumcenter(data3, data1, data2, 1000, centerX, centerY, radius);
		if (answerX != centerX || answerY != centerY || answerRadius != radius) throw 0;

		Algorithm::getCircumcenter(data3, data2, data1, 1000, centerX, centerY, radius);
		if (answerX != centerX || answerY != centerY || answerRadius != radius) throw 0;
	}

	// Test getCircumcenter method #2 : Verified input checks
	{
		float centerX = -1, centerY = -1, radius = -1;

		vector<touch_data> data1_input;
		vector<touch_data> data2_input;
		vector<touch_data> data3_input;
		vector<float> centerX_checks;
		vector<float> centerY_checks;
		vector<float> radius_checks;
		touch_data data1, data2, data3;
		int test_num = 0;

		// Initialize
		data1.time = 0;	data1.id = 0;
		data2.time = 0;	data2.id = 0;
		data3.time = 0;	data3.id = 0;

		// Input 1 : Three same points
		// Fails result due to: No radius for circumcircle
		data1.x = 100;	data1.y = 100;	
		data2.x = 100;	data2.y = 100;
		data3.x = 100;	data3.y = 100;
		data1_input.push_back(data1);
		data2_input.push_back(data2);
		data3_input.push_back(data3);
		centerX_checks.push_back(-1);
		centerY_checks.push_back(-1);
		radius_checks.push_back(-1);
		test_num++;

		// Input 2 : Three points in a line
		// Fails result due to: No center for circumcircle
		data1.x = 0;	data1.y = 0;
		data2.x = 50;	data2.y = 50;
		data3.x = 100;	data3.y = 100;
		data1_input.push_back(data1);
		data2_input.push_back(data2);
		data3_input.push_back(data3);
		centerX_checks.push_back(-1);
		centerY_checks.push_back(-1);
		radius_checks.push_back(-1);
		test_num++;

		// Input 3 : Two points in a line (two of three are the same point)
		// Fails result due to: Infinitely many circumcenters
		data1.x = 50;	data1.y = 0;
		data2.x = 50;	data2.y = 100;
		data3.x = 50;	data3.y = 100;
		data1_input.push_back(data1);
		data2_input.push_back(data2);
		data3_input.push_back(data3);
		centerX_checks.push_back(-1);
		centerY_checks.push_back(-1);
		radius_checks.push_back(-1);
		test_num++;

		// Input 4 : Right Triangle
		// Info for test: http://jwilson.coe.uga.edu/emat6680fa05/evans/assignment%204/assignment%204.htm
		data1.x = 0;	data1.y = 0;
		data2.x = 0;	data2.y = 30;
		data3.x = 40;	data3.y = 0;
		data1_input.push_back(data1);
		data2_input.push_back(data2);
		data3_input.push_back(data3);
		centerX_checks.push_back(20);
		centerY_checks.push_back(15);
		radius_checks.push_back(25);
		test_num++;

		// Input 5 : Equilateral Triangle
		//Info for test: http://geometryatlas.com/entries/43
		//				 https://www.easycalculation.com/analytical/circumcenter-triangle-calculator.php
		data1.x = 30;	data1.y = 0;
		data2.x = 0;	data2.y = 60;
		data3.x = 60;	data3.y = 60;
		data1_input.push_back(data1);
		data2_input.push_back(data2);
		data3_input.push_back(data3);
		centerX_checks.push_back(30);
		centerY_checks.push_back(37.5);
		radius_checks.push_back(37.5);
		test_num++;

		// Input 6 : Obtuse Triangle
		// Info for test: https://www.easycalculation.com/analytical/circumcenter-triangle-calculator.php
		data1.x = 0;	data1.y = 50;
		data2.x = 30;	data2.y = 0;
		data3.x = 10;	data3.y = 20;
		data1_input.push_back(data1);
		data2_input.push_back(data2);
		data3_input.push_back(data3);
		centerX_checks.push_back(65);
		centerY_checks.push_back(55);
		radius_checks.push_back(sqrt(4225 + 25));	// sqrt((65 - 0)^2 + (55 - 50)^2)
		test_num++;


		// Input 7 : Acute Triangle
		// Info for test: https://www.easycalculation.com/analytical/circumcenter-triangle-calculator.php
		data1.x = 10;	data1.y = 50;
		data2.x = 30;	data2.y = 30;
		data3.x = 0;	data3.y = 10;
		data1_input.push_back(data1);
		data2_input.push_back(data2);
		data3_input.push_back(data3);
		centerX_checks.push_back(9);
		centerY_checks.push_back(29);
		radius_checks.push_back(sqrt(81 + 361)); // sqrt((9 - 0)^2 + (29 - 10)^2)
		test_num++;


		// Test All Case
		for (int i = 0; i < test_num; i++) {
			Algorithm::getCircumcenter(data1_input[i], data2_input[i], data3_input[i], 1000, centerX, centerY, radius);
			if (centerX != centerX_checks[i] || centerY != centerY_checks[i] || radius != radius_checks[i]) throw 0;
		}
	}


	/* ======= ProcessorThread ====== */

	// Test operationShortestPath method #1 : Basic input checks
	{
		QList<touch_data> input;
		QList<TAMShape *> output;
		touch_data data;
		int res;

		//Empty
		ProcessorThread::operationShortestPath(input, output);
		if (!output.isEmpty()) throw 0;

		// One
		data.x = 0;	data.y = 0;	data.time = 0;	data.id = 0;
		input.push_back(data);
		ProcessorThread::operationShortestPath(input, output);
		if (!output.isEmpty()) throw 0;
	}

	// Test operationShortestPath method #2 : Test same answer for swap of vertices
	{
		QList<touch_data> input;
		QList<TAMShape *> output;
		QList<TAMShape *> results;

		touch_data data1, data2, data3;
		int res;
		srand(time(NULL));

		//data1.x = rand() % 100;	data1.y = rand() % 300;	data1.time = 0;	data1.id = 0;
		//data2.x = rand() % 100;	data2.y = rand() % 300;	data2.time = 0;	data2.id = 0;
		//data3.x = rand() % 100;	data3.y = rand() % 300;	data3.time = 0;	data3.id = 0;

		// Example values
		data1.x = 41;	data1.y = 293;	data1.time = 0;	data1.id = 0;
		data2.x = 3;	data2.y = 183;	data2.time = 0;	data2.id = 0;
		data3.x = 8;	data3.y = 287;	data3.time = 0;	data3.id = 0;

		float data1todata2 = sqrt((data1.x - data2.x)*(data1.x - data2.x) + (data1.y - data2.y)*(data1.y - data2.y));
		float data2todata3 = sqrt((data2.x - data3.x)*(data2.x - data3.x) + (data2.y - data3.y)*(data2.y - data3.y));
		float data3todata1 = sqrt((data3.x - data1.x)*(data3.x - data1.x) + (data3.y - data1.y)*(data3.y - data1.y));

		// Testing purposes
		if (data1todata2 > data2todata3){
			results.append(new Line(data2.x, data2.y, data3.x, data3.y, 70, 40));
			if (data1todata2 > data3todata1)
				 results.append(new Line(data3.x, data3.y, data1.x, data1.y, 70, 40));
			else results.append(new Line(data1.x, data1.y, data2.x, data2.y, 70, 40));
		}
		else {
			results.append(new Line(data1.x, data1.y, data2.x, data2.y, 70, 40));
			if (data2todata3 > data3todata1)
				 results.append(new Line(data1.x, data1.y, data2.x, data2.y, 70, 40));
			else results.append(new Line(data2.x, data2.y, data3.x, data3.y, 70, 40));
		}
		

		// Iteration 1-2-3
		input.push_back(data1);	input.push_back(data2);	input.push_back(data3);
		ProcessorThread::operationShortestPath(input, output);
		if (!compareLists(output, results)) throw 0;
		input.clear();
		output.clear();

		// Iteration 1-3-2
		input.push_back(data1);	input.push_back(data3);	input.push_back(data2);
		ProcessorThread::operationShortestPath(input, output);
		if (!compareLists(output, results)) throw 0;
		input.clear();
		output.clear();

		// Iteration 2-1-3
		input.push_back(data2);	input.push_back(data1);	input.push_back(data3);
		ProcessorThread::operationShortestPath(input, output);
		if (!compareLists(output, results)) throw 0;
		input.clear();
		output.clear();

		// Iteration 2-3-1
		input.push_back(data2);	input.push_back(data3);	input.push_back(data1);
		ProcessorThread::operationShortestPath(input, output);
		if (!compareLists(output, results)) throw 0;
		input.clear();
		output.clear();

		// Iteration 3-1-2
		input.push_back(data3);	input.push_back(data1);	input.push_back(data2);
		ProcessorThread::operationShortestPath(input, output);
		if (!compareLists(output, results)) throw 0;
		input.clear();
		output.clear();

		// Iteration 3-2-1
		input.push_back(data3);	input.push_back(data2);	input.push_back(data1);
		ProcessorThread::operationShortestPath(input, output);
		if (!compareLists(output, results)) throw 0;
	}

	// Test getShortestHamiltonianPath method #3 : Verified input checks
	{
		QList<touch_data> input;
		QList<TAMShape *> output;
		QList<TAMShape *> results;
		touch_data data;
		int res;

		// Input 1 : Basic Two Vertices Graph
		data.x = 0;	data.y = 0;	data.time = 0;	data.id = 0;	input.push_back(data);
		data.x = 1;	data.y = 1;	data.time = 0;	data.id = 0;	input.push_back(data);
		results.push_back(new Line(0, 0, 1, 1, 70));
		ProcessorThread::operationShortestPath(input, output);
		if (!compareLists(output, results)) throw 0;
		results.clear();
		input.clear();
		output.clear();


		// Input 2 : Basic Three Vertices Graph (Simple Right Triangle of 3 : 4 : 5 side ratio)
		data.x = 60;	data.y = 60;	data.time = 0;	data.id = 0;	input.push_back(data);
		data.x = 20;	data.y = 30;	data.time = 0;	data.id = 0;	input.push_back(data);
		data.x = 60;	data.y = 30;	data.time = 0;	data.id = 0;	input.push_back(data);
		results.push_back(new Line(20, 30, 60, 30, 70));
		results.push_back(new Line(60, 30, 60, 60, 70));
		ProcessorThread::operationShortestPath(input, output);
		if (!compareLists(output, results)) throw 0;
		results.clear();
		input.clear();
		output.clear();
	}

	// Test getShortestHamiltonianPath method #1 : Verified input checks
	{
		// Input 3 :  Difficult 15 vertices solution
		// Application used for testing: http://tspsg.info/en
		/*	========================= ADJACENCY MATRIX =========================
			__|	C00	C01	C02	C03	C04	C05	C06	C07	C08	C09	C10	C11	C12	C13	C14	CXX
			C00	---	298	307	499	191	498	286	278	225	442	237	428	232	429	483	1000
			C01	298 ---	186	102	345	404	228	140	261	350	498 344	151	173	380	1000
			C02	307	186	---	300	360	138	440	500	368	464	277	184	146	111	404	1000
			C03	499	102	300	---	396	134	481	136	494	162	388	219	481	470	173	1000
			C04	191	345	360	396	---	101	134	433	117	489	120	302	367	418	183	1000
			C05	498	404	138	134	101	---	224	471	215	429	101	332	268	257	298	1000
			C06	286	228	440	481	134	224	---	145	304	470	306	269	203	423	238	1000
			C07	278	140	500	136	433	471	145	---	429	279	396	386	346	497	397	1000
			C08	225	261	368	494	117	215	304	429	---	317	357	412	477	301	479	1000
			C09	442	350	464	162	489	429	470	279	317	---	151	123	197	174	473	1000
			C10	237	498	277	388	120	101	306	396	357	151	---	273	334	403	221	1000
			C11	428	344	184	219	302	332	269	386	412	123	273	---	129	472	219	1000
			C12	232	151	146	481	367	268	203	346	477	197	334	129	---	491	211	1000
			C13	429	173	111	470	418	257	423	497	301	174	403	472	491	---	161	1000
			C14	483	380	404	173	183	298	238	397	479	473	221	219	211	161	---	1000
			CXX	100010001000100010001000100010001000100010001000100010001000 ---
		*/
		/*	Answer:	City XX -> City 15 -> City 14 -> City 03 -> City 06 -> City 11 ->
					City 10 -> City 12 -> City 13 -> City 02 -> City 04 -> City 08 ->
					City 07 -> City 05 -> City 09 -> City 01 -> City XX
			Total Distance in Shortest Ham. Cycle	= 3924 (includes +2000 for the two City XX path)
			Total Distance in Shortest Ham. Path	= 1924*/

		QList<QList<int>> adj_matrix;
		QList<int> row[15];
		QList<int> result, answer;
		int i = 0;
		int res;
		//	C00	---	298	307	499	191	498	286	278	225	442	237	428	232	429	483	1000
		row[i].push_back(0);	row[i].push_back(298); row[i].push_back(307);	row[i].push_back(499);	row[i].push_back(191);
		row[i].push_back(498);	row[i].push_back(286);	row[i].push_back(278);	row[i].push_back(225);	row[i].push_back(442);
		row[i].push_back(237);	row[i].push_back(428);	row[i].push_back(232);	row[i].push_back(429);	row[i].push_back(483);
		i++;
		//	C01	298 ---	186	102	345	404	228	140	261	350	498 344	151	173	380	1000
		row[i].push_back(298);	row[i].push_back(0); row[i].push_back(186);	row[i].push_back(102);	row[i].push_back(345);
		row[i].push_back(404);	row[i].push_back(228);	row[i].push_back(140);	row[i].push_back(261);	row[i].push_back(350);
		row[i].push_back(498);	row[i].push_back(344);	row[i].push_back(151);	row[i].push_back(173);	row[i].push_back(380);
		i++;
		//	C02	307	186	---	300	360	138	440	500	368	464	277	184	146	111	404	1000
		row[i].push_back(307);	row[i].push_back(186); row[i].push_back(0);	row[i].push_back(300);	row[i].push_back(360);
		row[i].push_back(138);	row[i].push_back(440);	row[i].push_back(500);	row[i].push_back(368);	row[i].push_back(464);
		row[i].push_back(277);	row[i].push_back(184);	row[i].push_back(146);	row[i].push_back(111);	row[i].push_back(404);
		i++;
		//	C03	499	102	300	---	396	134	481	136	494	162	388	219	481	470	173	1000
		row[i].push_back(499);	row[i].push_back(102); row[i].push_back(300);	row[i].push_back(0);	row[i].push_back(396);
		row[i].push_back(134);	row[i].push_back(481);	row[i].push_back(136);	row[i].push_back(494);	row[i].push_back(162);
		row[i].push_back(388);	row[i].push_back(219);	row[i].push_back(481);	row[i].push_back(470);	row[i].push_back(173);
		i++;
		//	C04	191	345	360	396	---	101	134	433	117	489	120	302	367	418	183	1000
		row[i].push_back(191);	row[i].push_back(345); row[i].push_back(360);	row[i].push_back(396);	row[i].push_back(0);
		row[i].push_back(101);	row[i].push_back(134);	row[i].push_back(433);	row[i].push_back(117);	row[i].push_back(489);
		row[i].push_back(120);	row[i].push_back(302);	row[i].push_back(367);	row[i].push_back(418);	row[i].push_back(183);
		i++;
		//	C05	498	404	138	134	101	---	224	471	215	429	101	332	268	257	298	1000
		row[i].push_back(498);	row[i].push_back(404); row[i].push_back(138); row[i].push_back(134);	row[i].push_back(101);
		row[i].push_back(0);	row[i].push_back(224);	row[i].push_back(471);	row[i].push_back(215);	row[i].push_back(429);
		row[i].push_back(101);	row[i].push_back(332);	row[i].push_back(268);	row[i].push_back(257);	row[i].push_back(298);
		i++;
		//	C06	286	228	440	481	134	224	---	145	304	470	306	269	203	423	238	1000
		row[i].push_back(286);	row[i].push_back(228); row[i].push_back(440);	row[i].push_back(481);	row[i].push_back(134);
		row[i].push_back(224);	row[i].push_back(0);	row[i].push_back(145);	row[i].push_back(304);	row[i].push_back(470);
		row[i].push_back(306);	row[i].push_back(269);	row[i].push_back(203);	row[i].push_back(423);	row[i].push_back(238);
		i++;
		//	C07	278	140	500	136	433	471	145	---	429	279	396	386	346	497	397	1000
		row[i].push_back(278);	row[i].push_back(140); row[i].push_back(500);	row[i].push_back(136);	row[i].push_back(433);
		row[i].push_back(471);	row[i].push_back(145);	row[i].push_back(0);	row[i].push_back(429);	row[i].push_back(279);
		row[i].push_back(396);	row[i].push_back(386);	row[i].push_back(346);	row[i].push_back(497);	row[i].push_back(397);
		i++;
		//	C08	225	261	368	494	117	215	304	429	---	317	357	412	477	301	479	1000
		row[i].push_back(225);	row[i].push_back(261); row[i].push_back(368);	row[i].push_back(494);	row[i].push_back(117);
		row[i].push_back(215);	row[i].push_back(304);	row[i].push_back(429);	row[i].push_back(0);	row[i].push_back(317);
		row[i].push_back(357);	row[i].push_back(412);	row[i].push_back(477);	row[i].push_back(301);	row[i].push_back(479);
		i++;
		//	C09	442	350	464	162	489	429	470	279	317	---	151	123	197	174	473	1000
		row[i].push_back(442);	row[i].push_back(350); row[i].push_back(464);	row[i].push_back(162);	row[i].push_back(489);
		row[i].push_back(429);	row[i].push_back(470);	row[i].push_back(279);	row[i].push_back(317);	row[i].push_back(0);
		row[i].push_back(151);	row[i].push_back(123);	row[i].push_back(197);	row[i].push_back(174);	row[i].push_back(473);
		i++;
		//	C10	237	498	277	388	120	101	306	396	357	151	---	273	334	403	221	1000
		row[i].push_back(237);	row[i].push_back(498); row[i].push_back(277);	row[i].push_back(388);	row[i].push_back(120);
		row[i].push_back(101);	row[i].push_back(306);	row[i].push_back(396);	row[i].push_back(357);	row[i].push_back(151);
		row[i].push_back(0);	row[i].push_back(273);	row[i].push_back(334);	row[i].push_back(403);	row[i].push_back(221);
		i++;
		//	C11	428	344	184	219	302	332	269	386	412	123	273	---	129	472	219	1000
		row[i].push_back(428);	row[i].push_back(344); row[i].push_back(184);	row[i].push_back(219);	row[i].push_back(302);
		row[i].push_back(332);	row[i].push_back(269);	row[i].push_back(386);	row[i].push_back(412);	row[i].push_back(123);
		row[i].push_back(273);	row[i].push_back(0);	row[i].push_back(129);	row[i].push_back(472);	row[i].push_back(219);
		i++;
		//	C12	232	151	146	481	367	268	203	346	477	197	334	129	---	491	211	1000
		row[i].push_back(232);	row[i].push_back(151); row[i].push_back(146);	row[i].push_back(481);	row[i].push_back(367);
		row[i].push_back(268);	row[i].push_back(203);	row[i].push_back(346);	row[i].push_back(477);	row[i].push_back(197);
		row[i].push_back(334);	row[i].push_back(129);	row[i].push_back(0);	row[i].push_back(491);	row[i].push_back(211);
		i++;
		//	C13	429	173	111	470	418	257	423	497	301	174	403	472	491	---	161	1000
		row[i].push_back(429);	row[i].push_back(173);	row[i].push_back(111);	row[i].push_back(470);	row[i].push_back(418);
		row[i].push_back(257);	row[i].push_back(423);	row[i].push_back(497);	row[i].push_back(301);	row[i].push_back(174);
		row[i].push_back(403);	row[i].push_back(472);	row[i].push_back(491);	row[i].push_back(0);	row[i].push_back(161);
		i++;
		//	C14	483	380	404	173	183	298	238	397	479	473	221	219	211	161	---	1000
		row[i].push_back(483);	row[i].push_back(380);	row[i].push_back(404);	row[i].push_back(173);	row[i].push_back(183);
		row[i].push_back(298);	row[i].push_back(238);	row[i].push_back(397);	row[i].push_back(479);	row[i].push_back(473);
		row[i].push_back(221);	row[i].push_back(219);	row[i].push_back(211);	row[i].push_back(161);	row[i].push_back(0);
		i++;
		//	CXX	100010001000100010001000100010001000100010001000100010001000 ---
		// City XX is excluded, since it is used for Hamiltonian Cycle (removing XX produces Hamiltonian Cycle

		// Answer:	 City XX -> City 14 -> City 13 -> City 02 -> City 05 -> City 10 ->
		//						City 09 -> City 11 -> City 12 -> City 01 -> City 03 ->
		//						City 07 -> City 06 -> City 04 -> City 08 -> City 00 -> City XX
		answer.push_back(14);	answer.push_back(13);	answer.push_back(2);	answer.push_back(5);	answer.push_back(10);
		answer.push_back(9);	answer.push_back(11);	answer.push_back(12);	answer.push_back(1);	answer.push_back(3);
		answer.push_back(7);	answer.push_back(6);	answer.push_back(4);	answer.push_back(8);	answer.push_back(0);

		for (QList<int> r : row){
			adj_matrix.append(r);
		}

		result = Algorithm::getShortestHamiltonianPath(adj_matrix, res);
		
		if (result.size() != answer.size()) throw 0;
		if (res >= 1924) throw 0; // shortest distance must be smaller or equal to based on best heuristic data found online
		/*for (int i = 0; i < answer.size(); i++){
			if (result[i] != answer[i]) throw 0;
		}*/
	}

	// Test setFingers method #1 : Basic invalid input checks
	{
		ProcessorThread *proc;
		QList<touch_data> empty, invalid1, invalid2, valid;
		QList<TAMShape *> result;
		touch_data data;
		int time, time_limit = 100000;
		
		data.x = 0;	data.y = 0; data.time = 0; data.id = 0;
		valid.push_back(data);
		data.x = -1;	data.y = 0; data.time = 0; data.id = 0;
		invalid1.push_back(data);
		data.x = 0;	data.y = -1; data.time = 0; data.id = 0;
		invalid2.push_back(data);
		proc = new ProcessorThread();
		proc->setFingers(valid);
		for (time = 0; time < time_limit; time++){}

		// Empty
		proc->setFingers(empty);
		result = proc->getResults();
		for (time = 0; time < time_limit && !result.isEmpty(); time++) {
			result = proc->getResults();
		}
		if (time == time_limit) throw 0;

		// Reset
		proc->setFingers(valid);
		for (time = 0; time < time_limit; time++){}

		// Invalid X Data
		proc->setFingers(invalid1);
		for (time = 0; time < time_limit; time++) {}
		result = proc->getResults();
		if (!result.isEmpty()) throw 0;

		// Reset
		proc->setFingers(valid);
		for (time = 0; time < time_limit; time++){}

		// Invalid Y Data
		proc->setFingers(invalid2);
		for (time = 0; time < time_limit; time++) {}
		result = proc->getResults();
		if (!result.isEmpty()) throw 0;

		delete proc;
	}

	// ======== ALL TESTS PASSED ========
	return true;
}