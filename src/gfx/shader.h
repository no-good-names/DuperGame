//
// Created by no-good-names on 11/2/2024.
//

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <cglm/cglm.h>

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	unsigned int ID;
} Shader;

char* readShaderFile(const char* filename);

unsigned int createProgram(const char *vss, const char *fss);
void useShader(unsigned int ID);
void setBool(unsigned int ID, const char* name, int value);
void setInt(unsigned int ID, const char* name, int value);
void setFloat(unsigned int ID, const char* name, float value);
void setMat4(unsigned int ID, const char* name, const mat4 value);

#endif //SHADER_H
