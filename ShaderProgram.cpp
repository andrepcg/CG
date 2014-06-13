#include <string>
#include <fstream>
#include <vector>


#include "ShaderProgram.h"


ShaderProgram::ShaderProgram(const char* vertexFilePath, const char* fragmentFilePath) : programID(0) {
	GLint Result = GL_FALSE;
	int InfoLogLength;

	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragmentFilePath, std::ios::in);

	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertexFilePath, std::ios::in);

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	if (VertexShaderStream.is_open()){
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertexFilePath);
	}

	if (FragmentShaderStream.is_open()){
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}



	if (!compileShader(VertexShaderID, VertexShaderCode.c_str())) return;
	if (!compileShader(FragmentShaderID, FragmentShaderCode.c_str())) return;



	// Link the program
	printf("Linking program\n");
	this->programID = glCreateProgram();
	glAttachShader(this->programID, VertexShaderID);
	glAttachShader(this->programID, FragmentShaderID);
	glLinkProgram(this->programID);

	// Check the program
	glGetProgramiv(this->programID, GL_LINK_STATUS, &Result);
	glGetProgramiv(this->programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(this->programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

}

ShaderProgram::~ShaderProgram() {
	if (programID != 0)
		glDeleteProgram(programID);
}

void ShaderProgram::use() {
	glUseProgram(programID);
}

void ShaderProgram::stopUsing() {
	glUseProgram(0);
}

bool ShaderProgram::compileShader(GLuint id, const char* code) {
	GLint Result = GL_FALSE;
	int InfoLogLength;

	printf("Compiling shader %u\n", id);
	glShaderSource(id, 1, &code, NULL);
	glCompileShader(id);

	// Check result
	glGetShaderiv(id, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<char> msg(InfoLogLength + 1);
		glGetShaderInfoLog(id, InfoLogLength, NULL, &msg[0]);
		printf("%s\n", &msg[0]);
		return false;
	}

	return true;
}


/*

#define ATTRIB_N_UNIFORM_SETTERS(OGL_TYPE, TYPE_PREFIX, TYPE_SUFFIX) \
\
    void ShaderProgram::setAttrib(const GLchar* name, OGL_TYPE v0) \
        {  glVertexAttrib ## TYPE_PREFIX ## 1 ## TYPE_SUFFIX (getAttrib(name), v0); } \
    void ShaderProgram::setAttrib(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1) \
        {  glVertexAttrib ## TYPE_PREFIX ## 2 ## TYPE_SUFFIX (getAttrib(name), v0, v1); } \
    void ShaderProgram::setAttrib(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2) \
        {  glVertexAttrib ## TYPE_PREFIX ## 3 ## TYPE_SUFFIX (getAttrib(name), v0, v1, v2); } \
    void ShaderProgram::setAttrib(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3) \
        {  glVertexAttrib ## TYPE_PREFIX ## 4 ## TYPE_SUFFIX (getAttrib(name), v0, v1, v2, v3); } \
\
    void ShaderProgram::setAttrib1v(const GLchar* name, const OGL_TYPE* v) \
        {  glVertexAttrib ## TYPE_PREFIX ## 1 ## TYPE_SUFFIX ## v (getAttrib(name), v); } \
    void ShaderProgram::setAttrib2v(const GLchar* name, const OGL_TYPE* v) \
        {  glVertexAttrib ## TYPE_PREFIX ## 2 ## TYPE_SUFFIX ## v (getAttrib(name), v); } \
    void ShaderProgram::setAttrib3v(const GLchar* name, const OGL_TYPE* v) \
        {  glVertexAttrib ## TYPE_PREFIX ## 3 ## TYPE_SUFFIX ## v (getAttrib(name), v); } \
    void ShaderProgram::setAttrib4v(const GLchar* name, const OGL_TYPE* v) \
        {  glVertexAttrib ## TYPE_PREFIX ## 4 ## TYPE_SUFFIX ## v (getAttrib(name), v); } \
\
    void ShaderProgram::setUniform(const GLchar* name, OGL_TYPE v0) \
        {  glUniform1 ## TYPE_SUFFIX (getUniform(name), v0); } \
    void ShaderProgram::setUniform(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1) \
        {  glUniform2 ## TYPE_SUFFIX (getUniform(name), v0, v1); } \
    void ShaderProgram::setUniform(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2) \
        {  glUniform3 ## TYPE_SUFFIX (getUniform(name), v0, v1, v2); } \
    void ShaderProgram::setUniform(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3) \
        {  glUniform4 ## TYPE_SUFFIX (getUniform(name), v0, v1, v2, v3); } \
\
    void ShaderProgram::setUniform1v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
        {  glUniform1 ## TYPE_SUFFIX ## v (getUniform(name), count, v); } \
    void ShaderProgram::setUniform2v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
        {  glUniform2 ## TYPE_SUFFIX ## v (getUniform(name), count, v); } \
    void ShaderProgram::setUniform3v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
        {  glUniform3 ## TYPE_SUFFIX ## v (getUniform(name), count, v); } \
    void ShaderProgram::setUniform4v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
        {  glUniform4 ## TYPE_SUFFIX ## v (getUniform(name), count, v); }

ATTRIB_N_UNIFORM_SETTERS(GLfloat, , f);
ATTRIB_N_UNIFORM_SETTERS(GLdouble, , d);
ATTRIB_N_UNIFORM_SETTERS(GLint, I, i);
ATTRIB_N_UNIFORM_SETTERS(GLuint, I, ui);

void ShaderProgram::setUniform(const GLchar* name, const glm::mat2& m, GLboolean transpose) {
	glUniformMatrix2fv(getUniform(name), 1, transpose, glm::value_ptr(m));
}
void ShaderProgram::setUniform(const GLchar* name, const glm::mat3& m, GLboolean transpose) {
	glUniformMatrix3fv(getUniform(name), 1, transpose, glm::value_ptr(m));
}
void ShaderProgram::setUniform(const GLchar* name, const glm::mat4& m, GLboolean transpose) {
	glUniformMatrix4fv(getUniform(name), 1, transpose, glm::value_ptr(m));
}
void ShaderProgram::setUniform(const GLchar* name, const glm::vec3& v) {
	setUniform3v(name, glm::value_ptr(v));
}
void ShaderProgram::setUniform(const GLchar* name, const glm::vec4& v) {
	setUniform4v(name, glm::value_ptr(v));

}


*/
GLint ShaderProgram::getUniform(const GLchar* uniformName) const {

	return glGetUniformLocation(this->programID, uniformName);
}
/*
GLint ShaderProgram::getAttrib(const GLchar* attribName) const {

	return glGetAttribLocation(this->programID, attribName);;
}
*/

void ShaderProgram::SetWVP(const GLchar* name, const Matrix4f& WVP)
{
	glUniformMatrix4fv(getUniform(name), 1, GL_TRUE, (const GLfloat*)WVP.m);
}
