//
// Created by no-good-names on 11/4/2024.
//

#ifndef TEXTURES_H
#define TEXTURES_H

#include <glad/glad.h>

// TODO: Implement a function to load a texture from a file

enum TextureError {
	TEXTURE_LOAD_SUCCESS = 0,
	TEXTURE_LOAD_ERROR = 1
};

enum TextureWarning {
	TEXTURE_WARNING_NONE = 0,
	TEXTURE_WARNING_FILE_NOT_FOUND = 1,
	TEXTURE_WARNING_FILE_NOT_SUPPORTED = 2,
	TEXTURE_WARNING_FILE_OVERSIZED = 3 // File is too large to be loaded or might cause performance issues
};

typedef struct Texture_t {
	unsigned int ID;
    char *name;
} Texture;

Texture loadTexture(const char *filename);
void freeTexture(unsigned int ID);
void useTexture(unsigned int ID);

#endif //TEXTURES_H
