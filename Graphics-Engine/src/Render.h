#pragma once
#include <string>
#include "Model.h"
#include "Camera.h"


class Render {
private:

public:

	Model model;
	Camera camera;
	Shader m_shader;
	//Transform transformer;
	//Client client;

	Render(std::string modelpath, std::string vs, std::string fs, GLFWwindow* window);
	void Draw();
};