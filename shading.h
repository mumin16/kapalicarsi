#pragma once
#define SHADER_SOURCE(...) #__VA_ARGS__
#include <string>
#include <vector>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class shading
{
public:
	shading() {};
	~shading() {};
	GLuint mProgram;
	void demoshader();
	void loadmvp(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
	void loadcolor(glm::vec4 color);
private:
	GLuint CompileShader(GLenum type, const std::string &source);
	GLuint CompileProgram(const std::string &vsSource, const std::string &fsSource);

};

void shading::loadcolor(glm::vec4 color) {

	GLuint colorloc = glGetUniformLocation(mProgram, "vColor");

	glUniform4fv(colorloc, 1,  glm::value_ptr(color));

}
void shading::loadmvp(glm::mat4 model, glm::mat4 view, glm::mat4 projection) {

	GLuint modelloc = glGetUniformLocation(mProgram, "model");

	glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(model));

	GLuint viewloc = glGetUniformLocation(mProgram, "view");

	glUniformMatrix4fv(viewloc, 1, GL_FALSE, glm::value_ptr(view));

	GLuint projloc = glGetUniformLocation(mProgram, "projection");

	glUniformMatrix4fv(projloc, 1, GL_FALSE, glm::value_ptr(projection));
}
void shading::demoshader() {

	const std::string vs = SHADER_SOURCE
	(
		attribute vec4 vPosition;
	uniform vec4 vColor;
	varying vec4 fColor;
	uniform mat4 view;
	uniform mat4 projection;
	uniform mat4 model;
	//uniform int charttype;
	void main()
	{

		//MVP = mat4(0.0f);
		//gl_PointSize = 3.0f;
		gl_Position = projection * view * model * vPosition;
		fColor = vColor;
	}
	);

	const std::string fs = SHADER_SOURCE
	(
		//precision mediump float;
		varying vec4 fColor;
	void main()
	{
		gl_FragColor = fColor;//vec4(0.0, 1.0, 0.0, 1.0);
	}
	);

	std::vector<std::string> emptyVector;
	mProgram = CompileProgram(vs, fs);
	if (!mProgram)
	{
		return;
	}
}
GLuint shading::CompileShader(GLenum type, const std::string &source)
{
	GLuint shader = glCreateShader(type);
	const char *sourceArray[1] = { source.c_str() };
	glShaderSource(shader, 1, sourceArray, NULL);
	glCompileShader(shader);
	return shader;
}
GLuint shading::CompileProgram(
	const std::string &vsSource,
	const std::string &fsSource)
{
	GLuint program = glCreateProgram();

	GLuint vs = CompileShader(GL_VERTEX_SHADER, vsSource);
	GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fsSource);


	glAttachShader(program, vs);
	glDeleteShader(vs);

	glAttachShader(program, fs);
	glDeleteShader(fs);



	glLinkProgram(program);



	return program;
}