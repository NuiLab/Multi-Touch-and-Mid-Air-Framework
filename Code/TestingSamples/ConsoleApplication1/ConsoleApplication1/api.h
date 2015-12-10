#include <glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <cmath>
#include <unordered_map>

namespace TAM {

	enum Position {NONE, TOP, BOT, LEFT, RIGHT};

	/* @TODO: Make a line shape for connection. How to compound primitives(put 2 or more triangles together). Allow right angle triangle to make shapes.
	 * Leap Motion running with API.
	 */

	/* Provides a way to map the coordinates a device to the coordinates needed by the API */
	class InputMapper
	{
	private:
		float x, y, z, w;
	public:
		InputMapper();
		InputMapper(void(*function)(float, float, float, float, float *, float *, float *, float *), float x, float y, float z, float w);
		inline float getXCoordinate();
		inline float getYCoordinate();
		inline float getZCoordinate();
		inline float getWCoordinate();
		void setXCoordinate(float x);
		void setYCoordinate(float y);
		void setZCoordinate(float z);
	};

	/* General Shape class to be used as base for any primitive. Currently using a cartesian coordinate system.
	 * @TODO: Add the compound and clear functions.
	 **/
	class Shape
	{
	public:
		virtual void draw(InputMapper In) = 0;
		//virtual void compound() = 0;
	};

	/* Provides a general way of drawing and visualizing a circle */ 
	class Circle : Shape
	{
	private:
		float r, g, b; // Color for the circle
		float radius;
	public:
		Circle(float rad, float r, float g, float b);
		void draw(InputMapper In) override;
	};

	/* Provides a general way of drawing and visualizing a triangle.
	   @TODO: Allow size. */
	class Triangle : Shape
	{
	private:
		float r, g, b; // Color for the triangle		
	public:
		Triangle(float r, float g, float b);
		void draw(InputMapper In) override;
	};

	/* Provides a general way of drawing and visualizing a Square.
	   @TODO: Allow size */
	class Square : Shape
	{
	private:
		float r, g, b; // Color for the point		
	public:
		Square(float r, float g, float b);
		void draw(InputMapper In) override;
	};

	/* Provides a general way of drawing and visualizing a Right Triangle */
	class RightTriangle : Shape
	{
	private:
		float r, g, b;
		float top, bot, left, right;
		float size;
		bool drawn;
		int vertical_flip;
	public:
		RightTriangle(float r, float g, float b, float size);
		void draw(InputMapper In) override;
		InputMapper compound(InputMapper In, Position pos, bool hflip, bool vflip); 
		void clear();
	};

	/* Provides a general way of drawing and visualizing a Line */
	class Line : Shape
	{
	private:
		/* Contains coordinates for the end points of the given Line*/
		struct Coordinates
		{
			float bot_x, bot_y, bot_z, top_x, top_y, top_z;
			bool operator!=(Coordinates x) 
			{
				if (bot_x != x.bot_x || bot_y != x.bot_y || bot_z != x.bot_z || top_x != x.top_x || top_y != x.top_y || top_z != x.top_z) {
					return true;
				}
				return false;
			}
		};				
		float r, g, b;
		float top, bot;
		float thick;
		float x, y, z;
		int drawn, element, old_element;
		bool back_mode;
		std::vector<Coordinates> vec;
		std::vector<Coordinates> old_vec;
	public:
		Line(float r, float g, float b, float thickness);
		void setEndPoint(float x, float y, float z);
		void draw(InputMapper In) override;
		void compound(Position pos);
		void goBack();
		void goForward();
		void restore();
		void connect(InputMapper In, InputMapper new_In); // Not tested yet
	};

	/* Class has a hashmap for generalizing all kinds of coordinates*/
	template<class K, class T>
	class DeviceData
	{
	public:
		std::unordered_map<K, T> input_data;
	};

	/* Generic class for holding all kinds of coordinates */
	template<class K, class T>
	class DeviceDataTime
	{
	public:
		std::list<DeviceData<K, T>> data;
	};

	template<class T>
	class Algorithms
	{

	public:
		void getFingerAveraging(int &avg_x, int &avg_y);
		std::list<T> getShortestMapping();
		std::list<T> getCircularConnection();
		std::list<T> getShortestPath();

	};

	/* Class designed to generalize the visualization part for any kind of device
	* it inherits from the TAMShape interface and implements its own generic draw function
	**/
	template<class K, class T>
	class OBJ : public TAMShape
	{
	private:
		DeviceDataTime<K, T> *coords;
	public:
		OBJ(DeviceDataTime<K, T> *arg);
		void draw(void(*function)(GLfloat, GLfloat), void(*drawing)(), GLfloat vertexbuffer[], GLfloat colorbuffer[]) override;
	};

	/*Initializes coordinates*/
	template<class K, class T>
	TAM::OBJ<K, T>::OBJ(DeviceDataTime<K, T> *arg) {
		coords = arg;

	}

	/* This function will perform the drawing for a selected device. It draws a fixed shape(cube, circle, or any shape)
	* by calling a function provided by a developer. It will then proceed to use the coordinates provided by the generic
	* coordinate class DeviceDataTime and thus not fully implemented until a design approval is received.
	* @TODO: Update translation and rotation to use DeviceDataTime as coordinates.(pending approval)
	***/
	template<class K, class T>
	void TAM::OBJ<K, T>::draw(void(*function)(GLfloat, GLfloat), void(*drawing)(), GLfloat vertexbuffer[], GLfloat colorbuffer[]) {
		GLuint VertexArrayID;
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);
		// Create and compile our GLSL program from the shaders
		GLuint programID = LoadShaders("C:/TransformVertexShader.vertexshader", "C:/ColorFragmentShader.fragmentshader");
		// Get a handle for our "MVP" uniform
		GLuint MatrixID = glGetUniformLocation(programID, "MVP");
		auto t_now = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
		// Camera matrix
		glm::mat4 View = glm::lookAt(
			glm::vec3(0, 0, -3), // Camera is at (4,3,-3), in World Space
			glm::vec3(0, 0, 0), // and looks at the origin
			glm::vec3(0, 1, 0) // Head is up (set to 0,-1,0 to look upside-down)
			);
		// Model matrix : an identity matrix (model will be at the origin)
		glm::mat4 Model = glm::mat4(1.0f);
		Model = glm::translate(Model, glm::vec3(1 / 3, -1 / 2, 0.0f));
		Model = glm::rotate(
			Model,
			time * glm::radians(180.0f),
			glm::vec3(1, 1, -GLSpace::v_near)
			);

		Model = glm::scale(Model, glm::vec3(1 / 10, 1 / 10, 1 / 10));
		// Our ModelViewProjection : multiplication of our 3 matrices
		glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

		/* Here vertices for the shape are created,	buffers are binded.
		* This is provided by the developer as a function.
		**/

		function(vertexbuffer, colorbuffer);

		GLuint vbuffer;
		glGenBuffers(1, &vbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexbuffer), vertexbuffer, GL_STATIC_DRAW);
		GLuint cbuffer;
		glGenBuffers(1, &cbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, cbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colorbuffer), colorbuffer, GL_STATIC_DRAW);
		glUseProgram(programID);

		// Send our transformation to the currently bound shader,
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		 //1rst attribute buffer : vertices

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
		glVertexAttribPointer(
			0, // attribute. No particular reason for 0, but must match the layout in the shader.
			3, // size
			GL_FLOAT, // type
			GL_FALSE, // normalized
			0, // stride
			(void*)0 // array buffer offset
			);

		 //2nd attribute buffer : colors

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, cbuffer);
		glVertexAttribPointer(
			1, // attribute. No particular reason for 1, but must match the layout in the shader.
			3, // size
			GL_FLOAT, // type
			GL_FALSE, // normalized?
			0, // stride
			(void*)0 // array buffer offset
			);

		/* This is also provided by the developer. This is where the actual drawing happens.*/

		drawing();

		 //Cleanup VBO and shader

		glDeleteBuffers(1, &vbuffer);
		glDeleteBuffers(1, &cbuffer);
		glDeleteProgram(programID);
		glDeleteVertexArrays(1, &VertexArrayID);

	}

	template<class T>
	class Playback
	{
	public:
		void save();
		static void start(std::list<T> &list, std::unordered_map<int, int> &hash, void(*function)(), void(T::* glupdate)());

	};

	template<class T>
	void TAM::Playback<T>::start(std::list<T> &list, std::unordered_map<int, int> &hash, void(*function)(), void(T::* glupdate)()) {

		//std::list<touch_data> touch_list = list;
		//std::unordered_map<int, int> fingers = hash;
		//int beginTime = clock();
		//int time;
		//int index = 0;

		//foreach(touch_data data, touch_list) {
		//	time = clock();
		//	if (data.time - (time - beginTime) > 0) {
		//		cout << " SLEEP FOR " << (data.time - (time - beginTime)) << endl;
		//		stringstream str;

		//		str << " SLEEP FOR " << (data.time - (time - beginTime));

		//		DebugWindow::println(str);

		//		// Sleep for the duration until the next touch-event happened

		//		QThread::yieldCurrentThread();

		//		qApp->processEvents();

		//		QThread::msleep(data.time - (time - beginTime));

		//	}

		//	// Update Data (without updating the touch_list being read)

		//	if (data.x < 0 || data.y < 0) {

		//		fingers.remove(data.id);

		//	}

		//	else {

		//		fingers[data.id] = index;

		//	}

		//	// Send data to proccess thread for calculation

		//	function();

		//	//sendDataToProcessThread();

		//	// Update current frame

		//	//glupdate();

		//	index++;

		//	// Stop when playback mode closes

		//	//if (!playback_mode) break;

		//}

	}

	/* Contains everything that is to be passed to the GL context */

	template<class T>
	class VisualizerHandler
	{

	public:
		static void initGL(void(*function)());
		static float resizeGL(float(*function)(int, int), int width, int height);
		static void paintGL(void(*function)(std::list<T *>), std::list<T *> res);

	};

	/*Initializes OpenGL context to default if no function has been passed*/

	template<class T>
	void TAM::VisualizerHandler<T>::initGL(void(*function)()) {

		if (function != NULL) 
		{
			function();
			return;
		}

		glClearColor(1.0, 1.0, 1.0, 0.0);

		glEnable(GL_DEPTH_TEST);

		glDepthFunc(GL_LESS);

		glEnable(GL_COLOR_MATERIAL);

		GLenum glew_status = glewInit();

		if (glew_status != GLEW_OK) 
		{

			std::cout << "Error: glewInit: " << glewGetErrorString(glew_status) << endl;

		}

	}

	/* Resize function for OpenGL context it will be initialized to default if no function has been passed */

	template<class T>

	float TAM::VisualizerHandler<T>::resizeGL(float(*function)(int, int), int width, int height) {

		if (function != NULL) {

			return function(width, height);

		}

		glViewport(0, 0, (GLint)width, (GLint)height);

		/* Create viewing cone with near and far clipping planes */

		float screen_ratio = (float)height / (float)width;

		glOrtho(-5, 5, -5.0f * screen_ratio, 5.0f * screen_ratio, 0.5, 30.0);	//To Switch to Orthogonal View (Not tested)

		return screen_ratio;

	}

	/* Handles drawing in OpenGL context. Receives a function to draw shapes. */

	template<class T>

	void TAM::VisualizerHandler<T>::paintGL(void(*function)(std::list<T *>), std::list<T *> res) {

		/*delete color and depth buffer*/

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		/* Draw results to screen */

		function(res);

	}

	//template<class T>

	//class Visualizer

	//{

	//private:

	//	ProcessorThread* process;

	//public:

	//	ProcessorThread* startProcess();

	//	void stopProcess();

	//	Coordinates coord;

	//	Algorithms<T> algo;

	//};

	///*Initializes the processor thread in order to receive input data.

	//Set custom to false to draw using one of the default shapes or true

	//to implement your own.*/

	//template<class T>

	//ProcessorThread* TAM::Visualizer<T>::startProcess()

	//{

	//	process = new ProcessorThread();

	//	return process;

	//}

	///* Deletes the process currently running */

	//template<class T>

	//void TAM::Visualizer<T>::stopProcess()

	//{

	//	delete process;

	//}

	/* This algorithms functions are pending approval
	* by mentor Dr. Ortega so they are not yet implemented
	**/

	/*Outputs average position of all vertices to x and y*/
	template<class T>
	void TAM::Algorithms<T>::getFingerAveraging(int &x, int &y) {

	}

	/*Outputs minimum spanning tree connection to QList*/

	template<class T>

	std::list<T> TAM::Algorithms<T>::getShortestMapping() {

		return NULL;

	}

	/*Outputs to QList Best-Fit for drawing a circle that comes closest as possible to circumscribing to all the vertices.*/

	template<class T>

	std::list<T> TAM::Algorithms<T>::getCircularConnection() {

		return NULL;

	}

	/*Outputs QList containing shortest Hamiltonian Path for all vertices */

	template<class T>

	std::list<T> TAM::Algorithms<T>::getShortestPath() {

		return NULL;

	}

}