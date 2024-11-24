//
// Created by cp176 on 11/23/2024.
//

#ifndef GUI_H
#define GUI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include <cimgui_impl.h>

#ifdef IMGUI_HAS_IMSTR
#define igBegin igBegin_Str
#define igSliderFloat igSliderFloat_Str
#define igCheckbox igCheckbox_Str
#define igColorEdit3 igColorEdit3_Str
#define igButton igButton_Str
#endif

void initGUI(GLFWwindow *window, const char *glsl_version);
void renderGUI();
void freeGUI();

#endif //GUI_H
