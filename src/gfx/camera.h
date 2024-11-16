//
// Created by cp176 on 11/12/2024.
//

#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <cglm/cglm.h>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
#define YAW -90.0f
#define PITCH 0.0f
#define SPEED 2.5f
#define SENSITIVITY 0.1f
#define ZOOM 45.0f

struct Camera {
	vec3 Position;
	vec3 Front;
	vec3 Up;
	vec3 Right;
	vec3 WorldUp;
	float Yaw;
	float Pitch;
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;
};

void initCamera(struct Camera *camera, vec3 position, vec3 up, float yaw, float pitch);
void processKeyboard(struct Camera *camera, enum Camera_Movement direction, float deltaTime);
void processMouseMovement(struct Camera *camera, float xoffset, float yoffset, GLboolean constrainPitch);
void processMouseScroll(struct Camera *camera, float yoffset);
void updateCameraVectors(struct Camera *camera);
static void GetViewMatrix(struct Camera camera, mat4 view) {
	glm_lookat(camera.Position, (vec3){camera.Position[0] + camera.Front[0],
									   camera.Position[1] + camera.Front[1],
									   camera.Position[2] + camera.Front[2]},
			   camera.Up, view);
}

#endif //CAMERA_H
