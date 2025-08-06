#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "includes.h"
#include "global.h"
#include "utils.h"


namespace uniforms {

//Uniforms; [Many overloads]
inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, bool value);
inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, size_t value);
inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, int value);
inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, float value);
inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, glm::ivec2 value);
inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, glm::vec2 value);
inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, glm::ivec3 value);
inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, glm::vec3 value);
inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, glm::ivec4 value);
inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, glm::vec4 value);
inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, glm::mat4& value);

}



namespace graphics {
	GLFWwindow* initializeWindow(int width, int height, const char* title);
	GLuint createShaderProgram(std::string name, bool hasVertexSource=true);


	
	GLuint createShaderStorageBufferObject(int binding, size_t bufferSize=0);

	template<typename TGPU, typename TCPU>
	void updateShaderStorageBufferObject(GLuint SSBO, std::vector<TCPU>* dataSetIn);


	GLuint getVAO(glm::vec3 position, glm::vec3 angle, glm::vec3 scale);
	glm::mat4 getPVMMatrix();


	void prepareOpenGL();
}


namespace frame {

	void updateSSBOS();

	void draw();

}

#endif
