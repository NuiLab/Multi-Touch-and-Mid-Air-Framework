#include "api.h"

/* Empty Constructor */
TAM::InputMapper::InputMapper() {

}

/* Pass a function to map the coordinates a given device to the coordinates needed by the API.
* The function must return mapped values in its last four parameters and the values must be between -1 and 1.
**/
TAM::InputMapper::InputMapper(void(*function)(float, float, float, float, float *, float *, float *, float *), float x, float y, float z, float w) {
	function(x, y, z, w, &this->x, &this->y, &this->z, &this->w);
	assert(this->x <= 1 || this->x >= -1);
	assert(this->y <= 1 || this->y >= -1);
	assert(this->z <= 1 || this->z >= -1);
	assert(this->w <= 1 || this->w >= -1);
}

float TAM::InputMapper::getWCoordinate()
{
	return w;
}

/* Returns x coordinate */
inline float TAM::InputMapper::getXCoordinate() {
	return x;
}

/* Returns y coordinate */
inline float TAM::InputMapper::getYCoordinate() {
	return y;
}

/* Returns z coordinate */
inline float TAM::InputMapper::getZCoordinate() {
	return z;
}

/* Sets x coordinate */
void TAM::InputMapper::setXCoordinate(float x) {
	this->x = x;
}

/* Sets y coordinate */
void TAM::InputMapper::setYCoordinate(float y) {
	this->y = y;
}

/* Sets z coordinate */
void TAM::InputMapper::setZCoordinate(float z) {
	this->z = z;
}

/* Sets w coordinate */
void TAM::InputMapper::setWCoordinate(float z) {
	this->w = w;
}

/* Static private members of Helpers */
int TAM::Helpers::screen_width;
int TAM::Helpers::screen_height;

/* Set the screen size value on construction */
TAM::Helpers::Helpers(int width, int height)
{
	screen_width = width;
	screen_height = height;
}

/* If screen size changes after the object construction then
 * this function must be called before calling the mapper functions
 * otherwise undesired results will be returned.
 * */
void TAM::Helpers::setScreenSize(int width, int height)
{
	screen_width = width;
	screen_height = height;
}

/* Loads and compiles shaders from files */
GLuint TAM::Helpers::LoadShaders(const char * vertex_file_path, const char * fragment_file_path){

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()){
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else{
		std::cout << "Impossible to open filesn\n";
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()){
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}



	GLint Result = GL_FALSE;
	int InfoLogLength;



	// Compile Vertex Shader
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		std::cout << &VertexShaderErrorMessage[0] << std::endl;
	}



	// Compile Fragment Shader
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		std::cout << &FragmentShaderErrorMessage[0] << std::endl;
	}



	// Link the program
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		std::cout << &ProgramErrorMessage[0] << std::endl;
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

/* Randomly generates values for r g b */
void TAM::Helpers::generateColor(int id, float &r, float &g, float &b)
{
	r = (((id + 1) * 07) % 19) / 20.0f;
	g = (((id + 3) * 11) % 19) / 20.0f;
	b = (((id + 5) * 13) % 19) / 20.0f;
}

/* Sets values for the radius and color on construction. If fill is set to true a filled circle will be drawn. */
TAM::Circle::Circle(float rad, float r, float g, float b, float t, bool fill) {
	radius = rad;
	this->r = r;
	this->g = g;
	this->b = b;
	this->t = t;
	this->fill = fill;
}

/* Renders a circle into the opengl context */
void TAM::Circle::draw(InputMapper In) {
	static const GLfloat twicePi = 2.0f * atan(1) * 4;
	float centerZ = -In.getZCoordinate();

	std::string frag =
		"#version 150 core\n"
		"out vec4 outColor;"
		"void main() {"
		"   outColor = vec4(" + std::to_string(r) + "," + std::to_string(g) + "," + std::to_string(b) + "," + std::to_string(t) + ");"
		"}";

	const GLchar* vertexSource =
		"#version 150 core\n"
		"in vec2 position;"
		"uniform mat4 MVP;"
		"void main() {"
		"   gl_Position = vec4(position, 0.0, 1.0);"
		"}";

	const GLchar* fragmentSource = frag.c_str();

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create a Vertex Buffer Object and copy the vertex data to it
	GLuint vbo;
	glGenBuffers(1, &vbo);

	const int VERTEX_COUNT = 30;

	GLfloat buffer[3 * VERTEX_COUNT];
	int id = 0;

	for (int i = 0; i < VERTEX_COUNT; i++)
	{
		buffer[id++] = In.getXCoordinate() + (radius * cos(((float)i * twicePi) / (float)VERTEX_COUNT));
		buffer[id++] = In.getYCoordinate() + (radius * sin(((float)i * twicePi) / (float)VERTEX_COUNT));
		buffer[id++] = centerZ;
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);

	// Create and compile the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	// Create and compile the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	// Link the vertex and fragment shader into a shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

	(fill) ? glDrawArrays(GL_TRIANGLE_FAN, 0, VERTEX_COUNT) : glDrawArrays(GL_LINE_LOOP, 0, VERTEX_COUNT);

	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

}

/* Initialize color for the triangle. Last argument is for transparency */
TAM::Triangle::Triangle(float r, float g, float b, float t) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->t = t;
}

/* Draw a triangle based on coordinates in an InputMapper */
void TAM::Triangle::draw(InputMapper In) {
	float centerZ = -In.getZCoordinate();

	std::string frag =
		"#version 150 core\n"
		"out vec4 outColor;"
		"void main() {"
		"   outColor = vec4(" + std::to_string(r) + "," + std::to_string(g) + "," + std::to_string(b) + "," + std::to_string(t) + ");"
		"}";

	const GLchar* vertexSource =
		"#version 150 core\n"
		"in vec2 position;"
		"void main() {"
		"   gl_Position = vec4(position, 0.0, 1.0);"
		"}";

	const GLchar* fragmentSource = frag.c_str();

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create a Vertex Buffer Object and copy the vertex data to it
	GLuint vbo;
	glGenBuffers(1, &vbo);

	const int VERTEX_COUNT = 6;

	GLfloat buffer[] = {
		/* First face */
		In.getXCoordinate(), In.getYCoordinate(), centerZ,
		In.getXCoordinate() + 0.025f, In.getYCoordinate(), centerZ,
		In.getXCoordinate(), In.getYCoordinate() + 0.05f, centerZ,

		/* Second face */
		In.getXCoordinate(), In.getYCoordinate(), centerZ,
		In.getXCoordinate() - 0.025f, In.getYCoordinate(), centerZ,
		In.getXCoordinate(), In.getYCoordinate() + 0.05f, centerZ
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);

	// Create and compile the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	// Create and compile the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	// Link the vertex and fragment shader into a shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

	glDrawArrays(GL_TRIANGLES, 0, VERTEX_COUNT);

	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

}

/* Initialize color for the square. Last argument is for transparency */
TAM::Square::Square(float r, float g, float b, float t) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->t = t;
}

/* Draw a square based on coordinates in an InputMapper */
void TAM::Square::draw(InputMapper In) {
	float centerZ = -In.getZCoordinate();

	std::string frag =
		"#version 150 core\n"
		"out vec4 outColor;"
		"void main() {"
		"   outColor = vec4(" + std::to_string(r) + "," + std::to_string(g) + "," + std::to_string(b) + "," + std::to_string(t) + ");"
		"}";

	const GLchar* vertexSource =
		"#version 150 core\n"
		"in vec2 position;"
		"void main() {"
		"   gl_Position = vec4(position, 0.0, 1.0);"
		"}";

	const GLchar* fragmentSource = frag.c_str();

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create a Vertex Buffer Object and copy the vertex data to it
	GLuint vbo;
	glGenBuffers(1, &vbo);

	const int VERTEX_COUNT = 6;

	GLfloat buffer[] = {
		/* First triangle */
		In.getXCoordinate(), In.getYCoordinate(), centerZ,
		In.getXCoordinate() + 0.025f, In.getYCoordinate(), centerZ,
		In.getXCoordinate(), In.getYCoordinate() + 0.05f, centerZ,

		/* Second face */
		In.getXCoordinate() + 0.025f, In.getYCoordinate() + 0.05f, centerZ,
		In.getXCoordinate() + 0.025f, In.getYCoordinate(), centerZ,
		In.getXCoordinate(), In.getYCoordinate() + 0.05f, centerZ,
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);

	// Create and compile the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	// Create and compile the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	// Link the vertex and fragment shader into a shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

	glDrawArrays(GL_TRIANGLES, 0, VERTEX_COUNT);

	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

}

/* Initializes the color and size of the triangle object */
TAM::RightTriangle::RightTriangle(float r, float g, float b, float t, float size) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->t = t;
	this->size = size;
	drawn = false;
	vertical_flip = 0;
	top = bot = left = right = 0.0f;
}

/* Draw a triangle based on coordinates in an InputMapper */
void TAM::RightTriangle::draw(InputMapper In) {
	float centerZ = -In.getZCoordinate();
	drawn = true;

	std::string frag =
		"#version 150 core\n"
		"out vec4 outColor;"
		"void main() {"
		"   outColor = vec4(" + std::to_string(r) + "," + std::to_string(g) + "," + std::to_string(b) + "," + std::to_string(t) + ");"
		"}";

	const GLchar* vertexSource =
		"#version 150 core\n"
		"in vec2 position;"
		"void main() {"
		"   gl_Position = vec4(position, 0.0, 1.0);"
		"}";

	const GLchar* fragmentSource = frag.c_str();

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create a Vertex Buffer Object and copy the vertex data to it
	GLuint vbo;
	glGenBuffers(1, &vbo);

	const int VERTEX_COUNT = 6;

	GLfloat buffer[] = {
		In.getXCoordinate(), In.getYCoordinate(), centerZ,
		In.getXCoordinate() + size / 2, In.getYCoordinate(), centerZ,
		In.getXCoordinate(), In.getYCoordinate() + size, centerZ,
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);

	// Create and compile the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	// Create and compile the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	// Link the vertex and fragment shader into a shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

	glDrawArrays(GL_TRIANGLES, 0, VERTEX_COUNT);

	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

}

/* Compounds the Shape in either top, bot, left or right position depending on the Position value passed.
* It also flips the triangle if horizontally, vertically or both.
* InputMapper In: The InputMapper that needs to be passed
* Position pos: The position where it is going to be compounded. It can be TOP, BOT, LEFT, or RIGHT
* bool hflip: Whether it should be horizontally flipped or not
* bool vflip: Whether it should be vertically flipped or not
* Draw() must be called first for compound() to work.
***/
TAM::InputMapper TAM::RightTriangle::compound(InputMapper In, Position pos, bool hflip, bool vflip) {
	//If draw() has not been called then there is nothing to compound to
	assert(drawn);

	InputMapper ret;
	float centerZ = -In.getZCoordinate();

	std::string frag =
		"#version 150 core\n"
		"out vec4 outColor;"
		"void main() {"
		"   outColor = vec4(" + std::to_string(r) + "," + std::to_string(g) + "," + std::to_string(b) + "," + std::to_string(t) + ");"
		"}";

	const GLchar* vertexSource =
		"#version 150 core\n"
		"in vec2 position;"
		"void main() {"
		"   gl_Position = vec4(position, 0.0, 1.0);"
		"}";

	const GLchar* fragmentSource = frag.c_str();

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create a Vertex Buffer Object and copy the vertex data to it
	GLuint vbo;
	glGenBuffers(1, &vbo);

	const int VERTEX_COUNT = 6;
	GLfloat buffer[9];

	switch (pos) {
	case TOP:
		if (!vflip) vertical_flip = 0;
		top += size;
		if (vertical_flip) top += size;
		buffer[0] = (!vflip) ? In.getXCoordinate() : In.getXCoordinate() + size / 2;
		buffer[1] = In.getYCoordinate() + top;
		buffer[2] = centerZ;
		buffer[3] = (!hflip) ? In.getXCoordinate() + size / 2 : In.getXCoordinate() - size / 2;
		buffer[4] = (!vflip) ? In.getYCoordinate() + top : In.getYCoordinate() + top - size;
		buffer[5] = centerZ;
		buffer[6] = In.getXCoordinate();
		buffer[7] = (!vflip) ? In.getYCoordinate() + size + top : In.getYCoordinate() + top;
		buffer[8] = centerZ;
		if (vflip) {
			top -= size; //There is nothing compounded on top if it was only flipped vertically
			vertical_flip++;
		}
		ret.setYCoordinate(In.getYCoordinate() + top);
		(!vflip) ? ret.setXCoordinate(In.getXCoordinate()) : ret.setXCoordinate(In.getXCoordinate() + size / 2);
		break;
	case BOT:
		if (!vflip) vertical_flip = 0;
		bot += size;
		if (vertical_flip) bot += size;
		buffer[0] = (!vflip) ? In.getXCoordinate() : In.getXCoordinate() + size / 2;
		buffer[1] = (!vflip) ? In.getYCoordinate() - bot : In.getYCoordinate() - (bot - size);
		buffer[2] = centerZ;

		if (vflip) {
			buffer[3] = (!hflip) ? In.getXCoordinate() + size / 2 : In.getXCoordinate();
		}
		else {
			buffer[3] = (!hflip) ? In.getXCoordinate() + size / 2 : In.getXCoordinate() - size / 2;
		}

		buffer[4] = In.getYCoordinate() - bot;
		buffer[5] = centerZ;
		buffer[6] = In.getXCoordinate();
		buffer[7] = In.getYCoordinate() + size - bot;
		buffer[8] = centerZ;
		if (vflip) {
			bot -= size;
			vertical_flip++;
		}
		ret.setYCoordinate(In.getYCoordinate() - bot);
		ret.setXCoordinate(In.getXCoordinate());
		break;
	case LEFT:
		left += size / 2;

		if (vflip) {
			buffer[0] = In.getXCoordinate() - (left - size / 2);
		}
		else {
			buffer[0] = (!hflip) ? In.getXCoordinate() - left : In.getXCoordinate() - (left - size / 2);
		}

		buffer[1] = (!vflip) ? In.getYCoordinate() : In.getYCoordinate() + size;
		buffer[2] = centerZ;

		if (vflip) {
			buffer[3] = (!hflip) ? In.getXCoordinate() + size / 2 - left : (In.getXCoordinate() - size / 2) - (left - size / 2);
		}
		else {
			buffer[3] = (!hflip) ? In.getXCoordinate() + size / 2 - left : (In.getXCoordinate() - size / 2) - (left - size / 2);
		}

		buffer[4] = In.getYCoordinate();
		buffer[5] = centerZ;

		if (vflip) {
			buffer[6] = (!hflip) ? In.getXCoordinate() - left : (In.getXCoordinate() - size / 2) - (left - size / 2);
		}
		else {
			buffer[6] = (!hflip) ? In.getXCoordinate() - left : In.getXCoordinate() - (left - size / 2);
		}

		buffer[7] = In.getYCoordinate() + size;
		buffer[8] = centerZ;
		ret.setYCoordinate(In.getYCoordinate());
		ret.setXCoordinate(In.getXCoordinate() - left);
		break;
	case RIGHT:
		right += size / 2;

		if (vflip) {
			buffer[0] = In.getXCoordinate() + right + size / 2;
			buffer[1] = In.getYCoordinate() + size;
		}
		else {
			buffer[0] = In.getXCoordinate() + right;
			buffer[1] = In.getYCoordinate();
		}

		buffer[2] = centerZ;

		if (vflip) {
			buffer[3] = (!hflip) ? In.getXCoordinate() + size / 2 + right : In.getXCoordinate() + right;
		}
		else {
			buffer[3] = (!hflip) ? In.getXCoordinate() + size / 2 + right : In.getXCoordinate() - size / 2 + right;
		}

		buffer[4] = In.getYCoordinate();
		buffer[5] = centerZ;
		buffer[6] = In.getXCoordinate() + right;
		buffer[7] = In.getYCoordinate() + size;
		buffer[8] = centerZ;
		ret.setYCoordinate(In.getYCoordinate());
		ret.setXCoordinate(In.getXCoordinate() + right);
		break;

	default:
		break;
	}


	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);

	// Create and compile the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	// Create and compile the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	// Link the vertex and fragment shader into a shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

	glDrawArrays(GL_TRIANGLES, 0, VERTEX_COUNT);

	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	return ret;

}

/* Clear must be called to erase a Shape compounding */
void TAM::RightTriangle::clear()
{
	top = bot = left = right = 0.0f;
}

TAM::Line::Line(float r, float g, float b, float t, float thickness)
{
	Coordinates firstCord;
	this->r = r;
	this->g = g;
	this->b = b;
	this->t = t;
	thick = thickness;
	top = bot = 0.0f;
	x = y = 0.0f;
	old_element = element = drawn = 0;
	firstCord.bot_x = firstCord.bot_y = firstCord.top_x = firstCord.top_y = firstCord.top_z = firstCord.bot_z = 0.0f;
	vec.push_back(firstCord);
	old_vec = vec;
	back_mode = false;
}

/* Sets the coordinates for the first line to draw. Needs to be called before drawing or compounding. */
void TAM::Line::setEndPoint(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

/* Draws a Line in the coordinates given by the InputMapper. setEndPoint must be called first*/
void TAM::Line::draw(InputMapper In)
{
	//if (x == y == 0.0f) return;

	drawn++;
	float centerZ = In.getZCoordinate();

	std::string frag =
		"#version 150 core\n"
		"out vec4 outColor;"
		"void main() {"
		"   outColor = vec4(" + std::to_string(r) + "," + std::to_string(g) + "," + std::to_string(b) + "," + std::to_string(t) + ");"
		"}";

	const GLchar* vertexSource =
		"#version 150 core\n"
		"in vec2 position;"
		"void main() {"
		"   gl_Position = vec4(position, 0.0, 1.0);"
		"}";

	const GLchar* fragmentSource = frag.c_str();


	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);

	if (In.getYCoordinate() > y) {
		vec.at(element).top_y = In.getYCoordinate();
		vec.at(element).top_x = In.getXCoordinate();
		vec.at(element).top_z = In.getZCoordinate();
		vec.at(element).bot_y = y;
		vec.at(element).bot_x = x;
		vec.at(element).bot_z = z;
	}
	else {
		vec.at(element).top_y = y;
		vec.at(element).top_x = x;
		vec.at(element).top_z = z;
		vec.at(element).bot_y = In.getYCoordinate();
		vec.at(element).bot_x = In.getXCoordinate();
		vec.at(element).bot_z = In.getZCoordinate();
	}

	GLfloat vertices[6] =
	{
		In.getXCoordinate(), In.getYCoordinate(), In.getZCoordinate(), //First Vertex for line

		x, y, z //Second Vertex for line
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Create and compile the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	// Create and compile the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	// Link the vertex and fragment shader into a shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

	glLineWidth(thick);
	glDrawArrays(GL_LINES, 0, 2);

	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

}

/* Compounds a Line Shape either at TOP or BOT */
void TAM::Line::compound(Position pos)
{
	assert(drawn);

	std::string frag =
		"#version 150 core\n"
		"out vec4 outColor;"
		"void main() {"
		"   outColor = vec4(" + std::to_string(r) + "," + std::to_string(g) + "," + std::to_string(b) + ", 0.0" + ");"
		"}";

	const GLchar* vertexSource =
		"#version 150 core\n"
		"in vec2 position;"
		"void main() {"
		"   gl_Position = vec4(position, 0.0, 1.0);"
		"}";

	const GLchar* fragmentSource = frag.c_str();


	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);

	float new_x, new_y, new_z;

	/* Decides whether to use the previous Line top or bot coordinates depending on the value of pos */
	switch (pos)
	{
	case TOP:
		new_x = vec.at(element).top_x;
		new_y = vec.at(element).top_y;
		new_z = vec.at(element).top_z;
		break;
	case BOT:
		new_x = vec.at(element).bot_x;
		new_y = vec.at(element).bot_y;
		new_z = vec.at(element).bot_z;
		break;
	default:
		break;
	}

	/* Adds a new Coordinates object to the vector and increases the elements size */
	Coordinates newCord;
	vec.push_back(newCord);
	element++;

	/* Checks which set of points are on top or bot and sets the values of the vector accordingly */
	if (new_y > y)
	{
		vec.at(element).top_x = new_x;
		vec.at(element).top_y = new_y;
		vec.at(element).top_z = new_z;
		vec.at(element).bot_x = x;
		vec.at(element).bot_y = y;
		vec.at(element).bot_z = z;
	}
	else if (new_y < y)
	{
		vec.at(element).top_x = x;
		vec.at(element).top_y = y;
		vec.at(element).top_z = z;
		vec.at(element).bot_x = new_x;
		vec.at(element).bot_y = new_y;
		vec.at(element).bot_z = new_z;
	}
	else
	{
		vec.at(element).top_x = new_x;
		vec.at(element).top_y = new_y;
		vec.at(element).top_z = new_z;
		vec.at(element).bot_x = x;
		vec.at(element).bot_y = y;
		vec.at(element).bot_z = z;
	}

	GLfloat vertices[6] =
	{
		new_x, new_y, new_z, //First Vertex for line

		x, y, z //Second Vertex for line
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Create and compile the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	// Create and compile the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	// Link the vertex and fragment shader into a shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

	glLineWidth(thick);
	glDrawArrays(GL_LINES, 0, 2);

	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

/* Sets the active Line to be one Line before the current active Line.
* This is used for compounding only.
***/
void TAM::Line::goBack()
{
	if (!back_mode)
	{
		old_vec = vec;
		old_element = element;
	}
	if (--element < 0) element = 0;
	back_mode = true;
}

/* Sets the active Line to be one Line after the current active Line.
* This is used for compounding only.
***/
void TAM::Line::goForward()
{
	if (!back_mode)
	{
		old_vec = vec;
		old_element = element;
	}
	if (++element > vec.size()) --element;
	back_mode = true;
}

/* Restores the active Line to where it was before the first use of goBack() or goForward(). */
void TAM::Line::restore()
{
	if (back_mode)
	{
		for (int i = 0; i < vec.size(); i++) {
			if (vec.at(i) != old_vec.at(i)) {
				old_vec.push_back(vec.at(i)); //A new Coordinates has been found so we add it to the vector 
			}
		}
		vec = old_vec;
		element = old_element;
	}
	back_mode = false;
}

/* Connects two points given two InputMappers. Can be used to connect any kind of Shapes.
 * The color and thickness of the connecting line can be customized.
 ***/
void TAM::Line::connect(InputMapper In, InputMapper new_In)
{
	setEndPoint(new_In.getXCoordinate(), new_In.getYCoordinate(), new_In.getZCoordinate());
	draw(In);
}

/* Constructs a cube and assigns it size */
TAM::Cube::Cube(float size)
{
	this->size = size;
	forw = back = top = bot = left = right = 0;
}

/* Clears the cube compounding and retuns the pointer to the first drawn cube */
void TAM::Cube::clear()
{
	forw = back = top = bot = left = right = 0;
	vec_x = In.getXCoordinate() / 3, vec_y = -In.getYCoordinate() / 2, vec_z = 0.0f;
}

/* Clear the compounding in a specified position and keeps intact the rest */
void TAM::Cube::clear(Position pos)
{
	switch (pos)
	{
	case TOP:
		top = 0;
		break;
	case BOT:
		bot = 0;
		break;
	case RIGHT:
		right = 0;
		break;
	case LEFT:
		left = 0;
		break;
	case FORW:
		forw = 0;
		break;
	case BACK:
		back = 0;
		break;
	}
}

/* Sets the pointer to the current position in the given direction */
void TAM::Cube::set(Position pos)
{
	switch (pos)
	{
	case TOP:
		vec_y += top;
		top = 0;
		break;
	case BOT:
		vec_y -= bot;
		bot = 0;
		break;
	case RIGHT:
		vec_x -= right;
		right = 0;
		break;
	case LEFT:
		vec_x += left;
		left = 0;
		break;
	case FORW:
		vec_z -= forw;
		forw = 0;
		break;
	case BACK:
		vec_z += back;
		back = 0;
		break;
	}
}

/* Draws a cube in the coordinates pointed to by the InputMapper */
void TAM::Cube::draw(InputMapper In)
{
	vec_x = In.getXCoordinate() / 3, vec_y = -In.getYCoordinate() / 2, vec_z = 0.0f;
	this->In = In;
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	Helpers newHelp(500, 500);

	// Create and compile our GLSL program from the shaders
	GLuint programID = newHelp.LoadShaders("C:/TransformVertexShader.vertexshader", "C:/ColorFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, -3), // Camera is at (4,3,-3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);

	Model = glm::translate(Model, glm::vec3(In.getXCoordinate() / 3, -In.getYCoordinate() / 2, 0.0f));
	Model = glm::scale(Model, glm::vec3(size / 10, size / 10, size / 10));
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,

		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	static const GLfloat g_color_buffer_data[] = {
		0.583f, 0.771f, 0.014f,
		0.609f, 0.115f, 0.436f,
		0.327f, 0.483f, 0.844f,
		0.822f, 0.569f, 0.201f,
		0.435f, 0.602f, 0.223f,
		0.310f, 0.747f, 0.185f,
		0.597f, 0.770f, 0.761f,
		0.559f, 0.436f, 0.730f,
		0.359f, 0.583f, 0.152f,
		0.483f, 0.596f, 0.789f,
		0.559f, 0.861f, 0.639f,
		0.195f, 0.548f, 0.859f,
		0.014f, 0.184f, 0.576f,
		0.771f, 0.328f, 0.970f,
		0.406f, 0.615f, 0.116f,
		0.676f, 0.977f, 0.133f,
		0.971f, 0.572f, 0.833f,
		0.140f, 0.616f, 0.489f,
		0.997f, 0.513f, 0.064f,
		0.945f, 0.719f, 0.592f,
		0.543f, 0.021f, 0.978f,
		0.279f, 0.317f, 0.505f,
		0.167f, 0.620f, 0.077f,
		0.347f, 0.857f, 0.137f,
		0.055f, 0.953f, 0.042f,
		0.714f, 0.505f, 0.345f,
		0.783f, 0.290f, 0.734f,
		0.722f, 0.645f, 0.174f,
		0.302f, 0.455f, 0.848f,
		0.225f, 0.587f, 0.040f,
		0.517f, 0.713f, 0.338f,
		0.053f, 0.959f, 0.120f,
		0.393f, 0.621f, 0.362f,
		0.673f, 0.211f, 0.457f,
		0.820f, 0.883f, 0.371f,
		0.982f, 0.099f, 0.879f
	};
	// One color for each vertex. They were generated randomly.
	//static const GLfloat g_color_buffer_data[] = {
	//	1.0f, 0.0f, 0.0f,				/* Right side bottom triangle */
	//	1.0f, 0.0f, 0.0f,
	//	1.0f, 0.0f, 0.0f,

	//	1.0f, 0.388f, 0.278f,				/* Front facing triangle*/
	//	1.0f, 0.388f, 0.278f,
	//	1.0f, 0.388f, 0.278f,

	//	1.0f, 0.270f, 0.0f,				/* Bottom triangle */
	//	1.0f, 0.270f, 0.0f,
	//	1.0f, 0.270f, 0.0f,

	//	1.0f, 0.388f, 0.278f,				/* Front facing triangle */
	//	1.0f, 0.388f, 0.278f,
	//	1.0f, 0.388f, 0.278f,

	//	1.0f, 0.0f, 0.0f,			  /* Right side top triangle*/
	//	1.0f, 0.0f, 0.0f,
	//	1.0f, 0.0f, 0.0f,

	//	1.0f, 0.270f, 0.0f,		/* Bottom triangle */
	//	1.0f, 0.270f, 0.0f,
	//	1.0f, 0.270f, 0.0f,

	//	1.0f, 0.388f, 0.278f,		/* Back riangle */
	//	1.0f, 0.388f, 0.278f,
	//	1.0f, 0.388f, 0.278f,

	//	0.255f, 0.0f, 0.0f,			/* left top face triangle*/
	//	0.255f, 0.0f, 0.0f,
	//	0.255f, 0.0f, 0.0f,

	//	0.255f, 0.0f, 0.0f,			/* left bot face triangle */
	//	0.255f, 0.0f, 0.0f,
	//	0.255f, 0.0f, 0.0f,

	//	1.0f, 0.270f, 0.0f,			/* Top bot face triangle */
	//	1.0f, 0.270f, 0.0f,
	//	1.0f, 0.270f, 0.0f,

	//	1.0f, 0.270f, 0.0f,			/* Top top face triangle */
	//	1.0f, 0.270f, 0.0f,
	//	1.0f, 0.270f, 0.0f,

	//	0.255f, 0.0f, 0.0f,			/* Back triangle */
	//	0.255f, 0.0f, 0.0f,
	//	0.255f, 0.0f, 0.0f,
	//};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	glUseProgram(programID);

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);
}

/* Compounds a cube in the given direction */
void TAM::Cube::compound(Position pos)
{
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	Helpers newHelp(500, 500);

	// Create and compile our GLSL program from the shaders
	GLuint programID = newHelp.LoadShaders("C:/TransformVertexShader.vertexshader", "C:/ColorFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, -3), // Camera is at (4,3,-3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);

	switch (pos)
	{
	case TOP:
		top += size / 5;
		Model = glm::translate(Model, glm::vec3(vec_x, vec_y + top, vec_z));
		break;
	case BOT:
		bot += size / 5;
		Model = glm::translate(Model, glm::vec3(vec_x, vec_y - bot, vec_z));
		break;
	case RIGHT:
		right += size / 5;
		Model = glm::translate(Model, glm::vec3(vec_x - right, vec_y, vec_z));
		break;
	case LEFT:
		left += size / 5;
		Model = glm::translate(Model, glm::vec3(vec_x + left, vec_y + top, vec_z));
		break;
	case FORW:
		forw += size / 5;
		Model = glm::translate(Model, glm::vec3(vec_x + left, vec_y, vec_z - forw));
		break;
	case BACK:
		back += size / 5;
		Model = glm::translate(Model, glm::vec3(vec_x + left, vec_y, vec_z + back));
		break;
	default:
		break;
	}

	
	Model = glm::scale(Model, glm::vec3(size / 10, size / 10, size / 10));
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,

		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	// One color for each vertex. They were generated randomly.
	static const GLfloat g_color_buffer_data[] = {
		0.583f, 0.771f, 0.014f,
		0.609f, 0.115f, 0.436f,
		0.327f, 0.483f, 0.844f,
		0.822f, 0.569f, 0.201f,
		0.435f, 0.602f, 0.223f,
		0.310f, 0.747f, 0.185f,
		0.597f, 0.770f, 0.761f,
		0.559f, 0.436f, 0.730f,
		0.359f, 0.583f, 0.152f,
		0.483f, 0.596f, 0.789f,
		0.559f, 0.861f, 0.639f,
		0.195f, 0.548f, 0.859f,
		0.014f, 0.184f, 0.576f,
		0.771f, 0.328f, 0.970f,
		0.406f, 0.615f, 0.116f,
		0.676f, 0.977f, 0.133f,
		0.971f, 0.572f, 0.833f,
		0.140f, 0.616f, 0.489f,
		0.997f, 0.513f, 0.064f,
		0.945f, 0.719f, 0.592f,
		0.543f, 0.021f, 0.978f,
		0.279f, 0.317f, 0.505f,
		0.167f, 0.620f, 0.077f,
		0.347f, 0.857f, 0.137f,
		0.055f, 0.953f, 0.042f,
		0.714f, 0.505f, 0.345f,
		0.783f, 0.290f, 0.734f,
		0.722f, 0.645f, 0.174f,
		0.302f, 0.455f, 0.848f,
		0.225f, 0.587f, 0.040f,
		0.517f, 0.713f, 0.338f,
		0.053f, 0.959f, 0.120f,
		0.393f, 0.621f, 0.362f,
		0.673f, 0.211f, 0.457f,
		0.820f, 0.883f, 0.371f,
		0.982f, 0.099f, 0.879f
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	glUseProgram(programID);

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

}

TAM::DepthLine::DepthLine(float r, float g, float b, float width)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->width = width;
}

void TAM::DepthLine::setEndPoint(InputMapper In, float x, float y, float z)
{
	InputMapper newInput;
	//float new_x = In.getXCoordinate() - x;
	newInput.setXCoordinate(x);
	newInput.setYCoordinate(y);
	newInput.setZCoordinate(z);

	TAM::Cube newCube(width);
	newCube.draw(newInput);
}

void TAM::DepthLine::draw(InputMapper In)
{
	TAM::Cube newCube(width);
	newCube.draw(In);
}

/* Draws a pyramid in the coordinates given by the InputMapper */
void TAM::Pyramid::draw(InputMapper In)
{
	vec_x = In.getXCoordinate() / 3, vec_y = -In.getYCoordinate() / 2, vec_z = 0.0f;
	this->In = In;
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	Helpers newHelp(500, 500);

	// Create and compile our GLSL program from the shaders
	GLuint programID = newHelp.LoadShaders("C:/TransformVertexShader.vertexshader", "C:/ColorFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, -3), // Camera is at (4,3,-3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);

	Model = glm::translate(Model, glm::vec3(In.getXCoordinate() / 3, -In.getYCoordinate() / 2, 0.0f));
	Model = glm::scale(Model, glm::vec3(size / 10, size / 10, size / 10));
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

	/* Vertices for the pyramid */
	static const GLfloat g_vertex_buffer_data[] = {
		 -1.0f, -1.0f, 1.0f,
		 -1.0f, -1.0f, -1.0f,
		  1.0f, -1.0f, 1.0f,

		 -1.0f, -1.0f, -1.0f,
		  1.0f, -1.0f, -1.0f,
		  1.0f, -1.0f, 1.0f,

		  1.0f, -1.0f, -1.0f,
		  1.0f, -1.0f, 1.0f,
		  0.0f, 1.0f, 0.0f,

		  1.0f, -1.0f, 1.0f,
		  -1.0f, -1.0f, 1.0f,
		  0.0f, 1.0f, 0.0f,

		  -1.0f, -1.0f, 1.0f,
		  -1.0f, -1.0f, -1.0f,
		  0.0f, 1.0f, 0.0f,

		  -1.0f, -1.0f, -1.0f,
		  1.0f, -1.0f, -1.0f,
		  0.0f, 1.0f, 0.0f
	};

	// One color for each vertex. They were generated randomly.
	static const GLfloat g_color_buffer_data[] = {
		0.583f, 0.771f, 0.014f,
		0.609f, 0.115f, 0.436f,
		0.327f, 0.483f, 0.844f,

		0.822f, 0.569f, 0.201f,
		0.435f, 0.602f, 0.223f,
		0.310f, 0.747f, 0.185f,

		0.597f, 0.770f, 0.761f,
		0.559f, 0.436f, 0.730f,
		0.359f, 0.583f, 0.152f,

		0.483f, 0.596f, 0.789f,
		0.559f, 0.861f, 0.639f,
		0.195f, 0.548f, 0.859f,

		0.014f, 0.184f, 0.576f,
		0.771f, 0.328f, 0.970f,
		0.406f, 0.615f, 0.116f,

		0.676f, 0.977f, 0.133f,
		0.971f, 0.572f, 0.833f,
		0.140f, 0.616f, 0.489f		
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	glUseProgram(programID);

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	/* Draw the pyramid here */
	glDrawArrays(GL_TRIANGLES, 0, 6 * 3); 

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);
}

TAM::Pyramid::Pyramid(float size)
{
	this->size = size;
	forw = back = top = bot = left = right = 0;
}

void TAM::Pyramid::clear(Position pos)
{
	switch (pos)
	{
	case TOP:
		top = 0;
		break;
	case BOT:
		bot = 0;
		break;
	case RIGHT:
		right = 0;
		break;
	case LEFT:
		left = 0;
		break;
	case FORW:
		forw = 0;
		break;
	case BACK:
		back = 0;
		break;
	}
}


void TAM::Pyramid::clear()
{
	forw = back = top = bot = left = right = 0;
	vec_x = In.getXCoordinate() / 3, vec_y = -In.getYCoordinate() / 2, vec_z = 0.0f;
}

void TAM::Pyramid::set(Position pos)
{
	switch (pos)
	{
	case TOP:
		vec_y += top;
		top = 0;
		break;
	case BOT:
		vec_y -= bot;
		bot = 0;
		break;
	case RIGHT:
		vec_x -= right;
		right = 0;
		break;
	case LEFT:
		vec_x += left;
		left = 0;
		break;
	case FORW:
		vec_z -= forw;
		forw = 0;
		break;
	case BACK:
		vec_z += back;
		back = 0;
		break;
	}
}

/* Compounds a pyramid at Position pos. If rotation is desired rotate should be set to true.
   rads are the radians by which the pyramid is going to be rotated and x, y, z are the axis
   against which the rotation is going to be performed.
 */
void TAM::Pyramid::compound(InputMapper In, Position pos, bool rotate, float rads, float x, float y, float z)
{
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	Helpers newHelp(500, 500);

	// Create and compile our GLSL program from the shaders
	GLuint programID = newHelp.LoadShaders("C:/TransformVertexShader.vertexshader", "C:/ColorFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, -3), // Camera is at (4,3,-3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);

	switch (pos)
	{
	case TOP:
		top += size / 5;
		Model = glm::translate(Model, glm::vec3(vec_x, vec_y + top, vec_z));
		break;
	case BOT:
		bot += size / 5;
		Model = glm::translate(Model, glm::vec3(vec_x, vec_y - bot, vec_z));
		break;
	case RIGHT:
		right += size / 5;
		Model = glm::translate(Model, glm::vec3(vec_x - right, vec_y, vec_z));
		break;
	case LEFT:
		left += size / 5;
		Model = glm::translate(Model, glm::vec3(vec_x + left, vec_y + top, vec_z));
		break;
	case FORW:
		forw += size / 5;
		Model = glm::translate(Model, glm::vec3(vec_x + left, vec_y, vec_z - forw));
		break;
	case BACK:
		back += size / 5;
		Model = glm::translate(Model, glm::vec3(vec_x + left, vec_y, vec_z + back));
		break;
	default:
		break;
	}

	if (rotate) 
	{
		Model = glm::rotate(
			Model,
			glm::radians(rads),
			glm::vec3(x, y, z)
			);
	}

	Model = glm::scale(Model, glm::vec3(size / 10, size / 10, size / 10));
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

	/* Vertices for the pyramid */
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,

		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		0.0f, 1.0f, 0.0f,

		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		0.0f, 1.0f, 0.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		0.0f, 1.0f, 0.0f,

		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		0.0f, 1.0f, 0.0f
	};

	// One color for each vertex. They were generated randomly.
	static const GLfloat g_color_buffer_data[] = {
		0.583f, 0.771f, 0.014f,
		0.609f, 0.115f, 0.436f,
		0.327f, 0.483f, 0.844f,

		0.822f, 0.569f, 0.201f,
		0.435f, 0.602f, 0.223f,
		0.310f, 0.747f, 0.185f,

		0.597f, 0.770f, 0.761f,
		0.559f, 0.436f, 0.730f,
		0.359f, 0.583f, 0.152f,

		0.483f, 0.596f, 0.789f,
		0.559f, 0.861f, 0.639f,
		0.195f, 0.548f, 0.859f,

		0.014f, 0.184f, 0.576f,
		0.771f, 0.328f, 0.970f,
		0.406f, 0.615f, 0.116f,

		0.676f, 0.977f, 0.133f,
		0.971f, 0.572f, 0.833f,
		0.140f, 0.616f, 0.489f
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	glUseProgram(programID);

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	/* Draw the pyramid here */
	glDrawArrays(GL_TRIANGLES, 0, 6 * 3);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);
}

void TAM::Sphere::draw(InputMapper In)
{
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	Helpers newHelp(500, 500);

	// Create and compile our GLSL program from the shaders
	GLuint programID = newHelp.LoadShaders("C:/TransformVertexShader.vertexshader", "C:/ColorFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, -3), // Camera is at (4,3,-3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);

	Model = glm::translate(Model, glm::vec3(In.getXCoordinate() / 3, (-In.getYCoordinate() / 2), 0.0f));
	Model = glm::scale(Model, glm::vec3(size / 10, size / 10, size / 10));
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

	/* Vertices for the pyramid */
	const int rings = 12;
	const int sectors = 12;

	const float  R = 1. / (float)(rings - 1);
	const float  S = 1. / (float)(sectors - 1);
	const double PI = atan(1) * 4;

	std::vector<GLfloat> g_vertex_buffer_data;
	g_vertex_buffer_data.resize(rings * sectors * 3);
	std::vector<GLfloat>::iterator v = g_vertex_buffer_data.begin();
	
	for (int r = 0; r < rings; r++) for (int s = 0; s < sectors; s++) {
		float const y = sin((-PI/2) + PI * r * R);
		float const x = cos(2 * PI * s * S) * sin(PI * r * R);
		float const z = sin(2 * PI * s * S) * sin(PI * r * R);

		*v++ = x * radius;
		*v++ = y * radius;
		*v++ = z * radius;

	}


	// One color for each vertex. They were generated randomly.
	std::vector<GLfloat> g_color_buffer_data;
	g_color_buffer_data.resize(rings * sectors * 3);
	std::vector<GLfloat>::iterator c = g_color_buffer_data.begin();
	int count = 0;

	for (int r = 0; r < rings; r++) for (int s = 0; s < sectors; s++) {
		*c++ = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);;
		*c++ = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);;
		*c++ = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);;
	}

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), &g_vertex_buffer_data[0], GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), &g_color_buffer_data[0], GL_STATIC_DRAW);

	glUseProgram(programID);

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	/* Draw the pyramid here */
	glDrawArrays(GL_TRIANGLES, 0, sectors * rings * 3);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);
}