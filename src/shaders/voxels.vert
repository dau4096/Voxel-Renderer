/* voxels.vert */
#version 460 core

layout(location=0) in vec3 aPos;
layout(location=1) in float aDirection;

out vec3 fragPosition;
out vec2 fragUV;
out flat int fragDirection;

uniform mat4 pvmMatrix;


const vec2 UVList[4] = {
	vec2(0.0f, 0.0f),
	vec2(1.0f, 0.0f),
	vec2(1.0f, 1.0f),
	vec2(0.0f, 1.0f)
};


void main() {
	fragDirection = int(aDirection);
	fragUV = UVList[gl_VertexID % 4];

	vec4 worldPos = pvmMatrix * vec4(aPos, 1.0);
	fragPosition = worldPos.xyz;
	gl_Position = worldPos;
}