#include "ObjParser.h"
#include <filesystem>

void ObjParser::ObjCounter()
{
	std::ifstream file(m_filepath);
	std::string id;
	std::string line;

	file >> id;

	while (id != "o" && std::getline(file, line)) { file >> id; }

	m_objFaces;
	while (id == "o")
	{
		m_numObjects++;
		unsigned int num_faces = 0;
	
		while (id != "v" && std::getline(file, line)) { file >> id; }
	
		// c is now v, and file is at the first vertex
		while (id == "v" && std::getline(file, line)) { file >> id; m_numPositions++; }

		while (id == "vt" && std::getline(file, line)) { file >> id; m_numTexCoords++; }

		while (id == "vn" && std::getline(file, line)) { file >> id; m_numNormals++; }

		while (id != "f" && std::getline(file, line)) { file >> id; }
		while (id == "f" && std::getline(file, line)) { file >> id; num_faces++; m_numFaces++; }
		m_objFaces.push_back(num_faces);
	}



	file.close();
}

ObjParser::ObjParser(std::string& filepath)
{
	m_filepath = filepath;
	m_numObjects = 0;
	m_numPositions = 0;
	m_numTexCoords = 0;
	m_numNormals = 0;
	m_numFaces = 0;
	ObjCounter();

	m_Objects.reserve(m_numObjects);

	std::vector<glm::vec3> Positions;
	Positions.reserve(m_numPositions);

	std::vector<glm::vec2> TexCoords;
	TexCoords.reserve(m_numTexCoords);

	std::vector<glm::vec3> Normals;
	Normals.reserve(m_numNormals);

	m_Vertices.reserve(m_numFaces * 3);

	std::ifstream file(filepath);
	std::string id;
	std::string line;
	
	file >> id;

	while (id != "o" && std::getline(file, line)) { file >> id; }
	
	
	for (unsigned int obj_id = 0; obj_id < m_numObjects; obj_id++)
	{
		std::string obj_name;
		file >> obj_name;
		int delim = obj_name.find("_");
		obj_name = obj_name.substr(0, delim);
		Object obj(obj_name);
		m_Objects.push_back(obj);

		while (id != "v") { std::getline(file, line);  file >> id; }

		std::vector<Vertex> vertices;
		m_Objects[obj_id].vertices.reserve(m_objFaces[obj_id] * 3);

		while (id == "v") 
		{
				
			std::string x, y, z;
			file >> x >> y >> z;
			Positions.emplace_back(glm::vec3(std::stof(x), std::stof(y), std::stof(z)));
			std::getline(file, line);
			file >> id;

		}
		while (id == "vt") 
		{
			std::string x, y;
			file  >> x >> y;
			TexCoords.emplace_back(glm::vec2(std::stof(x), std::stof(y)));
			std::getline(file, line);
			file >> id;
				
		}
		while (id == "vn") 
		{

			std::string x, y, z;
			file >> x >> y >> z;
			Normals.emplace_back(glm::vec3(std::stof(x), std::stof(y), std::stof(z)));
			std::getline(file, line);
			file >> id;

		}

		while (id != "f" && std::getline(file, line)) { file >> id; }
		while (id == "f")
		{
			std::string v1, v2, v3;
			file >> v1 >> v2 >> v3;

			for (std::string v : { v1, v2, v3 })
			{
				unsigned short next = v.find("/");
				unsigned int vp_index = std::stoi(v.substr(0, next)); 
				unsigned short last = next + 1;

				next = v.find("/", last);
				unsigned int vt_index = std::stoi(v.substr(last, next - last));
				last = next + 1;

				unsigned int vn_index = std::stoi(v.substr(last, v.length()));
					
				m_Objects[obj_id].vertices.emplace_back(Positions[vp_index - 1], TexCoords[vt_index - 1], Normals[vn_index - 1]);
			}
			std::getline(file, line);

			if (file.peek() != EOF)
			{
				file >> id;
			}
			else {
				break;
			}
			
		}

	}
	file.close();
}

void ObjParser::ObjIndicize()
{
}


