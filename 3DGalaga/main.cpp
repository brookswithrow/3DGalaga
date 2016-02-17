#define GLFW_INCLUDE_GLU
#include <assimp/Importer.hpp> 
#include <assimp/scene.h> 
#include <assimp/postprocess.h>
#include "game.h"
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <stdlib.h>
#include "shader.h"
#include "model.h"

// Lots of variables used for game logic

#define SHOOTERS 5 
#define CHASERS 5
#define SPEED 0.25
#define MAX_MISSILES 20
#define MISSILE_SPEED 2
#define MISSILE_WIDTH 1
#define MISSILE_HEIGHT 4

Shooter shooters[SHOOTERS];
Chaser chasers[CHASERS];
Missile missiles[(SHOOTERS + CHASERS + 1) * 2];
Player player;
int gameState = 2;
int missileCooldown = 0;
int frameCounter = 0;
int numMissiles = 0;
int numChasers = CHASERS;
int numShooters = SHOOTERS;


glm::mat4 proj;
glm::mat4 view;
glm::mat4 model;
glm::mat4 mvp;

GLFWwindow* window;

int screenWidth = 800;
int screenHeight = 600;

using namespace std;

//For user interaction, currently arrow keys = move, x = shoot
void onKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_BACKSPACE) {
		gameState = 0;
	}
	if (key == GLFW_KEY_UP) {
		player.y += SPEED;
	}
	else if (key == GLFW_KEY_DOWN) {
		player.y -= SPEED;
	}
	else if (key == GLFW_KEY_LEFT) {
		player.x += SPEED;
	}
	else if (key == GLFW_KEY_RIGHT) {
		player.x -= SPEED;
	}

	if (key == GLFW_KEY_X && missileCooldown > 50) {
		firePlayerMissile(missiles, MAX_MISSILES, player, &missileCooldown);
		printf("cooldown: %d", missileCooldown);
		numMissiles++;
	}
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Galaga", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, onKeyPress);
	glewExperimental = GL_TRUE;
	glewInit();
	glViewport(0, 0, screenWidth, screenHeight);
	glEnable(GL_DEPTH_TEST);

	Shader shader("C:/Users/Brooks/Documents/GitHub/3DGalaga/3DGalaga/simple.vert", "C:/Users/Brooks/Documents/GitHub/3DGalaga/3DGalaga/simple.frag");

	Model playerModel("C:/Users/Brooks/Documents/GitHub/3DGalaga/3DGalaga/model/player/ship.obj");

	Model shooterModel("C:/Users/Brooks/Documents/GitHub/3DGalaga/3DGalaga/model/shooter/shooter.obj");

	Model chaserModel("C:/Users/Brooks/Documents/GitHub/3DGalaga/3DGalaga/model/chaser/chaser.obj");

	Model pmissile("C:/Users/Brooks/Documents/GitHub/3DGalaga/3DGalaga/model/pmissile/pmissile.obj");

	Model emissile("C:/Users/Brooks/Documents/GitHub/3DGalaga/3DGalaga/model/emissile/emissile0.obj");

	// Draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	makeShooters(shooters, 5);
	makeChasers(chasers, 5);
	player.lives = 3;

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		if (gameState == 2) {
			gameLoop(&missileCooldown, &frameCounter, &player, chasers, shooters, missiles, CHASERS, SHOOTERS, MAX_MISSILES, &numChasers, &numShooters, &numMissiles, &gameState);
		}

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Use();

		// Setting up projection & view matrices since they don't change
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 640.0f / 480.0f, 0.1f, 100.0f);;
		glm::mat4 view = glm::lookAt(glm::vec3(0, 0, -30), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));;
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		// Drawing the player
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(player.x, player.y, 0.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1, 0, 0));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		playerModel.Draw(shader);

		//Drawing shooters
		for (int i = 0; i < SHOOTERS; i++) {
			glm::mat4 model;
			if (shooters[i].exists) {
				model = glm::translate(model, glm::vec3(shooters[i].x, shooters[i].y, 0.0f));
				model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1, 0, 0));
				model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
				model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
				glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
				shooterModel.Draw(shader);
			}
		}

		//Drawing chasers
		for (int i = 0; i < CHASERS; i++) {
			glm::mat4 model;
			if (chasers[i].exists) {
				model = glm::translate(model, glm::vec3(chasers[i].x, chasers[i].y, 0.0f));
				model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1, 0, 0));
				model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
				model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
				glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
				chaserModel.Draw(shader);
			}
		}

		//Drawing missiles, color & orientation depends on if they were fired by
		//an enemy or the player
		for (int i = 0; i < MAX_MISSILES; i++) {
			if (missiles[i].exists) {
				glm::mat4 model;
				model = glm::translate(model, glm::vec3(missiles[i].x, missiles[i].y, 0.0f));
				if (missiles[i].color == 1) {
					model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1, 0, 0));
					model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
					glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
					pmissile.Draw(shader);
				}
				else {
					model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
					model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
					glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
					emissile.Draw(shader);
				}
				
			}
		}

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
