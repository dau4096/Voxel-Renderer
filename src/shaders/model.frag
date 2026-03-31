/* model.frag */
#version 460 core

in vec3 gridPosition; //Where in the grid to start DDA from.
out vec4 fragColour; //Final fragment colour.

uniform mat4 cameraToWorldspaceMat; //Matrix inverse of pvmMat in the vertex shader.

#include <DDA> //Differential-Difference Analysis functions.



void main(void) {

	//TBA.
	fragColour = vec4(1.0f, 0.0f, 1.0f, 1.0f);

}
