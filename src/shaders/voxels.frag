/* voxels.frag */
#version 460 core

in vec3 fragPosition;
in vec2 fragUV;
in flat int fragDirection;

out vec4 fragColour;


uniform ivec3 voxelGridSize;

layout(std430, binding=0) buffer voxelSSBO {
	uint[] voxelData;
};


void main() {
	fragColour = vec4(fragUV.xy, 0.0f, 1.0f);
}