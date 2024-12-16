#include "Transforms.h"

glm::mat4 Transforms::rotate(const glm::vec3 &point, const glm::mat4& matrix, float angle, glm::vec3 axis)
{
	glm::mat4 t = glm::translate(matrix, point);
	glm::mat4 t1 = glm::rotate(t, angle, axis);
	glm::mat4 t2 = glm::translate(t1, -point);
	return t2;
}
