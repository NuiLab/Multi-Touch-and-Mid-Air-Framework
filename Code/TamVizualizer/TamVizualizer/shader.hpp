#ifndef SHADER_HPP
#define SHADER_HPP
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <string.h>
#include <glew.h>
#include <qdebug.h>
/*credit: https://code.google.com/p/opengl-tutorial-org/source/browse/common/shader.hpp*/
GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);

#endif
