//
// Created by no-good-names on 11/2/2024.
//

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <cglm/cglm.h>

#include <stdio.h>
#include <stdlib.h>

typedef struct Shader_t {
	unsigned int ID;
} Shader;

char* readShaderFile(const char* filename);

#ifdef SHADER_DEBUG
void compile_error(const unsigned int source);
void link_err(const unsigned int source);
#endif

unsigned int createProgram(const char *vss, const char *fss);
void useShader(unsigned int ID);
void setBool(unsigned int ID, const char* name, int value);
void setInt(unsigned int ID, const char* name, int value);
void setFloat(unsigned int ID, const char* name, float value);
void setMat4(unsigned int ID, const char* name, const mat4 value);

#endif //SHADER_H
