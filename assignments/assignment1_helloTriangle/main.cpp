#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

unsigned int createVAO(float* vertexData, int numVertices) {
	//VBO
	//Define a new buffer id
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Allocate space for + send vertex data to GPU.
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * numVertices, vertexData, GL_STATIC_DRAW);

	//VAO
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//Tell vao to pull vertex data from vbo
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Define position attribute (3 floats)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)0);
	glEnableVertexAttribArray(0);

	return vao;
}

unsigned int createShader(GLenum shaderType, const char* sourceCode) {
	if (shaderType == GL_VERTEX_SHADER) {
		
		
		//COMPILING vertex shader
		//Create a new vertex shader object
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		//Supply the shader object with source code
		glShaderSource(vertexShader, 1, &sourceCode, NULL);
		//Compile the shader object
		glCompileShader(vertexShader);
		//...
		int successVertex;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successVertex);
		if (!successVertex) {
			//512 is an arbitrary length, but should be plenty of characters for our error message.
			char infoLog[512];
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			printf("Failed to compile shader: %s", infoLog);
			return 0;
		}

		return vertexShader;
	}
	else if (shaderType == GL_FRAGMENT_SHADER) {


		//COMPILING frag shader
		//Create a new frag shader object
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		//Supply the shader object with source code
		glShaderSource(fragmentShader, 1, &sourceCode, NULL);
		//Compile the shader object
		glCompileShader(fragmentShader);
		//...
		int successFrag;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successFrag);
		if (!successFrag) {
			//512 is an arbitrary length, but should be plenty of characters for our error message.
			char infoLog[512];
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			printf("Failed to compile shader: %s", infoLog);
			return 0;
		}

		return fragmentShader;
	}
}

unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
	
	//attaching/linking shaders
	unsigned int vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	unsigned int shaderProgram = glCreateProgram();
	//Attach each stage
	glAttachShader(shaderProgram, vertexShader);
	//glAttachShader(shaderProgram, geometryShader);
	glAttachShader(shaderProgram, fragmentShader);
	//Link all the stages together
	glLinkProgram(shaderProgram);
	//Check for link errors
	int success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("Failed to link shader program: %s", infoLog);
		return 0;
	}
	//The linked program now contains our compiled code, so we can delete these intermediate objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

int main() {
	//include in createShader?
	//vertex shader
	const char* vertexShaderSource = R"(
	#version 450
	layout(location = 0) in vec3 vPos;
	void main(){
		gl_Position = vec4(vPos,1.0);
	}
	)";

	//Frag shader
	const char* fragmentShaderSource = R"(
	#version 450
	out vec4 FragColor;
	void main(){
		FragColor = vec4(1.0,0.0,0.0,1.0);
	}
	)";



	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}
	
	//vertex data
	float vertices[9] = {
		//x   //y  //z
		-0.5, -0.5, 0.0, //Bottom left
		 0.5, -0.5, 0.0, //Bottom right
		 0.0,  0.5, 0.0  //Top center
	};
	
	unsigned int shader = createShaderProgram(vertexShaderSource, fragmentShaderSource);
	unsigned int vao = createVAO(vertices, 3);


	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}


