#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "string"
#include "Render.h"
#include "Client.h"
#include <thread>

enum class protocols { OFFLINE, BOOT, ONLINE, SHUTDOWN };
enum class bootstages { NA = -1, LIFTOFF, ENGINE2_CONFIG, ENGINE3_CONFIG };
enum class movecommands { NA = -1, LEFT, RIGHT, FORWARD, BACKWARD, STOP};
enum class landstages {NA = -1, ENGINE1_CONFIG, ENGINE2_CONFIG, ENGINE3_CONFIG, LAND};

struct Timer {
	std::string name;
	float t;
};


class Engine {
private:
	float theta = 0.1;
	Timer time;
	GLFWwindow* window;

public:
	protocols protocol;
	bootstages bootstage;
	movecommands movecmd;
	landstages landstage;

	std::string commandID;
	Client client;
	

	Render render;
	//Client client;

	Model* ModelID;
	Mesh* BodyID;
	Mesh* Engine1ID;
	Mesh* Engine2ID;
	Mesh* Engine3ID;

	Engine(std::string modelpath, std::string vs, std::string fs, GLFWwindow* window);
	
	void GetCommand();
	void ProcessCommand();
	void Move();
	void Simulate();
	//void ClientProtocol();

	
};