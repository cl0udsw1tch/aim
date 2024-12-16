#pragma once
#include "Mesh.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Mesh::Mesh(Object obj)
{
    this->vertices = obj.vertices;
    this->indices = obj.indices;
    normal = glm::vec3(0.0f, 1.0f, 0.0f);
    theta = 0;
    name = obj.name;
    transform = glm::mat4(1.0f);
    setupMesh();
}

Mesh::Mesh(std::vector<Vertex> vertices)
{
    this->vertices = vertices;
    setupMesh();
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //lGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // vertex normals
    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glBindVertexArray(0);
}
void Mesh::Set_Transform(glm::mat4 t) {
    transform = t;
}
void Mesh::Set_Normal(glm::vec3 n) {
    normal = n;
}
void Mesh::Draw()
{
    //glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}
void Mesh::Unbind() {
    glBindVertexArray(0);
}
