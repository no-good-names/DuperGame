//
// Created by cp176 on 11/23/2024.
//

#ifndef GUI_H
#define GUI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>

#ifdef IMGUI_HAS_IMSTR
#define igBegin igBegin_Str
#define igSliderFloat igSliderFloat_Str
#define igCheckbox igCheckbox_Str
#define igColorEdit3 igColorEdit3_Str
#define igButton igButton_Str
#endif

enum GUIState_e {
	GUI_STATE_NONE = 0,
	GUI_STATE_MAIN_MENU,
	GUI_STATE_GAME,
	GUI_STATE_PAUSE_MENU,
	GUI_STATE_SETTINGS,
	GUI_STATE_GAME_OVER,
	GUI_HIDDEN,
	DEMO_TEST_WINDOW = 100,
	DEMO_TEST_SHOW_ANOTHER_WINDOW = 101
};

typedef struct GUIState_t {
	enum GUIState_e state;
	bool show;
	// docking
	ImGuiIO *ioptr;
} GUIState;

void guiInit(GLFWwindow* window, GUIState *state);
void guiUpdate(GUIState *state);
void guiRender(GUIState *state);
void guiDestroy();

#endif //GUI_H
