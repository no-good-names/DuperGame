#include "shader.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cglm/cglm.h>

char* readShaderFile(const char* filename) {
	FILE* file = fopen(filename, "r");
	if (!file) {
		fprintf(stderr, "Error: Could not open shader file %s\n", filename);
		return NULL;
	}

	// Allocate an initial buffer size (we will grow it dynamically)
	size_t bufferSize = 1024;
	char* shaderCode = (char*)malloc(bufferSize);
	if (!shaderCode) {
		fprintf(stderr, "Error: Could not allocate memory\n");
		fclose(file);
		return NULL;
	}

	// Temporary buffer to store each line
	char lineBuffer[256];
	shaderCode[0] = '\0';  // Initialize shaderCode as an empty string

	// Read file line by line
	while (fgets(lineBuffer, sizeof(lineBuffer), file)) {
		// Ensure we have enough space in shaderCode
		if (strlen(shaderCode) + strlen(lineBuffer) + 1 > bufferSize) {
			bufferSize *= 2;  // Double the buffer size
			shaderCode = (char*)realloc(shaderCode, bufferSize);
			if (!shaderCode) {
				fprintf(stderr, "Error: Could not reallocate memory\n");
				fclose(file);
				return NULL;
			}
		}

		// Append the line and ensure it ends with a newline
		strcat(shaderCode, lineBuffer);
	}

	fclose(file);
	return shaderCode;
}

void compile_error(const unsigned int source) {
	int status;
	glGetShaderiv(source, GL_COMPILE_STATUS, &status);
if(!status)
	{
		char log[512];
		glGetShaderInfoLog(source, 512, NULL, log);
		fprintf(stderr, "ERROR::SHADER::VERTEX/FRAGMENT::COMPILATION_FAILED\n");
	}
}

void link_err(const unsigned int source) {
	int success;
	glGetProgramiv(source, GL_LINK_STATUS, &success);
	if(!success) {
		char infoLog[512];
		glGetProgramInfoLog(source, 512, NULL, infoLog);
		fprintf(stderr, "err %s", infoLog);
	}
}

unsigned int createProgram(const char *vss, const char *fss) {
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vss, NULL);
	glCompileShader(vertexShader);

	compile_error(vertexShader);

	const unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fss, NULL);
	glCompileShader(fragmentShader);

	compile_error(fragmentShader);

	const unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	link_err(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

void useShader(const unsigned int ID) {
    glUseProgram(ID);
}

void setBool(const unsigned int ID, const char *name, int value) {
    glUniform1i(glGetUniformLocation(ID, name), (int) value);
}

void setInt(const unsigned int ID, const char *name, int value) {
    glUniform1i(glGetUniformLocation(ID, name), value);
}

void setFloat(const unsigned int ID, const char *name, float value) {
    glUniform1f(glGetUniformLocation(ID, name), value);
}
void setVec2(const unsigned int ID, const char *name, const vec2 value) {
	glUniform2fv(glGetUniformLocation(ID, name), 1, (float*)value);
}
void setVec3(const unsigned int ID, const char *name, const vec3 value) {
	glUniform3fv(glGetUniformLocation(ID, name), 1, (float*)value);
}
void setVec4(const unsigned int ID, const char *name, const vec4 value) {
	glUniform4fv(glGetUniformLocation(ID, name), 1, (float*)value);
}
void setMat2(const unsigned int ID, const char *name, const mat2 value) {
	glUniformMatrix2fv(glGetUniformLocation(ID, name), 1, GL_FALSE, (float*)value);
}
void setMat3(const unsigned int ID, const char *name, const mat3 value) {
	glUniformMatrix3fv(glGetUniformLocation(ID, name), 1, GL_FALSE, (float*)value);
}
void setMat4(const unsigned int ID, const char *name, const mat4 value) {
	glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, (float*)value);
}
