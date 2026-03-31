/* DDA.glsl */
//Will be added into other files that use `#include <DDA>` by a preprocessor step.



bool checkVoxel(in ivec3 position, in ivec3 voxelGridSize, in ivec2 voxelDataIndices, out uint foundType) {
	uint index = voxelDataIndices.x + (
		(position.z * voxelGridSize.y * voxelGridSize.x) +
		(position.y * voxelGridSize.x) +
		position.x
	);
	if (
		((position.x < 0) || (position.x >= voxelGridSize.x)) || 
		((position.y < 0) || (position.y >= voxelGridSize.y)) || 
		((position.z < 0) || (position.z >= voxelGridSize.z))
	) {
		return false; //Outside grid.
	}
	if (index >= voxelDataIndices.y) {return false; /* Outside the data. */}

	uint gridValue = voxelData[index];

	if (gridValue > 0u) {
		//0 is empty, blank, none, etc.
		foundType = gridValue;
		return true;
	}
	return false;
}


bool differentialDifferenceAnalysis(
	in vec3 origin, in vec3 direction, in ivec3 gridSize, in ivec2 voxelDataIndices, out uint foundType, out vec3 position
) {
	ivec3 voxel = ivec3(floor(origin));
	ivec3 step = ivec3(sign(direction));

	//Stop DIV0
	vec3 invDir = 1.0 / max(abs(direction), vec3(1e-6));

	//Dist to cross 1 voxel
	vec3 tDelta = abs(invDir);

	//First voxel boundary
	vec3 nextBoundary = vec3(
		(direction.x > 0.0) ? float(voxel.x + 1) : float(voxel.x),
		(direction.y > 0.0) ? float(voxel.y + 1) : float(voxel.y),
		(direction.z > 0.0) ? float(voxel.z + 1) : float(voxel.z)
	);

	vec3 tMax = (nextBoundary - origin) / direction;


	//Traversal loop
	float t = 0.0f;
	while (true) {
		if (checkVoxel(voxel, gridSize, voxelDataIndices, foundType)) {
			position = origin + (t * direction);
			return true;
		}

		//Step accross to next voxel.
		float tNext = min(tMax.x, min(tMax.y, tMax.z));
		t = tNext;

		for (uint i=0; i<3; i++) { //For each component..
			if (tMax[i] == tNext) {
			    voxel[i] += step[i];
			    tMax[i] += tDelta[i];
			}
		}

		if (
			((voxel.x < 0) || (voxel.x >= gridSize.x)) || 
			((voxel.y < 0) || (voxel.y >= gridSize.y)) || 
			((voxel.z < 0) || (voxel.z >= gridSize.z))
		) {
			break; //Outside grid.
		}
	}

	return false;
}