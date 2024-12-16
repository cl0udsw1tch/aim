#include "Model.h"


Model::Model(std::string path)
{
	int start = path.find_last_of("\\");
	int last = path.find(".obj");
	name = path.substr(start + 1, last - start - 1);
	ObjParser model(path);
	
	m_meshes.reserve(model.m_numObjects);

	for (Object object : model.m_Objects) {
		m_meshes.push_back(object);

	}

	transform = glm::mat4(1.0f);
	position = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Model::Set_Transform(glm::mat4 t) {
	transform = t;
}


Model::~Model()
{}