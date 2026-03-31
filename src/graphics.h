#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "includes.h"
#include "global.h"
#include "utils.h"

using namespace glm;




namespace uniforms {

//Uniforms; [Many overloads]
static inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, bool value);
static inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, size_t value);
static inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, int value);
static inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, float value);
static inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, glm::ivec2 value);
static inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, glm::vec2 value);
static inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, glm::ivec3 value);
static inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, glm::vec3 value);
static inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, glm::ivec4 value);
static inline void bindUniformValue(GLuint shaderProgram, const GLchar* uniformName, glm::vec4 value);

}




namespace graphics {
	GLFWwindow* initialiseWindow(glm::ivec2 resolution, const char* title);
	static inline GLuint createShaderProgram(std::string fragShaderName, std::string vertexShaderName);
	GLuint createComputeShader(std::string compShaderName);


	GLuint createShaderStorageBufferObject(int binding, size_t bufferSize=0, GLuint glType=GL_DYNAMIC_DRAW);
	//Different types for GPU and CPU (padding, unecessary data removed.)
	template<typename TGPU, typename TCPU>  void updateShaderStorageBufferObject(GLuint SSBO, std::vector<TCPU>* dataSetIn, int allocSize);
	//Overload of the above without the size specified.
	template<typename TGPU, typename TCPU>  void updateShaderStorageBufferObject(GLuint SSBO, std::vector<TCPU>* dataSetIn);
	//Same type for CPU & GPU.
	template<typename T> 					void updateShaderStorageBufferObject(GLuint SSBO, T* data, size_t count);


	//// TEXTURES ////
	void saveImage(GLuint textureID, bool silent=false);
	GLuint createGLImage2D(size_t width, size_t height, GLint internalFormat=GL_RGBA32F, GLint samplingType=GL_NEAREST, GLint edgeSampling=GL_REPEAT);
	GLuint loadGLTexture2D(const std::string textureName, glm::ivec2 expectedRes=glm::ivec2(-1, -1));
	GLuint createTexture2DArray(std::vector<std::string>& textureNames, glm::ivec2 expectedRes=display::TEXTURE_RESOLUTION);
	GLuint createGLImage2DArray(glm::ivec3 resolution, GLenum filtering=GL_NEAREST);
	//// TEXTURES ////


	inline GLuint getEmptyVAO();


	//// FRAMEBUFFERS ////
	//Without depth texture
	GLuint createAFBO(glm::uvec2 resolution, GLuint& colourTexture);
	//With depth texture
	GLuint createADFBO(glm::uvec2 resolution, GLuint& colourTexture, GLuint& depthTexture);
	//// FRAMEBUFFERS ////


	//// ATOMIC ////
	GLuint zero = 0u;
	void fetchAndClearAtomic(GLuint atomicCounter, GLuint* counterValue);
	GLuint createAtomicCounter(unsigned int binding);
	//// ATOMIC ////


	void prepareOpenGL();
}


namespace frame {

	inline void renderingGeneric(const std::string& shaderName="");

	void draw();

}

#endif
