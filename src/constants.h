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


	//Camera values.
	constexpr float MOVE_SPEED_BASE = 0.0125f;
	constexpr float MOVE_SPEED_CROUCH_MULT = 0.25f;
	constexpr float MOVE_SPEED_RUN_MULT = 3.0f;
	constexpr float CAMERA_TURN_SPEED = 0.125f;
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


	//Camera FOV
	constexpr float FOV = 70.0f * constants::TO_RAD;
}

namespace dev {
	//Assorted DEV/DEBUG constants
	constexpr bool PAUSE_ON_OPENGL_ERROR = true;
	constexpr bool SHOW_HZ_CONSOLE = false;
	constexpr bool SHOW_DT_CONSOLE = false;
	constexpr bool VSYNC = false;
}