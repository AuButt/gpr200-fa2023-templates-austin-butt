#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <ew/procGen.h>
#include <ab/transformations.h>
#include <ab/camera.h>

//Camera aiming related variables
struct CameraControls {
	double prevMouseX, prevMouseY; //Mouse position from previous frame
	float yaw = 0, pitch = 0; //Degrees
	float mouseSensitivity = 0.1f; //How fast to turn with mouse
	bool firstMouse = true; //Flag to store initial mouse position
	float moveSpeed = 5.0f; //How fast to move with arrow keys (M/S)
};

void moveCamera(GLFWwindow* window, ab::Camera* camera, CameraControls* controls, float deltaTime) {
	//If right mouse is not held, release cursor and return early.
	if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2)) {
		//Release cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		controls->firstMouse = true;
		return;
	}
	//GLFW_CURSOR_DISABLED hides the cursor, but the position will still be changed as we move our mouse.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Get screen mouse position this frame
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	//If we just started right clicking, set prevMouse values to current position.
	//This prevents a bug where the camera moves as soon as we click.
	if (controls->firstMouse) {
		controls->firstMouse = false;
		controls->prevMouseX = mouseX;
		controls->prevMouseY = mouseY;
	}

	//TODO: Get mouse position delta for this frame
	float xDelta = mouseX - controls->prevMouseX;
	float yDelta = mouseY - controls->prevMouseY;
	//TODO: Add to yaw and pitch
	controls->yaw += (xDelta)*controls->mouseSensitivity;
	controls->pitch += (-yDelta) * controls->mouseSensitivity;
	//TODO: Clamp pitch between -89 and 89 degrees
	if (controls->pitch > 89) {
		controls->pitch = 89;
	}
	else if (controls->pitch < -89) {
		controls->pitch = -89;
	}
	//Remember previous mouse position
	controls->prevMouseX = mouseX;
	controls->prevMouseY = mouseY;

	//Construct forward vector using yaw and pitch. Don't forget to convert to radians!
	float newyaw = ew::Radians(controls->yaw);
	float newpitch = ew::Radians(controls->pitch);
	ew::Vec3 forward = ew::Vec3(sin(newyaw) * cos(newpitch), sin(newpitch), -cos(newyaw) * cos(newpitch));
	//By setting target to a point in front of the camera along its forward direction, our LookAt will be updated accordingly when rendering.
	camera->target = camera->position + forward;


	//TODO: Using camera forward and world up (0,1,0), construct camera right and up vectors. Graham-schmidt process!
	ew::Vec3 right = ew::Vec3(ew::Normalize(ew::Cross(forward, ew::Vec3(0, 1, 0))));
	ew::Vec3 up = ew::Vec3(ew::Normalize(ew::Cross(right, forward)));
		//TODO: Keyboard controls for moving along forward, back, right, left, up, and down. See Requirements for key mappings.
		//EXAMPLE: Moving along forward axis if W is held.
		//Note that this is framerate dependent, and will be very fast until you scale by deltaTime. See the next section.
		if (glfwGetKey(window, GLFW_KEY_W)) {
			//camera->position += forward * controls->moveSpeed;
			camera->position += forward * controls->moveSpeed * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_S)) {
			camera->position += -forward * controls->moveSpeed * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_D)) {
			camera->position += right * controls->moveSpeed * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_A)) {
			camera->position += -right * controls->moveSpeed * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_E)) {
			camera->position += up * controls->moveSpeed * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_Q)) {
			camera->position += -up * controls->moveSpeed * deltaTime;
		}

	//Setting camera.target should be done after changing position. Otherwise, it will use camera.position from the previous frame and lag behind
	camera->target = camera->position + forward;



};


void framebufferSizeCallback(GLFWwindow* window, int width, int height);

//Projection will account for aspect ratio!
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

const int NUM_CUBES = 4;
ab::Transform cubeTransforms[NUM_CUBES];

ab::Camera camera;
CameraControls cameraControls;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Camera", NULL, NULL);
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
	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	//Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Depth testing - required for depth sorting!
	glEnable(GL_DEPTH_TEST);

	ew::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	
	
	//camera setup
	camera.position = ew::Vec3(0, 0, 5);
	camera.target = ew::Vec3(0, 0, 0);
	camera.fov = 60;
	camera.aspectRatio = (float) SCREEN_WIDTH / SCREEN_HEIGHT;
	camera.orthoSize = 6;
	camera.orthographic = false;
	camera.nearPlane = 0.1;
	camera.farPlane = 100;
	

	//Cube mesh
	ew::Mesh cubeMesh(ew::createCube(0.5f));

	//Cube positions
	for (size_t i = 0; i < NUM_CUBES; i++)
	{
		cubeTransforms[i].position.x = i % (NUM_CUBES / 2) - 0.5;
		cubeTransforms[i].position.y = i / (NUM_CUBES / 2) - 0.5;
	}

	float prevTime = (float)glfwGetTime(); //Timestamp of previous frame
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		//Clear both color buffer AND depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Set uniforms
		shader.use();

		//TODO: Set model matrix uniform
		for (size_t i = 0; i < NUM_CUBES; i++)
		{
			//Construct model matrix
			shader.setMat4("_Model", cubeTransforms[i].getModelMatrix());
			cubeMesh.draw();
		}
		
		//Calculate deltaTime
		float time = (float)glfwGetTime(); //Timestamp of current frame
		float deltaTime = time - prevTime;
		prevTime = time;

		//Pass deltaTime into moveCamera. Update this function to include a 4th parameter.
		//camera controls

		moveCamera(window, &camera, &cameraControls, deltaTime);

		shader.setMat4("_CamView", camera.ViewMatrix());
		shader.setMat4("_CamProj", camera.ProjectionMatrix());

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Text("Cubes");
			for (size_t i = 0; i < NUM_CUBES; i++)
			{
				ImGui::PushID(i);
				if (ImGui::CollapsingHeader("Transform")) {
					ImGui::DragFloat3("Position", &cubeTransforms[i].position.x, 0.05f);
					ImGui::DragFloat3("Rotation", &cubeTransforms[i].rotation.x, 0.1f);
					ImGui::DragFloat3("Scale", &cubeTransforms[i].scale.x, 0.05f);
				}
				ImGui::PopID();
			}
			ImGui::Text("Camera");
			ImGui::DragFloat3("Position", &camera.position.x, 0.05f);
			ImGui::DragFloat3("Target", &camera.target.x, 0.05f);
			ImGui::Checkbox("Orthographic", &camera.orthographic);
			ImGui::SliderFloat("FOV", &camera.fov, 0.0f, 180.0f);	//issue
			ImGui::SliderFloat("Near Plane", &camera.nearPlane, 0.0f, 100.0f);	
			ImGui::SliderFloat("Far Place", &camera.farPlane, 0.0f, 100.0f);	
			ImGui::End();
			
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

