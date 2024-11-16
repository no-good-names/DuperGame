//
// Created by cp176 on 11/12/2024.
//

#include "camera.h"

void initCamera(struct Camera *camera, vec3 position, vec3 up, float yaw, float pitch) {
    glm_vec3_copy(position, camera->Position);
    glm_vec3_copy(up, camera->WorldUp);
    camera->Yaw = yaw;
    camera->Pitch = pitch;
    camera->MovementSpeed = SPEED;
    camera->MouseSensitivity = SENSITIVITY;
    camera->Zoom = ZOOM;

    updateCameraVectors(camera);
}


void processKeyboard(struct Camera *camera, enum Camera_Movement direction, float deltaTime) {
	vec3 velocity = {camera->MovementSpeed * deltaTime, camera->MovementSpeed * deltaTime, camera->MovementSpeed * deltaTime};
	vec3 cameraFrontTmp;
	if(direction == FORWARD) {
		glm_vec3_mul(camera->Front, velocity,cameraFrontTmp);
		glm_vec3_add(camera->Position, cameraFrontTmp, camera->Position);
	}
	if(direction == BACKWARD) {
		glm_vec3_mul(camera->Front, velocity,cameraFrontTmp);
		glm_vec3_sub(camera->Position, cameraFrontTmp, camera->Position);
	}
	if(direction == LEFT) {
		glm_vec3_crossn(camera->Front, camera->Up, cameraFrontTmp);
		glm_vec3_mul(cameraFrontTmp, velocity, cameraFrontTmp);
		glm_vec3_sub(camera->Position, cameraFrontTmp, camera->Position);
	}
	if(direction == RIGHT) {
		glm_vec3_crossn(camera->Front, camera->Up, cameraFrontTmp);
		glm_vec3_mul(cameraFrontTmp, velocity, cameraFrontTmp);
		glm_vec3_add(camera->Position, cameraFrontTmp, camera->Position);
	}
}
void processMouseMovement(struct Camera *camera, float xoffset, float yoffset, GLboolean constrainPitch) {
	xoffset *= camera->MouseSensitivity;
	yoffset *= camera->MouseSensitivity;

	camera->Yaw += xoffset;
	camera->Pitch += yoffset;

	// Constrain pitch to prevent camera flipping
	if (constrainPitch) {
		if (camera->Pitch > 89.0f) camera->Pitch = 89.0f;
		if (camera->Pitch < -89.0f) camera->Pitch = -89.0f;
	}

	updateCameraVectors(camera);
}

void processMouseScroll(struct Camera *camera, float yoffset) {
	camera->Zoom -= yoffset;
	if (camera->Zoom < 1.0f) camera->Zoom = 1.0f;
	if (camera->Zoom > 45.0f) camera->Zoom = 45.0f;
}

void updateCameraVectors(struct Camera *camera) {
	vec3 front;
	front[0] = cosf(glm_rad(camera->Yaw)) * cosf(glm_rad(camera->Pitch));
	front[1] = sinf(glm_rad(camera->Pitch));
	front[2] = sinf(glm_rad(camera->Yaw)) * cosf(glm_rad(camera->Pitch));
	glm_vec3_normalize_to(front, camera->Front);

	// Recalculate Right and Up vectors
	glm_vec3_cross(camera->Front, camera->WorldUp, camera->Right);
	glm_vec3_normalize(camera->Right);
	glm_vec3_cross(camera->Right, camera->Front, camera->Up);
	glm_vec3_normalize(camera->Up);
}


