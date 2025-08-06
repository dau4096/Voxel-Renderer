#include "includes.h"
#include "global.h"
#include "utils.h"
using namespace std;
using namespace utils;
using namespace glm;


namespace uniforms {

//Uniforms; [Many overloads]
inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, bool value) {
	GLuint location = glGetUniformLocation(shaderProgram, uniformName);
	if (location >= 0) {
		glUniform1i(location, value);
	}
}
inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, size_t value) {
	GLuint location = glGetUniformLocation(shaderProgram, uniformName);
	if (location >= 0) {
		glUniform1i(location, value);
	}
}
inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, int value) {
	GLuint location = glGetUniformLocation(shaderProgram, uniformName);
	if (location >= 0) {
		glUniform1i(location, value);
	}
}
inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, float value) {
	GLuint location = glGetUniformLocation(shaderProgram, uniformName);
	if (location >= 0) {
		glUniform1f(location, value);
	}
}
inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, glm::ivec2 value) {
	GLuint location = glGetUniformLocation(shaderProgram, uniformName);
	if (location >= 0) {
		glUniform2i(location, value.x, value.y);
	}
}
inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, glm::vec2 value) {
	GLuint location = glGetUniformLocation(shaderProgram, uniformName);
	if (location >= 0) {
		glUniform2f(location, value.x, value.y);
	}
}
inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, glm::ivec3 value) {
	GLuint location = glGetUniformLocation(shaderProgram, uniformName);
	if (location >= 0) {
		glUniform3i(location, value.x, value.y, value.z);
	}
}
inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, glm::vec3 value) {
	GLuint location = glGetUniformLocation(shaderProgram, uniformName);
	if (location >= 0) {
		glUniform3f(location, value.x, value.y, value.z);
	}
}
inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, glm::ivec4 value) {
	GLuint location = glGetUniformLocation(shaderProgram, uniformName);
	if (location >= 0) {
		glUniform4i(location, value.x, value.y, value.z, value.w);
	}
}
inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, glm::vec4 value) {
	GLuint location = glGetUniformLocation(shaderProgram, uniformName);
	if (location >= 0) {
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}
}
inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, glm::mat4& value) {
	GLuint location = glGetUniformLocation(shaderProgram, uniformName);
	if (location >= 0) {
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
}

}


namespace graphics {

GLFWwindow* initializeWindow(int width, int height, const char* title) {
	if (!glfwInit()) {
		raise("Failed to initialize GLFW");
		return nullptr;
	}


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);  // Set OpenGL major version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);  // Set OpenGL minor version
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // Use Core profile


	GLFWwindow* Window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!Window) {
		glfwTerminate();
		raise("Failed to create GLFW window");
		return nullptr;
	}
	glfwMakeContextCurrent(Window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		raise("Failed to initialize GLEW.");
	}

	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return Window;
}



GLuint compileShader(GLenum shaderType, string filePath) {
	std::string source = utils::readFile(filePath);
	const char* src = source.c_str();

	// Create a shader object
	GLuint shader = glCreateShader(shaderType);
	if (shader == 0) {
		raise("Error: Failed to create shader.");
		return 0;
	}

	// Attach the shader source code to the shader object
	glShaderSource(shader, 1, &src, nullptr);

	// Compile the shader
	glCompileShader(shader);
	

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		if (!utils::isConsoleVisible()) {
			utils::showConsole();
		}
		char infolog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infolog);
		raise("Error: Shader compilation failed;\n" + string(infolog));
	}

	return shader;
}


GLuint createShaderProgram(std::string name, bool hasVertexSource=true) {
	GLuint vertexShader;
	if (hasVertexSource) {
		vertexShader = compileShader(GL_VERTEX_SHADER, "src\\shaders\\"+ name +".vert");
	} else {
		vertexShader = compileShader(GL_VERTEX_SHADER, "src\\shaders\\generic.vert");
	}
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, "src\\shaders\\"+ name +".frag");

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	GLint success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		if (!utils::isConsoleVisible()) {
			utils::showConsole();
		}
		char infolog[512];
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infolog);
		raise("Error: Program linking failed;\n" + string(infolog));
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}



static glm::mat4 getModelMat4(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) {
	glm::mat4 translationMat = glm::mat4(
		1.0f, 	0.0f, 	0.0f, 	pos.x,
		0.0f, 	1.0f, 	0.0f, 	pos.y,
		0.0f, 	0.0f, 	1.0f, 	pos.z,
		0.0f, 	0.0f, 	0.0f, 	1.0f
	);

	float sx = sin(rot.x), cx = cos(rot.x);
	float sy = sin(rot.y), cy = cos(rot.y);
	float sz = sin(rot.z), cz = cos(rot.z);
	glm::mat4 rotationMat = glm::mat4(
		cy*cz, cy*sz, -sy, 0.0f,
		sx*sy*cz-cx*sz, sx*sy*sz+cx*cz, sx*cy, 0.0f,
		cx*sy*cz+sx*sz, cx*sy*sz-sx*cz, cx*cy, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	glm::mat4 scaleMat = glm::mat4(
		scale.x,	0.0f, 		0.0f,		0.0f, 
		0.0f, 		scale.y,	0.0f, 		0.0f, 
		0.0f, 		0.0f, 		scale.z,	0.0f, 
		0.0f, 		0.0f, 		0.0f, 		1.0f
	);

	return rotationMat * scaleMat * translationMat;
}



enum FaceDirection {
	F_LOWER, F_UPPER,
	F_FRONT, F_BACK_,
	F_LEFT_, F_RIGHT
};



static inline float getBitsFromFaceDirection(FaceDirection fDir) {
	int iValue;
	switch (fDir) {
		case F_LOWER: {
			iValue = 0b000;
			break;
		}
		case F_UPPER: {
			iValue = 0b001;
			break;
		}

		case F_FRONT: {
			iValue = 0b000;
			break;
		}
		case F_BACK_: {
			iValue = 0b010;
			break;
		}

		case F_LEFT_: {
			iValue = 0b000;
			break;
		}
		case F_RIGHT: {
			iValue = 0b100;
			break;
		}

		default: {
			raise("Unknown FaceDirection.");
		}
	}

	return static_cast<float>(iValue);
}


void getVertsAndIndices(
		glm::vec3 position, glm::vec3 angle, glm::vec3 scale,
		std::array<float, 96>* vertices, std::array<GLuint, 36>* indices
	) {
	glm::mat4 matrix = getModelMat4(position, angle, scale);


	#define NEG -1.0f
	#define POS  1.0f


	std::array<float, 96> preVertices = {
		//Lower face;
		NEG, NEG, NEG, getBitsFromFaceDirection(F_LOWER),
		POS, NEG, NEG, getBitsFromFaceDirection(F_LOWER),
		NEG, POS, NEG, getBitsFromFaceDirection(F_LOWER),
		POS, POS, NEG, getBitsFromFaceDirection(F_LOWER),
		//Upper face;
		NEG, NEG, POS, getBitsFromFaceDirection(F_UPPER),
		POS, NEG, POS, getBitsFromFaceDirection(F_UPPER),
		NEG, POS, POS, getBitsFromFaceDirection(F_UPPER),
		POS, POS, POS, getBitsFromFaceDirection(F_UPPER),

		//-X face;
		NEG, NEG, NEG, getBitsFromFaceDirection(F_FRONT),
		NEG, POS, NEG, getBitsFromFaceDirection(F_FRONT),
		NEG, NEG, POS, getBitsFromFaceDirection(F_FRONT),
		NEG, POS, POS, getBitsFromFaceDirection(F_FRONT),
		//+X face;
		POS, NEG, NEG, getBitsFromFaceDirection(F_BACK_),
		POS, POS, NEG, getBitsFromFaceDirection(F_BACK_),
		POS, NEG, POS, getBitsFromFaceDirection(F_BACK_),
		POS, POS, POS, getBitsFromFaceDirection(F_BACK_),

		//-Y face;
		NEG, NEG, NEG, getBitsFromFaceDirection(F_LEFT_),
		POS, NEG, NEG, getBitsFromFaceDirection(F_LEFT_),
		NEG, NEG, POS, getBitsFromFaceDirection(F_LEFT_),
		POS, NEG, POS, getBitsFromFaceDirection(F_LEFT_),
		//+Y face;
		NEG, POS, NEG, getBitsFromFaceDirection(F_RIGHT),
		POS, POS, NEG, getBitsFromFaceDirection(F_RIGHT),
		NEG, POS, POS, getBitsFromFaceDirection(F_RIGHT),
		POS, POS, POS, getBitsFromFaceDirection(F_RIGHT),
	};

	for (int vIndex=0; vIndex<24; vIndex++) {
		int actualIndex = (vIndex * 4);
		glm::vec4 posVec = glm::vec4(
			preVertices[actualIndex + 0],
			preVertices[actualIndex + 1],
			preVertices[actualIndex + 2],
			1.0f
		);
		glm::vec4 newPosVec = posVec * matrix;
		(*vertices)[actualIndex + 0] = newPosVec.x;
		(*vertices)[actualIndex + 1] = newPosVec.y;
		(*vertices)[actualIndex + 2] = newPosVec.z;
		(*vertices)[actualIndex + 3] = preVertices[actualIndex + 3];
	}

	std::array<GLuint, 36> preIndices = {
		 0,  1,  2,  1,  2,  3,
		 4,  5,  6,  5,  6,  7,
		 8,  9, 10,  9, 10, 11,
		12, 13, 14, 13, 14, 15,
		16, 17, 18, 17, 18, 19,
		20, 21, 22, 21, 22, 23
	};
	*indices = preIndices;
}


GLuint getVAO(glm::vec3 position, glm::vec3 angle, glm::vec3 scale) {
	std::array<float, 96> vertices;
	std::array<GLuint, 36> indices;
	getVertsAndIndices(position, angle, scale, &vertices, &indices);


	//Create VAO
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Create VBO
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

	//Create EBO
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

	//Define position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Define direction attribute
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	return VAO;
}



GLuint createShaderStorageBufferObject(int binding, size_t bufferSize=0) {
	GLuint SSBO;
	glGenBuffers(1, &SSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, bufferSize, nullptr, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, SSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	return SSBO;
}

template<typename TGPU, typename TCPU>
void updateShaderStorageBufferObject(
		GLuint SSBO,
		std::vector<TCPU>* dataSetIn
	) {

	size_t singleItemSize = sizeof(TGPU);
	size_t size = dataSetIn->size();
	std::vector<TGPU> dataSet;

	for (size_t index=0; index<size; index++) {
		dataSet.push_back(TGPU(dataSetIn->at(index)));
	}

	if (size > 0 && !dataSet.empty()) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, singleItemSize * size, dataSet.data());
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
}



static inline glm::mat4 projectionMatrix() {
	float aspectRatio = float(currentRenderResolution.x) / float(currentRenderResolution.y);
	return glm::perspective(camera.FOV, aspectRatio, camera.nearZ, camera.farZ);
}

static inline glm::mat4 viewMatrix() {
	glm::vec3 forward = glm::vec3(
		sin(camera.viewAngle.x)*cos(camera.viewAngle.y),
		cos(camera.viewAngle.x)*cos(camera.viewAngle.y),
		sin(camera.viewAngle.y)
	);

	return glm::lookAt(camera.position, camera.position + forward, glm::vec3(0.0f, 0.0f, 1.0f));
}

glm::mat4 getPVMMatrix() {
	glm::mat4 pMat = projectionMatrix();
	glm::mat4 vMat = viewMatrix();
	glm::mat4 mMat = glm::mat4(1.0f); //Identity matrix.

	return pMat * vMat * mMat;
}


void APIENTRY openGLErrorCallback(
		GLenum source,
		GLenum type, GLuint id,
		GLenum severity,
		GLsizei length, const GLchar* message,
		const void* userParam
	) {
	/*
	Nicely formatted callback from;
	[https://learnopengl.com/In-Practice/Debugging]
	*/
	if(id == 131169 || id == 131185 || id == 131218 || id == 131204) {return;}

	std::cout << "---------------" << std::endl << "Debug message (" << id << ") | " << message << std::endl;

	switch (source)
	{
		case GL_DEBUG_SOURCE_API:             {std::cout << "Source: API"; break;}
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   {std::cout << "Source: Window System"; break;}
		case GL_DEBUG_SOURCE_SHADER_COMPILER: {std::cout << "Source: Shader Compiler"; break;}
		case GL_DEBUG_SOURCE_THIRD_PARTY:     {std::cout << "Source: Third Party"; break;}
		case GL_DEBUG_SOURCE_APPLICATION:     {std::cout << "Source: Application"; break;}
		case GL_DEBUG_SOURCE_OTHER:           {std::cout << "Source: Other"; break;}
	} std::cout << std::endl;

	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR:               {std::cout << "Type: Error"; break;}
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: {std::cout << "Type: Deprecated Behaviour"; break;}
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  {std::cout << "Type: Undefined Behaviour"; break;} 
		case GL_DEBUG_TYPE_PORTABILITY:         {std::cout << "Type: Portability"; break;}
		case GL_DEBUG_TYPE_PERFORMANCE:         {std::cout << "Type: Performance"; break;}
		case GL_DEBUG_TYPE_MARKER:              {std::cout << "Type: Marker"; break;}
		case GL_DEBUG_TYPE_PUSH_GROUP:          {std::cout << "Type: Push Group"; break;}
		case GL_DEBUG_TYPE_POP_GROUP:           {std::cout << "Type: Pop Group"; break;}
		case GL_DEBUG_TYPE_OTHER:               {std::cout << "Type: Other"; break;}
	} std::cout << std::endl;
	
	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:         {std::cout << "Severity: high"; break;}
		case GL_DEBUG_SEVERITY_MEDIUM:       {std::cout << "Severity: medium"; break;}
		case GL_DEBUG_SEVERITY_LOW:          {std::cout << "Severity: low"; break;}
		case GL_DEBUG_SEVERITY_NOTIFICATION: {std::cout << "Severity: notification"; break;}
	} std::cout << std::endl;
	std::cout << std::endl;

	utils::pause();
}


void prepareOpenGL() {
	//OpenGL setup;
	GLIndex::voxelSSBO = graphics::createShaderStorageBufferObject(
		0, sizeof(GLuint) * constants::VOXEL_GRID_SIZE
	);
	GLIndex::voxelVAO = graphics::getVAO(glm::vec3(-1.0f, 4.0f, 0.0f), glm::vec3(0.785398f, 0.785398f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));


	//Voxel shader
	GLIndex::voxelShader = graphics::createShaderProgram("voxels", true);


	glViewport(0, 0, currentWindowResolution.x, currentWindowResolution.y);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glClearDepth(1.0f);

	verticalFOV = 2.0f * atan(tan(display::FOV * 0.5f) * (float(currentRenderResolution.y) / float(currentRenderResolution.x)));
	

	//Debug settings
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(openGLErrorCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	utils::GLErrorcheck("Initialisation", true); //Old basic debugging
}


}




namespace frame {

void updateSSBOS() {
	//Update SSBOs.
	graphics::updateShaderStorageBufferObject<GLuint>(
		GLIndex::voxelSSBO, &voxelData
	);
	utils::GLErrorcheck("Updating SSBOs", true);
}

void draw() {
	glm::mat4 pvmMatrix = graphics::getPVMMatrix();

	//Update resolution
	glViewport(0, 0, currentRenderResolution.x, currentRenderResolution.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Voxel Shader.
	glUseProgram(GLIndex::voxelShader);

	//Uniforms
	uniforms::bindUniformValue(GLIndex::voxelShader, "pvmMatrix", pvmMatrix);
	uniforms::bindUniformValue(GLIndex::voxelShader, "voxelGridSize", constants::VOXEL_GRID_DIMENSIONS);

	glBindVertexArray(GLIndex::voxelVAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
	utils::GLErrorcheck("Voxel Shader", true);
}

}