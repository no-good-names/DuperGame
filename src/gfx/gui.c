//
// Created by cp176 on 11/23/2024.
//

#include "gui.h"

void initGUI(GLFWwindow *window, const char *glsl_version) {
	igCreateContext(NULL);
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	igStyleColorsDark(NULL);
}
void renderGUI() {
	// disable depth testing
	glDisable(GL_DEPTH_TEST);
	// start new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	igNewFrame();
	// render your GUI
	igBegin("Hello, world!", NULL, 0);
	igText("This is some useful text.");
	igEnd();
	// render GUI
	igRender();
	ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
	// enable depth testing
	glEnable(GL_DEPTH_TEST);
}

void freeGUI() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	igDestroyContext(NULL);
}
