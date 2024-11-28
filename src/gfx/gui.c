//
// Created by cp176 on 11/23/2024.
//

// NOTE: This file is not yet implemented
// DEPENDENCIES: glfw, glad, cimgui
// TODO: Implement GUI functions

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include <cimgui_impl.h>

#include "gui.h"
void guiInit(GLFWwindow* window, GUIState *state) {
#ifdef __APPLE__
	const char *glsl_version = "#version 150";
#else
	const char *glsl_version = "#version 130";
#endif
	igCreateContext(NULL);

	// set docking
	state->ioptr = igGetIO();
	state->ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
	//ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
#ifdef IMGUI_HAS_DOCK
	state->ioptr->ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
	state->ioptr->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows
#endif

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	igStyleColorsDark(NULL);
	// ImFontAtlas_AddFontDefault(io.Fonts, NULL);

}

void guiUpdate(GUIState *state) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	igNewFrame();

	if (state->state == DEMO_TEST_SHOW_ANOTHER_WINDOW)
		igShowDemoWindow(&state->show);

	// show a simple window that we created ourselves.
	{
		static float f = 0.0f;
		static int counter = 0;

		igBegin("Hello, world!", NULL, 0);
		igText("This is some useful text");
		igCheckbox("Demo window", &state->show);
		igCheckbox("Another window", &state->show);

		igSliderFloat("Float", &f, 0.0f, 1.0f, "%.3f", 0);
		igColorEdit3("clear color", (float *)&(ImVec4) {0.45f, 0.55f, 0.60f, 1.00f}, 0); // just for testing

		ImVec2 buttonSize;
		buttonSize.x = 0;
		buttonSize.y = 0;
		if (igButton("Button", buttonSize))
			counter++;
		igSameLine(0.0f, -1.0f);
		igText("counter = %d", counter);

		igText("Application average %.3f ms/frame (%.1f FPS)",
			   1000.0f / igGetIO()->Framerate, igGetIO()->Framerate);
		igEnd();
	}

	if (state->show)
	{
		igBegin("imgui Another Window", &state->show, 0);
		igText("Hello from imgui");
		ImVec2 buttonSize;
		buttonSize.x = 0;
		buttonSize.y = 0;
		if (igButton("Close me", buttonSize)) {
			state->show = false;
		}
		igEnd();
	}
}

void guiRender(GUIState *state) {

	ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
#ifdef IMGUI_HAS_DOCK
	if (state->ioptr->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow *backup_current_window = glfwGetCurrentContext();
		igUpdatePlatformWindows();
		igRenderPlatformWindowsDefault(NULL, NULL);
		glfwMakeContextCurrent(backup_current_window);
	}
#endif
}

void guiDestroy() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	igDestroyContext(NULL);
}
