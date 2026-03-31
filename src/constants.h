#pragma once

#include "includes.h"
#include <glm/glm.hpp>




namespace constants {
	static bool C_TRUE = true;
	static bool C_FALSE = false;


	//Mathematical Constants
	constexpr float PI = 3.141592f;
	constexpr float PI2 = PI * 2.0f;
	constexpr float EXP = 2.718281f;
	constexpr float INF = std::numeric_limits<float>::infinity();

	constexpr float TO_RAD = 0.017453f;
	constexpr float TO_DEG = 57.29577f;

}

namespace display {
	//Resolutions
	constexpr glm::ivec2 WINDOW_RESOLUTION = glm::ivec2(640, 360);
	constexpr glm::ivec2 RENDER_RESOLUTION = glm::ivec2(640, 360);

	//Opengl 460 core.
	constexpr int OPENGL_VERSION_MAJOR = 4;
	constexpr int OPENGL_VERSION_MINOR = 6;

	//Texture Standardisation
	constexpr glm::ivec2 TEXTURE_RESOLUTION = glm::ivec2(128, 128);

	//Time
	constexpr double HZ = 60.0d;
	constexpr double DT = 1.0f/HZ;
}

namespace dev {
	//Assorted DEV/DEBUG constants
	constexpr bool PAUSE_ON_OPENGL_ERROR = true;
	constexpr bool SHOW_HZ_CONSOLE = true;
	constexpr bool SHOW_DT_CONSOLE = true;
	constexpr bool VSYNC = false;
}