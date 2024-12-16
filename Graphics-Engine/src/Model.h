#pragma once
#include "Mesh.h"
#include "Shader.h"
#include <string>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Model {
private:



public:
	std::vector<Mesh> m_meshes;
	std::string name;
	glm::mat4 transform;
	glm::vec3 position;

	Model(std::string path);
	Model() {}

	void Set_Transform(glm::mat4 t);
	~Model();

};
