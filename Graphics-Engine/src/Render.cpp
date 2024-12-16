#include "Render.h"


Render::Render(std::string modelpath, std::string vs, std::string fs, GLFWwindow* window) 
	
	: model(modelpath), camera(window, 0.2), m_shader(vs, fs) {}


void Render::Draw()
{
	camera.SetCamera();

	for (Mesh mesh : model.m_meshes) {
		m_shader.SetUniforms(camera.m_view, model.transform * mesh.transform);
		mesh.Draw();
	}

}


