#include "Camera.h"
#include <iostream>


Camera::Camera(GLFWwindow* window, float camspeed)
{
    m_window = window;
    m_cameraPos = glm::vec3(0.0f, 0.0f, 20.0f);
    m_cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    m_camPosNorm = glm::distance(m_cameraPos, glm::vec3(0.0f, 0.0f, 0.0f));
    m_view = glm::lookAt(m_cameraPos, m_cameraTarget, m_cameraUp);
    m_CameraSpeed = camspeed;
    m_spin = false;
    m_direction = glm::vec3(0.0f, 0.0f, 1.0f);
    m_yaw = -90.0f;
    m_pitch = 0.0f;
    m_cam_sensitivity = 0.05;
    
}

float theta = 0.0f;
void Camera::SetCamera()
{
    Position();

}


void Camera::Position()
{
    extern float theta;
    if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        m_spin = false;
        m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    }
    else if (glfwGetKey(m_window, GLFW_KEY_TAB) == GLFW_PRESS && !m_spin)
    {
        m_spin = true;
    }
    else if (glfwGetKey(m_window, GLFW_KEY_TAB) == GLFW_PRESS && m_spin)
    {
        m_spin = false;
    }
    if (!m_spin)
    {
        Turn();
        Move();
        m_view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
    }
    else
    {
        float posx = sin(theta) * m_camPosNorm;
        float posz = cos(theta) * m_camPosNorm;
        m_cameraPos = glm::vec3(posx, 0.0f, posz);
        m_view = glm::lookAt(m_cameraPos, m_cameraTarget, m_cameraUp);
        theta += 0.01f;
    }

}
bool firstMouse = true;
double xpos;
double ypos;
double xoffset = 0.0;
double yoffset = 0.0;

void mouse_callback(GLFWwindow* window, double new_xpos, double new_ypos)
{
    if (firstMouse) // initially set to true
    {
        firstMouse = false;
    }
    else
    {
        xoffset = new_xpos - xpos;
        yoffset = ypos - new_ypos;
    }
    xpos = new_xpos;
    ypos = new_ypos;
}

void Camera::Turn()
{

    if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_3) == GLFW_PRESS) {
        
        glfwSetCursorPosCallback(m_window, mouse_callback);

        if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) { xoffset = 0.0; yoffset = 0.0; }
        if (abs(xoffset) < 5) { xoffset = 0; }
        if (abs(yoffset) < 5) { yoffset = 0; }
        m_yaw += (xoffset * m_cam_sensitivity);
        m_pitch += (yoffset * m_cam_sensitivity);


        m_direction = glm::vec3(cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)),
        sin(glm::radians(m_pitch)),
        sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));

        m_cameraFront = m_direction;
    }
}
void Camera::Move()
{
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
        m_cameraPos += m_CameraSpeed * m_cameraFront;
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
        m_cameraPos -= m_CameraSpeed * m_cameraFront;
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
        m_cameraPos -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * m_CameraSpeed;
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
        m_cameraPos += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * m_CameraSpeed;
    m_camPosNorm = glm::distance(m_cameraPos, glm::vec3(0.0f, 0.0f, 0.0f));
    
}

