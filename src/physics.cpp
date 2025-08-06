#include "includes.h"
#include "global.h"
#include "utils.h"
using namespace std;
using namespace utils;




namespace physics {

void cameraMove() {
	float cameraSpeed = constants::MOVE_SPEED_BASE;
	if (keyMap[GLFW_KEY_LEFT_CONTROL]) {
		cameraSpeed *= constants::MOVE_SPEED_CROUCH_MULT;
	} else if (keyMap[GLFW_KEY_LEFT_SHIFT]) {
		cameraSpeed *= constants::MOVE_SPEED_RUN_MULT;
	}

	//Move camera
	if (keyMap[GLFW_KEY_W]) {
		camera.position.x += cameraSpeed * sin(camera.viewAngle.x);
		camera.position.y += cameraSpeed * cos(camera.viewAngle.x);
	}
	if (keyMap[GLFW_KEY_S]) {
		camera.position.x -= cameraSpeed * sin(camera.viewAngle.x);
		camera.position.y -= cameraSpeed * cos(camera.viewAngle.x);
	}
	if (keyMap[GLFW_KEY_A]) {
		camera.position.x -= cameraSpeed *  cos(camera.viewAngle.x);
		camera.position.y -= cameraSpeed * -sin(camera.viewAngle.x);
	}
	if (keyMap[GLFW_KEY_D]) {
		camera.position.x += cameraSpeed *  cos(camera.viewAngle.x);
		camera.position.y += cameraSpeed * -sin(camera.viewAngle.x);
	}
	if (keyMap[GLFW_KEY_E]) {
		camera.position.z += cameraSpeed;
	}
	if (keyMap[GLFW_KEY_Q]) {
		camera.position.z -= cameraSpeed;
	}
};


}