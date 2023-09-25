 #include <ab/shader.h> //At top of file
#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

struct Vertex {
	float x, y, z;
	float u, v;
};

unsigned int createShader(GLenum shaderType, const char* sourceCode);
unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indicesData, int numIndices);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

const int SCREEN_WIDTH = 800; //using 800 x 450 because of shadertoy
const int SCREEN_HEIGHT = 450;



Vertex vertices[4] = {
	//x       y      z      u    v
   { -1.0 , -1.0 , 0.0 , 0.0 , 0.0 }, //Bottom left
   { 1.0 ,  -1.0,  0.0 , 1.0 , 0.0 }, //Bottom right
   { 1.0 ,  1.0 ,  0.0 , 1.0 , 1.0 },  //Top right
   { -1.0 ,  1.0 , 0.0 , 0.0 , 1.0 }  //Top left
};


unsigned int indices[6] = {
	0, 1, 2, //Triangle 1
	 2, 3, 0  //Triangle 2
};

float triangleColor[3] = { 5.0f, 1.0f, 0.0f };	//color
bool showImGUIDemoWindow = false;


int main() {
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
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}


	//Example usage in main()
	ab::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	shader.use();

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	unsigned int vao = createVAO(vertices, 4, indices, 6);

	//glUseProgram(shader);
	glBindVertexArray(vao);

	//Wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Set uniforms
		//shader.setVec3("triangleColor", triangleColor[0], triangleColor[1], triangleColor[2]);
		shader.setFloat("iTime", (float)glfwGetTime());
		shader.setVec3("iResolution", SCREEN_WIDTH, SCREEN_HEIGHT, 0.0);
		shader.setVec3("sunColor", 1.0, 1.0, 0.0);
		shader.setVec3("skyColorTop", 0.5, 0.5, 1.0);
		shader.setVec3("skyColorBot", 1.0, 0.5, 0.3);
		
		//Draw using indices
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);



		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Checkbox("Show Demo Window", &showImGUIDemoWindow);
			ImGui::ColorEdit3("Triangle Color", triangleColor);
			ImGui::End();
			if (showImGUIDemoWindow) {
				ImGui::ShowDemoWindow(&showImGUIDemoWindow);
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indicesData, int numIndices) {
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int ebo;	//Element buffer object
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, indicesData, GL_STATIC_DRAW);

	//Define a new buffer id
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Allocate space for + send vertex data to GPU.
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numVertices, vertexData, GL_STATIC_DRAW);

	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, x));
	glEnableVertexAttribArray(0);

	//UV
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, u)));
	glEnableVertexAttribArray(1);

	return vao;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

