#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image.h>
#include <stb_image_write.h>
#include "src/includes.h"
#include "src/global.h"
#include "src/utils.h"
#include "src/graphics.h"
using namespace std;
using namespace utils;
using namespace glm;




void framebufferSizeCallback(GLFWwindow* Window, int width, int height) {
	glViewport(0, 0, width, height);
#ifdef SCREENSPACE_ONLY
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
#endif

	currentWindowResolution = glm::ivec2(width, height);
	currentRenderResolution = glm::ivec2(
		glm::min(width, display::RENDER_RESOLUTION.x),
		glm::min(height, display::RENDER_RESOLUTION.y)
	);
}



void handleInputs() {
	glfwPollEvents();

	//Get keyboard inputs for this frame
	for (std::pair<int, bool> pair : keyMap) {
		int keyState = glfwGetKey(Window, pair.first);
		if (keyState == GLFW_PRESS) {keyMap[pair.first] = true;}
		else if (keyState == GLFW_RELEASE) {keyMap[pair.first] = false;}
	}


	//Mouse controls;
	cursorDelta = cursorPosition - cursorPositionPrevious;
}



//#define SCREENSPACE_ONLY //Only 2D scenes.
//#define WORLDSPACE_ONLY //Only 3D scenes.



int main() {
	try { //Catch exceptions

#ifdef __WIN32
	SetConsoleOutputCP(65001); //CP_UTF8, Windows.
#else
	#pragma execution_character_set("utf-8") //Linux.
#endif

	currentWindowResolution = display::WINDOW_RESOLUTION;
	currentRenderResolution = glm::ivec2(
		glm::min(display::WINDOW_RESOLUTION.x, display::RENDER_RESOLUTION.x),
		glm::min(display::WINDOW_RESOLUTION.y, display::RENDER_RESOLUTION.y)
	);

	Window = graphics::initialiseWindow(display::WINDOW_RESOLUTION, "Graphics-Template/OpenGL");
	glfwSetFramebufferSizeCallback(Window, framebufferSizeCallback);
	glfwGetCursorPos(Window, &cursorPosition.x, &cursorPosition.y);
	glEnable(GL_BLEND);
	glfwSwapInterval((dev::VSYNC) ? 1 : 0);



	cursorPositionPrevious.x = cursorPosition.x;
	cursorPositionPrevious.y = cursorPosition.y;
	utils::GLErrorcheck("Window Creation", true);

	graphics::prepareOpenGL();


	frameNumber = 0u;
	while (!glfwWindowShouldClose(Window)) {
		double frameStart = glfwGetTime();
		handleInputs();
		if (keyMap[GLFW_KEY_ESCAPE]) {break; /* Quit Immediately, ESC pressed. */}


		//Rendering logic here.
		frame::draw();


		float dt = glfwGetTime() - frameStart;
		if (dev::SHOW_DT_CONSOLE) {std::cout << "Frame #" << frameNumber << " took " << std::setprecision(2) << (dt * 1e3f) << "ms / Hypothetical framerate: " << static_cast<int>(1.0f / dt) << endl;}
		if (!dev::VSYNC) {while ((glfwGetTime() - frameStart) < display::DT) {std::this_thread::yield();} /* Wait. */}
		frameRate = ceil(1.0f / (glfwGetTime() - frameStart));
		if (dev::SHOW_HZ_CONSOLE) {std::cout << "Framerate: " << frameRate << "Hz" << std::endl;}

		cursorPositionPrevious = cursorPosition;
		frameNumber++;
	}


	//Cleanup and exit.
	glfwDestroyWindow(Window);
	glfwTerminate();
	return 0;


	//Catch exceptions.
	} catch (const std::exception& e) {
		if (!utils::isConsoleVisible()) {utils::showConsole();}
		std::cerr << "An exception was thrown: " << e.what() << std::endl;
		utils::pause();
		return -1;
	} catch (...) {
		if (!utils::isConsoleVisible()) {utils::showConsole();}
		std::cerr << "An unspecified exception was thrown." << std::endl;
		utils::pause();
		return -1;
	}
}
