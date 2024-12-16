#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Shader.h"
#include "ObjParser.h"
class Mesh
{
private:
    unsigned int VAO, VBO, EBO;
    void setupMesh();

public:
    std::string name;
    std::vector<Vertex> vertices;
    std::vector<glm::vec3> indices;
    glm::mat4 transform;
    glm::vec3 normal;
    float theta;

    Mesh(Object obj);
    Mesh(std::vector<Vertex> vertices);
    void Set_Transform(glm::mat4 t);
    void Set_Normal(glm::vec3 n);
    void Draw();
    void Unbind();
};