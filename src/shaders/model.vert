/* model.vert */
#version 460 core


out vec3 gridPosition; //Position of this fragment in the grid, to start DDA from.


uniform mat4 pvmMat; //Projection, View, Model values (Translation, Rotation, Scale.)
uniform ivec3 voxelGridScale; //Size of voxel grid.


//Vertices
const vec3 vertices[8] = {
	vec3(-1.0f, -1.0f, -1.0f), //-XYZ
	vec3( 1.0f, -1.0f, -1.0f), //+X, -YZ
	vec3(-1.0f,  1.0f, -1.0f), //+Y, -XZ
	vec3( 1.0f,  1.0f, -1.0f), //+XY, -Z

	vec3(-1.0f, -1.0f,  1.0f), //+Z, -XY
	vec3( 1.0f, -1.0f,  1.0f), //+XZ, -Y
	vec3(-1.0f,  1.0f,  1.0f), //+YZ, -X
	vec3( 1.0f,  1.0f,  1.0f)  //+XYZ
};

//Taken from: [https://www.cs.umd.edu/gvil/papers/av_ts.pdf]
const uint indices[14] = {
	4u, 3u, 7u, 8u, 5u, 3u, 1u,
	4u, 2u, 7u, 6u, 5u, 2u, 1u
};


void main(void) {
	//gl_VertexID from 0-7. Uses GL_TRIANGLE_STRIP.
	//Maps to indices 0-13.
	vec3 v = vertices[indices[gl_VertexID]];
	gl_Position = (pvmMat * vec4(v.xyz, 1.0f));

	gridPosition = (
		(v * 0.5f + 0.5f) * vec3(voxelGridScale) //Convert to [0-1] range, multiply by grid scale.
	);
}

