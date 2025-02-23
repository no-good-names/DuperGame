//
// Created by no-good-names on 11/2/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unistd.h>
#include <cglm/cglm.h>
#include <limits.h>

#include "gfx/shader.h"
#include "gfx/textures.h"
#include "gfx/camera.h"
#include "gfx/gui.h"

// consts
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// global variables
Camera camera;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool mouseEnabled = true;

// adds constant delay between key presses
bool keywait(float seconds) {
    static float lastTime = 0;
    float currentTime = glfwGetTime();
    if(currentTime - lastTime > seconds) {
        lastTime = currentTime;
        return true;
    }
    return false;
}

void processInput(GLFWwindow *window) {
	if(glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		processKeyboard(&camera, FORWARD, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		processKeyboard(&camera, BACKWARD, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		processKeyboard(&camera, LEFT, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		processKeyboard(&camera, RIGHT, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && keywait(0.5f) == true) {
        mouseEnabled = !mouseEnabled;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = xposIn;
	float ypos = yposIn;

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

    if(mouseEnabled) {
        processMouseMovement(&camera, xoffset, yoffset, false);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	processMouseScroll(&camera, yoffset);
}

void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

char *getPath(const char *filename) {
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		printf("Current working dir: %s\n", cwd);
	} else {
		perror("getcwd() error");
	}
	char *path = (char *)malloc(strlen(cwd) + strlen(filename) + 1);
	strcpy(path, cwd);
	strcat(path, "/");
	strcat(path, filename);
	return path;
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	const char *glsl_version = "#version 150";
	#else
	const char *glsl_version = "#version 130";
	#endif


	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetErrorCallback(glfw_error_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize GLAD\n");
		return -1;
	}
	fprintf(stderr, "OpenGL Version: %s\n", (char *) glGetString(GL_VERSION)); // Only for debugging
	fprintf(stderr, "GLSL Version: %s\n", (char *) glGetString(GL_SHADING_LANGUAGE_VERSION)); // Only for debugging
	fprintf(stderr, "GLFW Version: %s\n", glfwGetVersionString()); // Only for debugging
	fprintf(stderr, "End =========================================================================\n");

	Shader shader;
	shader.ID = createProgram(readShaderFile("res/basic.vs"), readShaderFile("res/basic.fs"));
	const float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	vec3 cubePositions[] = {
		 0.0f,  0.0f,  0.0f,
		 2.0f,  5.0f, -15.0f,
		-1.5f, -2.2f, -2.5f,
		-3.8f, -2.0f, -12.3f,
		 2.4f, -0.4f, -3.5f,
		-1.7f,  3.0f, -7.5f,
		 1.3f, -2.0f, -2.5f,
		 1.5f,  2.0f, -2.5f,
		 1.5f,  0.2f, -1.5f,
		-1.3f,  1.0f, -1.5f
	};
	const unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

    Texture texture1 = loadTexture("res/container.jpg");
    Texture texture2 = loadTexture("res/awesomeface.png");

    useShader(shader.ID);
    setInt(shader.ID, "texture1", 0);
    setInt(shader.ID, "texture2", 1);

	// delta time
	float lastFrame = 0.0f;

	mat4 projection = GLM_MAT4_IDENTITY_INIT;
	mat4 view = GLM_MAT4_IDENTITY_INIT;

	vec3 cameraPos = {0.0f, 0.0f, 3.0f};
	vec3 cameraUp = {0.0f, 1.0f, 0.0f};
	initCamera(&camera, cameraPos, cameraUp, -90.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	GUIState state;
	state.state = DEMO_TEST_SHOW_ANOTHER_WINDOW;
	state.show = true;
	guiInit(window, &state);

	while (!glfwWindowShouldClose(window)) {
		const float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		guiUpdate(&state);

		// render
		glfwMakeContextCurrent(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1.ID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2.ID);

		useShader(shader.ID);
		glm_perspective(glm_rad(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f, projection);
		setMat4(shader.ID, "projection", projection);

		GetViewMatrix(camera, view);
		setMat4(shader.ID, "view", view);

		glBindVertexArray(VAO);
		for(unsigned int i = 1; i < 10; i++) {
			mat4 model = GLM_MAT4_IDENTITY_INIT;
			glm_translate(model, cubePositions[i]);
			const float angle = 20.0f * i;
			glm_rotate(model, glm_rad(angle), (vec3){1.0f, 0.3f, 0.5f});
			setMat4(shader.ID, "model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		guiRender(&state);

		glfwSwapBuffers(window);
		glfwPollEvents();

		processMouseMovement(&camera, 0, 0, true);
		if(mouseEnabled) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		} else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
	guiDestroy();

    glDeleteProgram(shader.ID);
    glDeleteTextures(1, &texture1.ID);
    glDeleteTextures(1, &texture2.ID);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();
	return 0;
}
