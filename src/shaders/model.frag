/* model.frag */
#version 460 core

in vec3 gridPosition; //Where in the grid to start DDA from.
out vec4 fragColour; //Final fragment colour.

uniform mat4 invProjMat; //Matrix inverse of projMat.
uniform mat4 invViewMat; //Matrix inverse of viewMat.

#include <DDA> //Differential-Difference Analysis functions.



void main(void) {

	//TBA.
	fragColour = vec4(gridPosition.xyz, 1.0f);

}
