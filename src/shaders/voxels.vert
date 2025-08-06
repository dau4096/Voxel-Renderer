/* voxels.vert */
#version 460 core

layout(location=0) in vec3 aPos;
layout(location=1) in float aDirection;
layout(location=2) in vec2 aUV;

out vec3 fragPosition;
out vec2 fragUV;
out flat int fragDirection;

uniform mat4 pvmMatrix;



void main() {
	fragDirection = int(aDirection);
	fragUV = aUV;

	fragPosition = aPos;
	gl_Position = pvmMatrix * vec4(aPos, 1.0);
}