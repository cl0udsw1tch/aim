#pragma once
/* Parses OBJ Wavefront files into vector[s] of vertex data to be loaded into RAM. */

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Vertex
{
	glm::vec3 Position;
	glm::vec2 TexCoord;
	glm::vec3 Normal;
};
struct Object
{
	Object(std::string n) {
		name = n;
	}
	std::string name;
	std::vector<Vertex> vertices;
	std::vector<glm::vec3> indices;
};
class ObjParser
{
public:
	unsigned int m_numObjects;
	std::vector<unsigned int> m_objFaces;
	std::string m_filepath;
	unsigned int m_numPositions;
	unsigned int m_numTexCoords;
	unsigned int m_numNormals;
	unsigned int m_numFaces;
	std::vector<Object> m_Objects;
	std::vector<Vertex> m_Vertices;
	std::vector<glm::vec3> m_Indices;

private:
	void ObjCounter();
	void ObjIndicize();
public:
	ObjParser(std::string& filepath);
	
};