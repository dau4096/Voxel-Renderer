/* model.frag */
#version 460 core

in vec3 gridPosition; //Where in the grid to start DDA from.
out vec4 fragColour; //Final fragment colour.


//General values
uniform mat4 pvmMat; //Projection, View, Model values (Translation, Rotation, Scale.)
uniform mat4 invProjMat; //Matrix inverse of projMat.
uniform mat4 invViewMat; //Matrix inverse of viewMat.
uniform mat4 invRotationMat; //Matrix inverse of rotationMatrix.
uniform ivec2 resolution; //Screen resolution.

//Model values
uniform ivec3 voxelGridSize; //Size of voxel grid.
uniform ivec2 voxelDataIndices; //X: Start, Y: End


layout(binding=0) buffer voxelDataSSBO {
	uint voxelData[];
};


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
	vec3 direction = (invRotationMat * vec4(getFragDirection().xyz, 1.0f)).xyz;

	uint foundType; //Type of voxel, if one hit.
	vec3 voxelPosition; //Position within grid of the voxel. Is not floored to grid.
	bool foundCollision = differentialDifferenceAnalysis(gridPosition, direction, voxelGridSize, voxelDataIndices, foundType, voxelPosition);
	if (!foundCollision) {discard;}

	//Overwrite gl_FragDepth?
	vec3 voxelWorldPosition = gridPosition + voxelPosition; //Origin + pos inside grid
	vec4 clipPos = (pvmMat * vec4(voxelWorldPosition.xyz, 1.0f));
	float newDepth = clipPos.z / clipPos.w * 0.5f + 0.5f;
	if (newDepth >= gl_FragDepth) {discard; /* Covered inside the bounding box. */}
	gl_FragDepth = newDepth;

	//fragColour = vec4(direction.xyz, 1.0f);
	fragColour = vec4(voxelPosition.xyz / vec3(voxelGridSize.xyz), 1.0f);

}
