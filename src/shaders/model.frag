/* model.frag */
#version 460 core

in vec3 gridPosition; //Where in the grid to start DDA from.
out vec4 fragColour; //Final fragment colour.

uniform mat4 invProjMat; //Matrix inverse of projMat.
uniform mat4 invViewMat; //Matrix inverse of viewMat.
uniform ivec2 resolution; //Screen resolution.

#include <DDA> //Differential-Difference Analysis functions.


vec3 getFragDirection() {
	//Get frag direction from its position onscreen.
	vec2 NDC = vec2(
		((float(gl_FragCoord.x) + 0.5f) / float(resolution.x)) * 2.0f - 1.0f,
		((float(gl_FragCoord.y) + 0.5f) / float(resolution.y)) * 2.0f - 1.0f
	);
	vec4 rayClip = vec4(NDC.xy, -1.0f, 1.0f); //Uses near plane.

	//To viewspace.
	vec4 rayView = invProjMat * rayClip;
	rayView /= rayView.w; //Perspecitve divide.

	//To worldspace.
	return normalize((invViewMat * vec4(rayView.xyz, 0.0f)).xyz);
}


void main(void) {

	//Get direction of ray.
	vec3 direction = getFragDirection();

	fragColour = vec4(direction.xyz, 1.0f);

}
