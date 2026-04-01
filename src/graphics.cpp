#include "includes.h"
#include "global.h"
#include "utils.h"
#include "loader.h"
#include <stb_image.h>
#include <stb_image_write.h>
using namespace std;
using namespace utils;
using namespace glm;








//////// DEBUG ////////
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
	if ((id == 131169u) || (id == 131185u) || (id == 131218u) || (id == 131204u)) {return; /* Ignored warning IDs that are not errors */}

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
	} std::cout << std::endl << std::endl;

	if (dev::PAUSE_ON_OPENGL_ERROR) {
		utils::pause();
	}
}
//////// DEBUG ////////
















//////// SHADER COMPILATION ////////
static unsigned int lineNumberAt(const std::string& s, size_t pos) {
	//Find [#line] number from position
	return std::count(s.begin(), s.begin() + pos, '\n');
}

std::string preprocessIncludes(const std::string& source, const std::string& currentFile) {
	std::regex includeRegex(R"(^\s*#include\s*<([^>]+)>)", std::regex_constants::multiline);

	std::string result;
	std::sregex_iterator it(source.begin(), source.end(), includeRegex);
	std::sregex_iterator end;

	size_t lastPos = 0;
	for (; it!=end; it++) {
		const std::smatch& match = *it;

		//Copy text before include
		result.append(source.substr(lastPos, match.position() - lastPos));

		std::string includeFile = match[1].str();
		std::string includePath = "src/shaders/" + includeFile + ".glsl";

		std::string includedSource = utils::readFile(includePath);

		unsigned int includeLine = lineNumberAt(source, match.position());

	#ifdef LINE_DIRECTIVE_STRING
		//Can be format `#line [lnNum] [srcFile]`
		result += "#line 1 \"src/shaders/"+includeFile+".glsl\"\n"+includedSource+"\n"+"#line "+std::to_string(includeLine+1u)+" \""+currentFile+"\"\n";
	#else
		//Must be of format `#line [lnNum]`
		result += "#line 1 \n"+includedSource+"\n"+"#line "+std::to_string(includeLine+1u)+" \n";
	#endif

		lastPos = match.position() + match.length();
	}

	// Append remaining source
	result.append(source.substr(lastPos));

	return result;
}



GLuint compileShader(GLenum shaderType, string filePath) {
	std::string source = utils::readFile(filePath);
	source = preprocessIncludes(source, filePath);
	const char* src = source.c_str();

	//Create a shader id
	GLuint shader = glCreateShader(shaderType);
	if (shader == 0) {
		raise("Error: Failed to create shader.");
		return 0;
	}

	//Attach the shader src
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);
	

	//Errorcheck
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
//////// SHADER COMPILATION ////////



















namespace uniforms {

//Uniforms; [Many overloads]
static inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, bool value) {
	GLuint location = glGetUniformLocation(shaderProgram, uniformName);
	if (location >= 0) {
		glUniform1i(location, value);
	}
}
static inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, size_t value) {
	GLuint location = glGetUniformLocation(shaderProgram, uniformName);
	if (location >= 0) {
		glUniform1ui(location, value);
	}
}
static inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, int value) {
	GLuint location = glGetUniformLocation(shaderProgram, uniformName);
	if (location >= 0) {
		glUniform1i(location, value);
	}
}
static inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, float value) {
	GLuint location = glGetUniformLocation(shaderProgram, uniformName);
	if (location >= 0) {
		glUniform1f(location, value);
	}
}
static inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, glm::ivec2 value) {
	GLuint location = glGetUniformLocation(shaderProgram, uniformName);
	if (location >= 0) {
		glUniform2i(location, value.x, value.y);
	}
}
static inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, glm::vec2 value) {
	GLuint location = glGetUniformLocation(shaderProgram, uniformName);
	if (location >= 0) {
		glUniform2f(location, value.x, value.y);
	}
}
static inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, glm::ivec3 value) {
	GLuint location = glGetUniformLocation(shaderProgram, uniformName);
	if (location >= 0) {
		glUniform3i(location, value.x, value.y, value.z);
	}
}
static inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, glm::vec3 value) {
	GLuint location = glGetUniformLocation(shaderProgram, uniformName);
	if (location >= 0) {
		glUniform3f(location, value.x, value.y, value.z);
	}
}
static inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, glm::ivec4 value) {
	GLuint location = glGetUniformLocation(shaderProgram, uniformName);
	if (location >= 0) {
		glUniform4i(location, value.x, value.y, value.z, value.w);
	}
}
static inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, glm::vec4 value) {
	GLuint location = glGetUniformLocation(shaderProgram, uniformName);
	if (location >= 0) {
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}
}
inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, const glm::mat4& value) {
	GLuint location = glGetUniformLocation(shaderProgram, uniformName);
	if (location >= 0) {
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
}

}









namespace graphics {

GLFWwindow* initialiseWindow(glm::ivec2 resolution, const char* title) {
	if (!glfwInit()) {
		raise("Failed to initialize GLFW");
		return nullptr;
	}


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, display::OPENGL_VERSION_MAJOR);  //OpenGL major ver (4)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, display::OPENGL_VERSION_MINOR);  //OpenGL minor ver (6)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //Use Core (not ES)


	GLFWwindow* Window = glfwCreateWindow(resolution.x, resolution.y, title, NULL, NULL);
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










//////// SHADER COMPILATION ////////
GLuint createShaderProgram(std::string vertexShaderName, std::string fragShaderName) {
	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, "src/shaders/"+ vertexShaderName);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, "src/shaders/"+ fragShaderName);

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
//////// SHADER COMPILATION ////////










//////// SSBOs ////////
//Data added once, never changes.
template<typename T>
GLuint createShaderStorageBufferObjectStatic(int binding, T* ptr, size_t bufferSize=0u) {
	GLuint SSBO;
	glGenBuffers(1, &SSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, bufferSize, ptr, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, SSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	return SSBO;
}


//Data changes at any time.
GLuint createShaderStorageBufferObjectDynamic(int binding, size_t bufferSize=0u) {
	GLuint SSBO;
	glGenBuffers(1, &SSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, bufferSize, nullptr, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, SSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	return SSBO;
}

//Update dynamic SSBO.
template<typename T>
void updateShaderStorageBufferObjectDynamic(
	GLuint SSBO,
	T* data,
	size_t count
) {
	size_t size = sizeof(T) * count;

	if (count > 0) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, data);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
}
//////// SSBOs ////////











//////// INITIALISATION ////////
inline GLuint getEmptyVAO() {
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	return VAO;
}


void prepareOpenGL() {
	//OpenGL setup;
	glViewport(0, 0, display::RENDER_RESOLUTION.x, display::RENDER_RESOLUTION.y);

	//Depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glClearDepth(1.0f);

	//Culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Shaders
	GLIndex::modelShader = createShaderProgram("model.vert", "model.frag");

	//Empty VAO
	GLIndex::emptyVAO = getEmptyVAO();

	//Voxel SSBO
	std::vector<std::string> modelNames = {
		"shotgun", "rifle", "blowtorch", "sledge"
	};
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
	for (const std::string& modelName : modelNames) {load::modelsFromFile("models/" + modelName + ".vox", pos); pos.y += 2.0f;}
	std::vector<GLuint> vData = {}; //0 is "air", or "none".
	for (types::Model& model : models) {
		model.dataStartIDX = vData.size();
		vData.insert(vData.end(), model.data, model.data+(model.dimensions.x*model.dimensions.y*model.dimensions.z));
		model.dataEndIDX = vData.size();
	}
	GLIndex::voxelDataSSBO = createShaderStorageBufferObjectStatic(
		0, vData.data(), vData.size()*sizeof(GLuint) //Voxel data size is 5×5×6 uints.
	);


	//Debug settings
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(openGLErrorCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	utils::GLErrorcheck("Initialisation", true); //Old basic debugging
}
//////// INITIALISATION ////////





//////// MATRICES ////////
static inline glm::mat4 projectionMatrix() {
	float aspectRatio = float(currentRenderResolution.x) / float(currentRenderResolution.y);
	float verticalFOV = 2 * atan(tan(camera.FOV * 0.5f) * (float(currentRenderResolution.y) / float(currentRenderResolution.x)));
	return glm::perspective(verticalFOV, aspectRatio, camera.nearZ, camera.farZ);
}

static inline glm::mat4 viewMatrix() {
	glm::vec3 forward = glm::vec3(
		sin(camera.viewAngle.x)*cos(camera.viewAngle.y),
		cos(camera.viewAngle.x)*cos(camera.viewAngle.y),
		sin(camera.viewAngle.y)
	);

	return glm::lookAt(camera.position, camera.position + forward, glm::vec3(0.0f, 0.0f, 1.0f));
}

glm::mat4 getModelMatrix(
	const glm::vec3& position, const glm::mat4& rotationMatrix, const glm::vec3& scale
) {
	//Rotate, Scale, Translate.
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    modelMatrix = glm::translate(modelMatrix, position);

    modelMatrix *= rotationMatrix;

    modelMatrix = glm::scale(modelMatrix, scale);

	return modelMatrix;
}
//////// MATRICES ////////


}











namespace frame {


void drawVoxelModel(
	const types::Model& model, const glm::mat4& pvMat, const glm::mat4& invProjMat, const glm::mat4& invViewMat
) {
	//Draws a modelMatrix at some pos/rot/scale.
	glm::mat4 rotationMat = glm::mat4(1.0f);
    rotationMat = glm::rotate(rotationMat, model.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    rotationMat = glm::rotate(rotationMat, model.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    rotationMat = glm::rotate(rotationMat, model.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));


	glm::mat4 modelMatrix = graphics::getModelMatrix(model.position, rotationMat, glm::normalize(glm::vec3(model.dimensions)));
	glm::mat4 pvmMat = pvMat * modelMatrix;

	glm::mat4 invRotMat = glm::inverse(rotationMat);

	//Run with GL_TRIANGLE_STRIP and 16 indices (To make a cuboid)
	glUseProgram(GLIndex::modelShader);
	glBindVertexArray(GLIndex::emptyVAO); //Vertex shader defines vertices.

	//General values
	uniforms::bindUniformValue(GLIndex::modelShader, "pvmMat", pvmMat);
	uniforms::bindUniformValue(GLIndex::modelShader, "invProjMat", invProjMat);
	uniforms::bindUniformValue(GLIndex::modelShader, "invViewMat", invViewMat);
	uniforms::bindUniformValue(GLIndex::modelShader, "invRotationMat", invRotMat);
	uniforms::bindUniformValue(GLIndex::modelShader, "resolution", currentWindowResolution);

	//Model values
	uniforms::bindUniformValue(GLIndex::modelShader, "voxelGridSize", model.dimensions);
	uniforms::bindUniformValue(GLIndex::modelShader, "voxelDataIndices", glm::ivec2((int)(model.dataStartIDX), (int)(model.dataEndIDX)));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 16);
}


void draw() {
	//Per-frame.
	glViewport(0, 0, currentWindowResolution.x, currentWindowResolution.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projMat = graphics::projectionMatrix();
	glm::mat4 viewMat = graphics::viewMatrix();
	glm::mat4 pvMat = projMat * viewMat;

	glm::mat4 invProjMat = glm::inverse(projMat);
	glm::mat4 invViewMat = glm::inverse(viewMat);

	for (types::Model& model : models) {
		drawVoxelModel(
			model, pvMat, invProjMat, invViewMat //Model & matrices
		);
		model.rotation.y += 0.0125f;
		model.rotation.x += 0.00625f;
	}


	glBindVertexArray(0);
	glUseProgram(0);
}



}