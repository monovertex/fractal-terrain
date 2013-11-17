/**
* SPG Assignment #2 - 2013
*
* Author: Stamate Cosmin, 342C4
*
* Description: Camera entity, based on the movable class. Used mainly for
* the principal viewer camera.
*/

#pragma once
#include "camera.h"

// Default constructor.
Camera::Camera() : Movable() {}

// Constructor with specified vectors.
Camera::Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 right,
    glm::vec3 up)
    : Movable(position, forward, right, up) {}

// Deconstructor.
Camera::~Camera() {}

// Render the camera based on current vector information.
void Camera::render(unsigned int shader) {
    glm::vec3 eye = this->position;
    glm::vec3 center;

    center = eye + this->forward;
    glm::mat4 view_matrix = glm::lookAt(eye, center, this->up);

    glUniformMatrix4fv(glGetUniformLocation(shader, "view_matrix"),
        1, false, glm::value_ptr(view_matrix));
    glUniform3f(glGetUniformLocation(shader, "eye_position"),
        eye.x, eye.y, eye.z);
}