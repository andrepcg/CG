#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "OpenGLincludes.h"
#include "math_3d.h"


class ShaderProgram {
private:
	GLuint programID;

public:
	ShaderProgram(const char* vertexFilePath, const char* fragmentFilePath);
	~ShaderProgram();
	void use();
	void stopUsing();
	void ShaderProgram::SetWVP(const GLchar* name, const Matrix4f& WVP);
	
	GLint getUniform(const GLchar* uniformName) const;
	/*
	void setUniform(const GLchar* name, const glm::mat2& m, GLboolean transpose = GL_FALSE);
	void setUniform(const GLchar* name, const glm::mat3& m, GLboolean transpose = GL_FALSE);
	void setUniform(const GLchar* name, const glm::mat4& m, GLboolean transpose = GL_FALSE);
	void setUniform(const GLchar* uniformName, const glm::vec3& v);
	void setUniform(const GLchar* uniformName, const glm::vec4& v);

#define _SHADER_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(OGL_TYPE) \
        void setAttrib(const GLchar* attribName, OGL_TYPE v0); \
        void setAttrib(const GLchar* attribName, OGL_TYPE v0, OGL_TYPE v1); \
        void setAttrib(const GLchar* attribName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2); \
        void setAttrib(const GLchar* attribName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3); \
\
        void setAttrib1v(const GLchar* attribName, const OGL_TYPE* v); \
        void setAttrib2v(const GLchar* attribName, const OGL_TYPE* v); \
        void setAttrib3v(const GLchar* attribName, const OGL_TYPE* v); \
        void setAttrib4v(const GLchar* attribName, const OGL_TYPE* v); \
\
        void setUniform(const GLchar* uniformName, OGL_TYPE v0); \
        void setUniform(const GLchar* uniformName, OGL_TYPE v0, OGL_TYPE v1); \
        void setUniform(const GLchar* uniformName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2); \
        void setUniform(const GLchar* uniformName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3); \
\
        void setUniform1v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count=1); \
        void setUniform2v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count=1); \
        void setUniform3v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count=1); \
        void setUniform4v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count=1); \

	_SHADER_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLfloat)
		_SHADER_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLdouble)
		_SHADER_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLint)
		_SHADER_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLuint)

		GLint getAttrib(const GLchar* attribName) const;
		*/

private:
	ShaderProgram(const ShaderProgram& other);
	ShaderProgram& operator=(ShaderProgram other);

	bool compileShader(GLuint id, const char* path);
};

#endif