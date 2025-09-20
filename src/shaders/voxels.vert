/* voxels.vert */
#version 460 core

layout(location=0) in vec3 aPos;
layout(location=1) in float aDirection;
layout(location=2) in vec2 aUV;

out vec3 facePosition;
out vec2 faceUV;
out flat int faceDirection;

uniform mat4 pvmMatrix;



void main() {
	faceDirection = int(aDirection);
	faceUV = aUV;

	facePosition = aPos;
	gl_Position = pvmMatrix * vec4(aPos, 1.0);
}