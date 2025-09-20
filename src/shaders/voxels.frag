/* voxels.frag */
#version 460 core

in vec3 facePosition;
in vec2 faceUV;
in flat int faceDirection;

out vec4 fragColour;


uniform ivec3 voxelGridSize;
uniform ivec2 renderResolution;
uniform vec2 cameraRotation;
uniform vec3 cameraPosition;
uniform vec2 FOV;


layout(std430, binding=0) buffer voxelSSBO {
	uint[] voxelData;
};



vec3 getRayDirection(vec2 normPosition, out float distanceModifier) {
	vec2 angleOffset = normPosition * FOV * 0.5f;
	distanceModifier = cos(angleOffset.x) * cos(angleOffset.y);
	vec2 worldAngle = cameraRotation + angleOffset;
	return normalize(vec3(
		sin(worldAngle.x) * cos(worldAngle.y),
		cos(worldAngle.x) * cos(worldAngle.y),
		sin(worldAngle.y)
	));
}



int getIndex(ivec3 position) {
	return position.x + position.y * voxelGridSize.x + position.z * voxelGridSize.x * voxelGridSize.y;
}

float searchGridViaDDA(vec3 rayDir, out bool didHitVoxel, out uint hitType) {
	vec3 voxelPosition;
	switch(faceDirection) {
		case 0: { //Bottom face.
			voxelPosition = vec3(floor(vec2(voxelGridSize.xy) * faceUV.xy), 0.0f);
			break;
		}
		case 1: { //Top face.
			voxelPosition = vec3(floor(vec2(voxelGridSize.xy) * faceUV.xy), voxelGridSize.z);
			break;
		}
		case 2: { //-X face.
			voxelPosition = vec3(0.0f, floor(vec2(voxelGridSize.yz) * faceUV.xy));
			break;
		}
		case 3: { //+X face.
			voxelPosition = vec3(voxelGridSize.x, floor(vec2(voxelGridSize.yz) * faceUV.xy));
			break;
		}
		case 4: { //-Y face.
			vec2 startXZ = floor(vec2(voxelGridSize.xz) * faceUV.xy);
			voxelPosition = vec3(startXZ.x, 0.0f, startXZ.y);
			break;
		}
		case 5: { //+Y face.
			vec2 startXZ = floor(vec2(voxelGridSize.xz) * faceUV.xy);
			voxelPosition = vec3(startXZ.x, voxelGridSize.y, startXZ.y);
			break;
		}
	}

	float maxAxis = max(rayDir.x, max(rayDir.y, rayDir.z));
	int delta;
	if (maxAxis == rayDir.x) {
		delta = voxelGridSize.x;
	} else if (maxAxis == rayDir.y) {
		delta = voxelGridSize.y;
	} else {
		delta = voxelGridSize.z;
	}

	float t = 0.0f;
	for (int d=0; d<delta; d++) {
		ivec3 voxelPositionIV3 = ivec3(floor(voxelPosition));
		int index = getIndex(voxelPositionIV3);
		if (any(lessThan(voxelPositionIV3, ivec3(0))) ||
			any(greaterThanEqual(voxelPositionIV3, voxelGridSize))) {
			voxelPosition += rayDir;
			t++;
			continue;
		}
		uint voxelValue = voxelData[index];
		if (voxelValue != 0u) {
			//Voxel was not empty;
			didHitVoxel = true;
			hitType = voxelValue;
			return t;
		}


		voxelPosition += rayDir;
		t++;
	}

	return 0.0f;
}




void main() {
	vec2 screenPosition = gl_FragCoord.xy;
	vec2 normPosition = screenPosition / vec2(renderResolution);
	float distanceModifier;
	vec3 rayDirectionWorld = getRayDirection(normPosition, distanceModifier);

	bool didHitVoxel;
	uint hitType;
	float fragDistance = searchGridViaDDA(rayDirectionWorld, didHitVoxel, hitType);
	fragDistance *= distanceModifier;


	gl_FragDepth = fragDistance;
	fragColour = vec4((didHitVoxel) ? vec3(1.0f, 0.0f, 1.0f) : vec3(faceUV.xy, 0.0f), 1.0f);
	//fragColour = vec4(fragDistance, 0.0f, 0.0f, 1.0f);
}