#pragma once;
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Transforms {	
public:
	static glm::mat4 rotate(const glm::vec3& point, const glm::mat4& matrix, float angle, glm::vec3 axis);
};