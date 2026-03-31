#pragma once
#include "includes.h"
#include "constants.h"
using namespace std;


inline GLFWwindow* Window;


inline std::unordered_map<int, bool> keyMap = {
	//GLFW Enums mapped to boolean values (True if pressed.)
	{GLFW_KEY_ESCAPE, false},
	{GLFW_KEY_W, false}, {GLFW_KEY_S, false}, 
	{GLFW_KEY_A, false}, {GLFW_KEY_D, false}, 
	{GLFW_KEY_E, false}, {GLFW_KEY_Q, false}, 
	{GLFW_KEY_LEFT_SHIFT, false},
	{GLFW_KEY_LEFT_CONTROL, false}, 
};
inline glm::dvec2 cursorPosition, cursorPositionPrevious, cursorDelta;


inline glm::ivec2 currentWindowResolution;
inline glm::ivec2 currentRenderResolution;
inline float frameRate;
inline unsigned int frameNumber;


namespace GLIndex {

//Any indices required for OpenGL stuff.
inline GLuint emptyVAO;
inline GLuint modelShader;

}


namespace structs {

//Structs and other typedefs.
struct Camera {
	glm::vec3 position;
	glm::vec2 viewAngle;
	float FOV, nearZ, farZ;
	float height, vZ;

	Camera(glm::vec3 position, glm::vec2 angle, float FOV, float nearZ, float farZ, float height)
		: position(position), viewAngle(angle), FOV(FOV), nearZ(nearZ), farZ(farZ), height(height), vZ(0.0f) {}
};

}

inline structs::Camera camera = structs::Camera(
	glm::vec3(-2.0f, 0.0f, 1.0f), 	//Position
	glm::vec2(1.57f, -0.7854f), 	//Angle
	display::FOV, 0.1f, 96.0f, 		//FOV, zNear, zFar
	0.1875f							//Camera height over floor
);