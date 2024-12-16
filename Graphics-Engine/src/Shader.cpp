#include "Shader.h"
#include "glfw3.h"
#include <iostream>


Shader::Shader(std::string vertexshaderpath, std::string fragmentshaderpath)
{
	m_VertexShaderFilePath = vertexshaderpath;
	m_FragmentShaderFilePath = fragmentshaderpath;
	m_Shaders = GetShaders();

	CompileShaders();
	CreateShader();
}

Shader::~Shader()
{
	glDeleteProgram(m_ShaderProgramID);
	glDeleteProgram(m_VertexShaderID);
	glDeleteProgram(m_FragmentShaderID);
}

void Shader::SetUniforms(glm::mat4 view, glm::mat4 transform)
{
	glUseProgram(m_ShaderProgramID);
	// set uniforms

	m_ColorProtocolUL = glGetUniformLocation(m_ShaderProgramID, "BodyColor");
	glUniform4f(m_ColorProtocolUL, 1.0f, 1.0, 1.0, 1.0f);

	// get data from Transform class to put into transform matrix (model transform)
	m_TransformMatUL = glGetUniformLocation(m_ShaderProgramID, "BodyTransform");
	glUniformMatrix4fv(m_TransformMatUL, 1, GL_FALSE, glm::value_ptr(transform));

	// get data from Camera class to put into view matrix
	int viewUL = glGetUniformLocation(m_ShaderProgramID, "BodyView");
	glUniformMatrix4fv(viewUL, 1, GL_FALSE, glm::value_ptr(view));


	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 1500.0f);
	int projectionUL = glGetUniformLocation(m_ShaderProgramID, "BodyProjection");
	glUniformMatrix4fv(projectionUL, 1, GL_FALSE, glm::value_ptr(projection));

	//glUseProgram(0);

}
void Shader::Activate() {
	glUseProgram(m_ShaderProgramID);
}
void Shader::Deactivate() 
{
	glUseProgram(0);
}

// Really a helper function for the proceeding one. 

Shaders Shader::GetShaders() 
{
	std::ifstream VertexFile(m_VertexShaderFilePath);
	std::ifstream FragmentFile(m_FragmentShaderFilePath);
	
	std::stringstream vs_temp, fs_temp;

	if (VertexFile.is_open())
	{
		 vs_temp << VertexFile.rdbuf();
	}

	if (FragmentFile.is_open())
	{
		fs_temp << FragmentFile.rdbuf();
	}
	VertexFile.close();
	FragmentFile.close();

	return { vs_temp.str(), fs_temp.str() };

}

void Shader::CompileShaders()
{
	const char* VertexShader = m_Shaders.VertexShader.c_str();
	const char* FragmentShader = m_Shaders.FragmentShader.c_str();
	m_VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_VertexShaderID, 1, &VertexShader, NULL);
	glCompileShader(m_VertexShaderID);
	
	// Compilation error handling
	int v_success;
	char v_infoLog[512];

	glGetShaderiv(m_VertexShaderID, GL_COMPILE_STATUS, &v_success);
	if (!v_success)
	{
		glGetShaderInfoLog(m_VertexShaderID, 512, NULL, v_infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << v_infoLog << std::endl;
	}
	else { std::cout << "Vertex shader compiled successfully" << std::endl; }
	// ================================

	m_FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_FragmentShaderID, 1, &FragmentShader, NULL);
	glCompileShader(m_FragmentShaderID);

	// Compilation error handling
	int s_success;
	char s_infoLog[512];

	glGetShaderiv(m_FragmentShaderID, GL_COMPILE_STATUS, &s_success);
	if (!s_success)
	{
		glGetShaderInfoLog(m_FragmentShaderID, 512, NULL, s_infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << s_infoLog << std::endl;
	}
	else { std::cout << "Fragment shader compiled successfully" << std::endl; }
	// ================================
}

void Shader::CreateShader()
{
	m_ShaderProgramID = glCreateProgram();

	glAttachShader(m_ShaderProgramID, m_VertexShaderID);
	glAttachShader(m_ShaderProgramID, m_FragmentShaderID);

	glLinkProgram(m_ShaderProgramID);
}

