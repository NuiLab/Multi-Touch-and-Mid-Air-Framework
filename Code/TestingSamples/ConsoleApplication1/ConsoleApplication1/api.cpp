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

/* Returns w coordinate */
inline float TAM::InputMapper::getWCoordinate() {
	return w;
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

TAM::Circle::Circle(float rad, float r, float g, float b) {
	radius = rad;
	this->r = r;
	this->g = g;
	this->b = b;
}

void TAM::Circle::draw(InputMapper In) {
	static const GLfloat twicePi = 2.0f * atan(1)*4;
	float centerZ = -In.getZCoordinate();

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

	(false) ? glDrawArrays(GL_TRIANGLE_FAN, 0, VERTEX_COUNT) : glDrawArrays(GL_LINE_LOOP, 0, VERTEX_COUNT);

	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

}

/* Initialize color for the triangle */
TAM::Triangle::Triangle(float r, float g, float b) {
	this->r = r;
	this->g = g;
	this->b = b;
}

/* Draw a triangle based on coordinates in an InputMapper */
void TAM::Triangle::draw(InputMapper In) {
	float centerZ = -In.getZCoordinate();

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

/* Initialize color for the square */
TAM::Square::Square(float r, float g, float b) {
	this->r = r;
	this->g = g;
	this->b = b;
}

/* Draw a square based on coordinates in an InputMapper */
void TAM::Square::draw(InputMapper In) {
	float centerZ = -In.getZCoordinate();

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

TAM::RightTriangle::RightTriangle(float r, float g, float b, float size) {
	this->r = r;
	this->g = g;
	this->b = b;
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

	// Create a Vertex Buffer Object and copy the vertex data to it
	GLuint vbo;
	glGenBuffers(1, &vbo);

	const int VERTEX_COUNT = 6;

	GLfloat buffer[] = {
		In.getXCoordinate(), In.getYCoordinate(), centerZ,
		In.getXCoordinate() + size/2, In.getYCoordinate(), centerZ,
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
		buffer[0] = (!vflip) ? In.getXCoordinate() : In.getXCoordinate() + size/2;
		buffer[1] = In.getYCoordinate() + top;
		buffer[2] = centerZ;
		buffer[3] = (!hflip) ? In.getXCoordinate() + size/2 : In.getXCoordinate() - size/2;
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
		buffer[0] = (!vflip) ? In.getXCoordinate() : In.getXCoordinate() + size/2;
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
		break;
	case LEFT:
		left += size/2;

		if (vflip) {
			buffer[0] = In.getXCoordinate() - (left - size/2);
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
		break;
	case RIGHT:
		right += size/2;

		if (vflip) {
			buffer[0] = In.getXCoordinate() + right + size/2;
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

TAM::Line::Line(float r, float g, float b, float thickness) 
{
	Coordinates firstCord;
	this->r = r;
	this->g = g;
	this->b = b;
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
	if (new_y > y) {
		vec.at(element).top_x = new_x;
		vec.at(element).top_y = new_y;
		vec.at(element).top_z = new_z;
		vec.at(element).bot_x = x;
		vec.at(element).bot_y = y;
		vec.at(element).bot_z = z;
	}
	else {
		vec.at(element).top_x = x; 
		vec.at(element).top_y = y; 
		vec.at(element).top_z = z;
		vec.at(element).bot_x = new_x; //Weird should be new_x new_y new_z
		vec.at(element).bot_y = new_y;
		vec.at(element).bot_z = new_z;
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