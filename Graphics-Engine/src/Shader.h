#pragma once
#include "glad/glad.h"
#include "glfw3.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Shaders
{
	std::string VertexShader;
	std::string FragmentShader;
};

class Shader
{
private:
	std::string m_VertexShaderFilePath, m_FragmentShaderFilePath;
	unsigned int m_VertexShaderID, m_FragmentShaderID, m_ShaderProgramID;
	Shaders m_Shaders;
	unsigned int m_ColorProtocolUL, m_TransformMatUL;
public:
	Shader(std::string vertexshaderpath, std::string fragmentshaderpath);
	Shader() {}
	~Shader();

	void SetUniforms(glm::mat4 view, glm::mat4 transform);
	void Activate();

	void Deactivate();

	Shaders GetShaders();

	void CompileShaders();

	void CreateShader();

};