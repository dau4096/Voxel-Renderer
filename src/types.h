/* types.h */
#ifndef TYPES_H
#define TYPES_H


#include <cstdint>
#include "includes.h"


namespace types {

struct Model {
	//Model translation/rotation/scale
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	//Voxel data
	glm::ivec3 dimensions;
	const uint8_t* data;

	unsigned int dataStartIDX;
	unsigned int dataEndIDX;

	Model() : position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f), scale(0.0f, 0.0f, 0.0f), dimensions(0, 0, 0), data() {}

	Model(const glm::vec3 p, const glm::vec3 r, const glm::vec3 s, const glm::ivec3 dim, const uint8_t* dat)
		: position(p), rotation(r), scale(s), dimensions(dim), data(dat) {}
};


struct Camera {
	glm::vec3 position;
	glm::vec2 viewAngle;
	float FOV, nearZ, farZ;

	Camera(glm::vec3 position, glm::vec2 angle, float FOV, float nearZ, float farZ)
		: position(position), viewAngle(angle), FOV(FOV), nearZ(nearZ), farZ(farZ) {}
};

}



#endif
