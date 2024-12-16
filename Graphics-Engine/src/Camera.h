#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glfw3.h"

class Camera
{
public:
	GLFWwindow* m_window;
	glm::vec3 m_cameraPos;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_cameraFront;
	glm::vec3 m_cameraUp;
	float m_camPosNorm;
	glm::mat4 m_view;
	float m_CameraSpeed;
	bool m_spin;
	glm::vec3 m_direction;
	float m_yaw;
	float m_pitch;
	double m_cam_sensitivity;

public:
	Camera(GLFWwindow* window, float camspeed);
	Camera() {}
	void SetCamera();
	void Position();
	void Turn();
	void Move();
};

void mouse_callback(GLFWwindow* window, double new_xpos, double new_ypos);
