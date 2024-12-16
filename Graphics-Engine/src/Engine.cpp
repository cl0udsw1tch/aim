#include "Engine.h"

constexpr auto pi = 3.14159265;
glm::mat4 Identity = glm::mat4(1.0f);

void doTCP(Client& client) {
	client.TCP();
}

Engine::Engine(std::string modelpath, std::string vs, std::string fs, GLFWwindow* w)
	: render(modelpath, vs, fs, w), client()
{
	window = w;
	Timer time = {"default", 0.0f};
	protocol = protocols::OFFLINE; 
	bootstage = bootstages::NA;
	movecmd = movecommands::NA;
	landstage = landstages::NA;

	ModelID = &render.model;

	for (Mesh& mesh : render.model.m_meshes) {
		if (mesh.name == "Body") {
			BodyID = &mesh;
		}
		else if (mesh.name == "Engine1") {
			Engine1ID = &mesh;
		}
		else if (mesh.name == "Engine2") {
			Engine2ID = &mesh;
		}
		else if (mesh.name == "Engine3") {
			Engine3ID = &mesh;
		}
	}


	std::thread client_thread(doTCP, std::ref(client));
	client_thread.detach();

	
}
void Engine::GetCommand()
{
	// logging commands from ship
	// TODO: build an actual logger. 
	if (client.command != "") { 
		std::cout << "COMMAND: " << client.command << std::endl; 
		commandID = client.command;
		client.command = ""; 
	}

	switch (protocol) {
	case protocols::OFFLINE :
		if (commandID == "PRAN_MUDRA" || glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		{
			// begin BOOT protocol
			protocol = protocols::BOOT;
			if (time.name != "BOOT") {
				time.name = "BOOT";
				time.t = 0;
			}
		}
		break;
	case protocols::ONLINE:
		if (commandID == "FIST" || glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		{
			// begin LAND protocol
			protocol = protocols::SHUTDOWN;
			if (time.name != "SHUTDOWN") {
				time.name = "SHUTDOWN";
				time.t = 0;
			}
		}

		// move commands 
		else if (commandID == "POINT_LEFT" || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			movecmd = movecommands::LEFT;
			if (time.name != "LEFT") {
				time.name = "LEFT";
				time.t = 0;
			}
		}
		else if (commandID == "POINT_RIGHT" || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			movecmd = movecommands::RIGHT;
			if (time.name != "RIGHT") {
				time.name = "RIGHT";
				time.t = 0;
			}
		}
		else if (commandID == "ARM_CROSS" || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			movecmd = movecommands::FORWARD;
			if (time.name != "FORWARD") {
				time.name = "FORWARD";
				time.t = 0;
			}
		}
		else if (commandID == "BACK_HAND" || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			movecmd = movecommands::BACKWARD;
			if (time.name != "BACKWARD") {
				time.name = "BACKWARD";
				time.t = 0;
			}
		}
		else if (commandID == "FRONT_HAND" || glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
			movecmd = movecommands::STOP;
			if (time.name != "STOP") {
				time.name = "STOP";
				time.t = 0;
			}
		}
		break;
	}

}

void Engine::ProcessCommand() {

}

void Engine::Move()
{
	switch (protocol) 
	{
		case protocols::OFFLINE:
			break;
		case protocols::BOOT:

			if (bootstage == bootstages::NA) {
				bootstage = bootstages::LIFTOFF;
				if (time.name != "LIFTOFF") {
					time.name = "LIFTOFF";
					time.t = 0;
				}
			}

			switch (bootstage) {

				case bootstages::LIFTOFF :
					// model liftoff
					if (ModelID->position.y < 3.0f) {

						ModelID->position.y += 0.01f;
						glm::mat4 TranslateMat = glm::translate(ModelID->transform, glm::vec3(0.0f, 0.01f, 0.0f));
						ModelID->Set_Transform(TranslateMat);
					}
					else {
						time.name = "ENGINE2_CONFIG";
						time.t = 0;
						bootstage = bootstages::ENGINE2_CONFIG;
					}
					break;

				case bootstages::ENGINE2_CONFIG :
					// configure the engines
					if (glm::distance(Engine2ID->normal, glm::vec3(0.0f, 0.0f, 1.0f)) > 0.01) {

						glm::mat4 SpinMat2 = glm::rotate(Engine2ID->transform, 0.005f, glm::vec3(1.0f, 0.0f, 0.0f));
						glm::vec3 vec = glm::vec3(glm::rotate(Identity, 0.005f, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(Engine2ID->normal, 1.0f));

						Engine2ID->Set_Normal(vec);
						Engine2ID->Set_Transform(SpinMat2);

						// Engine 3 is locked to Engine 2 during booting.
						Engine3ID->Set_Normal(Engine2ID->normal);
						Engine3ID->Set_Transform(Engine2ID->transform);

					}
					else {
						time.name = "ENGINE3_CONFIG";
						time.t = 0;
						bootstage = bootstages::ENGINE3_CONFIG;
					}
					break;
				case bootstages::ENGINE3_CONFIG:
					if (glm::distance(Engine3ID->normal, glm::vec3(1.0f, 0.0f, 0.0f)) > 0.01) {
	
						glm::mat4 SpinMat3 = glm::rotate(Identity, 0.01f, glm::vec3(0.0f, 1.0f, 0.0f));
						glm::mat4 mat = SpinMat3 * Engine3ID->transform;
						glm::vec3 vec = glm::vec3(glm::rotate(Identity, 0.01f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(Engine3ID->normal, 1.0f));

						Engine3ID->Set_Normal(vec);
						Engine3ID->Set_Transform(mat);
					}
					else {
						time.name = "ONLINE";
						time.t = 0;
						bootstage = bootstages::NA;
						protocol = protocols::ONLINE;
					}
					break;
			}
			break;

		case protocols::ONLINE:

			// move mechanics
			switch (movecmd) {
				case movecommands::LEFT : 
					glm::mat4 TranslateMatL = glm::translate(ModelID->transform, glm::vec3(-0.03, 0.0f, 0.0f));
					ModelID->Set_Transform(TranslateMatL);
					break;
				case movecommands::RIGHT:
					glm::mat4 TranslateMatR = glm::translate(ModelID->transform, glm::vec3(0.03, 0.0f, 0.0f));
					ModelID->Set_Transform(TranslateMatR);
					break;
				case movecommands::FORWARD:
					glm::mat4 TranslateMatF = glm::translate(ModelID->transform, glm::vec3(0.0f, 0.0f, 0.03));
					ModelID->Set_Transform(TranslateMatF);
					break;
				case movecommands::BACKWARD:
					glm::mat4 TranslateMatB = glm::translate(ModelID->transform, glm::vec3(0.0f, 0.0f, -0.03));
					ModelID->Set_Transform(TranslateMatB);
					break;
				case movecommands::STOP :


					break;
			}

			break;

		case protocols::SHUTDOWN :

			if (landstage == landstages::NA) {
				landstage = landstages::ENGINE1_CONFIG;
				if (time.name != "ENGINE1_CONFIG") {
					time.name = "ENGINE1_CONFIG";
					time.t = 0;
				}
			}

			switch (landstage) {
				case landstages::ENGINE1_CONFIG :
					if (glm::distance(Engine1ID->normal, glm::vec3(0.0f, 1.0f, 0.0f)) > 0.01) {
						int sgn = 1;
						if (Engine1ID->normal.z > 0) { sgn = -1; }
						if (Engine1ID->normal.z < 0) { sgn = 1; }
						glm::mat4 SpinMat1 = glm::rotate(Identity, sgn * 0.01f, glm::vec3(1.0f, 0.0f, 0.0f));
						glm::mat4 mat = SpinMat1 * Engine1ID->transform;
						glm::vec3 vec = glm::vec3(glm::rotate(Identity, sgn * 0.01f, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(Engine1ID->normal, 1.0f));

						Engine1ID->Set_Normal(vec);
						Engine1ID->Set_Transform(mat);
					}
					else {
						landstage = landstages::ENGINE3_CONFIG;
						if (time.name != "ENGINE3_CONFIG") {
							time.name = "ENGINE3_CONFIG";
							time.t = 0;
						}
					}
					break;
				case landstages::ENGINE3_CONFIG :
					if (glm::distance(Engine3ID->normal, glm::vec3(0.0f, 0.0f, 1.0f)) > 0.01) {

						int sgn = 1;
						if (Engine3ID->normal.x > 0) { sgn = -1; }
						if (Engine3ID->normal.x < 0) { sgn = 1; }
						glm::mat4 SpinMat3 = glm::rotate(Identity, sgn * 0.01f, glm::vec3(0.0f, 1.0f, 0.0f));
						glm::mat4 mat = SpinMat3 * Engine3ID->transform;
						glm::vec3 vec = glm::vec3(glm::rotate(Identity, sgn * 0.01f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(Engine3ID->normal, 1.0f));

						Engine3ID->Set_Normal(vec);
						Engine3ID->Set_Transform(mat);
					}
					else {
						landstage = landstages::ENGINE2_CONFIG;
						if (time.name != "ENGINE2_CONFIG") {
							time.name = "ENGINE2_CONFIG";
							time.t = 0;
						}
					}
					break;
				case landstages::ENGINE2_CONFIG:
					if (glm::distance(Engine2ID->normal, glm::vec3(0.0f, 1.0f, 0.0f)) > 0.01) {
						int sgn = 1;
						if (Engine2ID->normal.z > 0) { sgn = -1; }
						if (Engine2ID->normal.z < -0) { sgn = 1; }
						glm::mat4 SpinMat2 = glm::rotate(Identity, sgn * 0.01f, glm::vec3(1.0f, 0.0f, 0.0f));
						glm::mat4 mat = SpinMat2 * Engine2ID->transform;
						glm::vec3 vec = glm::vec3(glm::rotate(Identity, sgn * 0.01f, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(Engine2ID->normal, 1.0f));

						Engine2ID->Set_Normal(vec);
						Engine2ID->Set_Transform(mat);

						// engine 3 is locked to engine 2.
						Engine3ID->Set_Normal(Engine2ID->normal);
						Engine3ID->Set_Transform(Engine2ID->transform);

					}
					else {
						landstage = landstages::LAND;
						if (time.name != "LAND") {
							time.name = "LAND";
							time.t = 0;
						}
					}
					break;
				case landstages::LAND :
					if (ModelID->position.y > 0.01f) {

						ModelID->position.y += -0.01f;
						glm::mat4 TranslateMat = glm::translate(ModelID->transform, glm::vec3(0.0f, -0.01f, 0.0f));
						ModelID->Set_Transform(TranslateMat);
					}
					else {
						time.name = "OFFLINE";
						time.t = 0;
						protocol = protocols::OFFLINE;
						bootstage = bootstages::NA;
						landstage = landstages::NA;
						movecmd = movecommands::NA;
					}
					break;
			}

			break;

	}
}

 void Engine::Simulate() {
	 render.Draw();
 }


