#pragma once
#include "includes.h"
#include "constants.h"
using namespace std;


inline GLFWwindow* Window;


inline std::unordered_map<int, bool> keyMap = {
	//GLFW Enums mapped to boolean values (True if pressed.)
	{GLFW_KEY_ESCAPE, false}, //Example
};
inline glm::dvec2 cursorPosition, cursorPositionPrevious, cursorDelta;


inline glm::ivec2 currentWindowResolution;
inline glm::ivec2 currentRenderResolution;
inline float frameRate;
inline unsigned int frameNumber;


namespace GLIndex {

//Any indices required for OpenGL stuff.

}


namespace structs {

//Structs and other typedefs.

}
