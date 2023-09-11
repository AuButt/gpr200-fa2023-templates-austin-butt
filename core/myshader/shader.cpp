//shader.cpp
#include "shader.h"
#include <stdio.h>

namespace myshader {
	std::string loadShaderSourceFromFile(const std::string& filePath) {
		std::ifstream fstream(filePath);
		if (!fstream.is_open()) {
			printf("Failed to load file %s", filePath);
			return {};
		}
		std::stringstream buffer;
		buffer << fstream.rdbuf();
		return buffer.str();
	}
}
