#include "src/includes.h"
#include "src/physics.h"
#include "src/graphics.h"
#include "src/utils.h"
using namespace std;
using namespace utils;
using namespace glm;



GLFWwindow* Window;


void framebufferSizeCallback(GLFWwindow* Window, int width, int height) {
	glViewport(0, 0, width, height);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	currentWindowResolution = glm::ivec2(width, height);
	currentRenderResolution = glm::ivec2(
		glm::min(width, desiredRenderResolution.x),
		glm::min(height, desiredRenderResolution.y)
	);

	verticalFOV = 2 * atan(tan(display::FOV * 0.5f) * (float(currentRenderResolution.y) / float(currentRenderResolution.x)));
}




void prepareVoxelData() {
	//10 x 5 x 5
	voxelData.push_back(GLuint(1u));
}



const std::vector<int> monitoredKeys = {
	GLFW_KEY_W, GLFW_KEY_S,
	GLFW_KEY_A, GLFW_KEY_D,
	GLFW_KEY_E, GLFW_KEY_Q,
	GLFW_KEY_SPACE,
	GLFW_KEY_LEFT_SHIFT,
	GLFW_KEY_LEFT_CONTROL,
	GLFW_KEY_ESCAPE,
};

double cursorXPos, cursorYPos, cursorXPosPrev, cursorYPosPrev;

void handleInputs() {
	glfwPollEvents();
	glfwGetCursorPos(Window, &cursorXPos, &cursorYPos);

	//Get inputs for this frame
	for (int key : monitoredKeys) {
		int keyState = glfwGetKey(Window, key);
		if (keyState == GLFW_PRESS) {
			keyMap[key] = true;

		} else if (keyState == GLFW_RELEASE) {
			keyMap[key] = false;
		}
	}


	//View rotation
	double cursorXDelta = cursorXPos - cursorXPosPrev;
	double cursorYDelta = cursorYPos - cursorYPosPrev;
	camera.viewAngle.x += cursorXDelta * constants::TO_RAD * constants::CAMERA_TURN_SPEED;
	camera.viewAngle.x = fmodf(camera.viewAngle.x + constants::PI*3.0f, constants::PI2) - constants::PI;
	double dY = cursorYDelta * constants::TO_RAD * constants::CAMERA_TURN_SPEED;
	camera.viewAngle.y = glm::clamp(float(camera.viewAngle.y-dY), verticalFOV-0.5f*constants::PI, 0.5f*constants::PI-verticalFOV);
}




int main() {
	try { //Catch exceptions
	currentWindowResolution = display::INITIAL_SCREEN_RESOLUTION;
	desiredRenderResolution = display::RENDER_RESOLUTION;
	currentRenderResolution = glm::ivec2(
		glm::min(currentWindowResolution.x, desiredRenderResolution.x),
		glm::min(currentWindowResolution.y, desiredRenderResolution.y)
	);


	Window = graphics::initializeWindow(currentWindowResolution.x, currentWindowResolution.y, "Voxel-Renderer");
	glfwSetFramebufferSizeCallback(Window, framebufferSizeCallback);
	glfwGetCursorPos(Window, &cursorXPos, &cursorYPos);
	glEnable(GL_BLEND);
	glfwSwapInterval(1);

	cursorXPosPrev = cursorXPos;
	cursorYPosPrev = cursorYPos;
	utils::GLErrorcheck("Window Creation", true);

	graphics::prepareOpenGL();
	double maxFrameTime = 1.0f/display::MAX_FREQ;
	prepareVoxelData();
	frame::updateSSBOS();
	for (int key : monitoredKeys) {
		keyMap[key] = false;
	}



	while (!glfwWindowShouldClose(Window)) {
		double frameStart = glfwGetTime();
		handleInputs();
		if (keyMap[GLFW_KEY_ESCAPE]) {
			break; //Quit
		}

		physics::cameraMove();
		if (dev::SHOW_VALUES) {
			printVec3(camera.position);
			printVec2(camera.viewAngle);
		}
		frame::draw();


		while (glfwGetTime() - frameStart < maxFrameTime) {std::this_thread::yield();}
		glfwSwapBuffers(Window);
		float freq = floor(1.0f / (glfwGetTime() - frameStart));
		if (dev::SHOW_FREQ_CONSOLE) {
			std::cout << freq << std::endl;
		}

		cursorXPosPrev = cursorXPos;
		cursorYPosPrev = cursorYPos;
	}


	glfwDestroyWindow(Window);
	glfwTerminate();
	return 0;


	//Catch exceptions.
	} catch (const std::exception& e) {
		if (!utils::isConsoleVisible()) {
			utils::showConsole();
		}
		std::cerr << "An exception was thrown: " << e.what() << std::endl;
		pause();
		return -1;
	} catch (...) {
		if (!utils::isConsoleVisible()) {
			utils::showConsole();
		}
		std::cerr << "An unspecified exception was thrown." << std::endl;
		pause();
		return -1;
	}
}
