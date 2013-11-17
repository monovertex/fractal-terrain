/**
* SPG Assignment #2 - 2013
*
* Author: Stamate Cosmin, 342C4
*
* Description: Movable entity on X, Y, Z axes.
*/

#pragma once
#include "movable.h"

// Initialize vectors and control signals.
void Movable::initialize(glm::vec3 position, glm::vec3 forward, glm::vec3 right,
    glm::vec3 up) {
    this->position = position;
    this->forward = forward;
    this->right = right;
    this->up = up;
    this->z_angle = 0;

    for (int i = 0; i < MOVABLE_AXIS_COUNT; i++)
        this->controls[i] = MOVABLE_NO_CONTROL;
}
Movable::~Movable() {}

// Constructors.
Movable::Movable(){
    this->initialize(glm::vec3(0, 200, 0), glm::vec3(-1, 0, 0),
        glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
}
Movable::Movable(glm::vec3 position, glm::vec3 forward, glm::vec3 right,
    glm::vec3 up) {
    this->initialize(position, forward, right, up);
}

// Move the entity taking the signals into account.
glm::vec3 Movable::move(float time, glm::vec2 angles) {
    float distance = time * MOVABLE_MOVE_SPEED;
    float direction_distance;
    glm::vec3 previous_position = this->position;

    // Test each signal and translate in corresponding direction.
    for (int i = 0; i < MOVABLE_AXIS_COUNT; i++) {
        if (this->controls[i] != MOVABLE_NO_CONTROL) {
            direction_distance = distance *
                MOVABLE_CONTROL_AXIS_DIRECTION[this->controls[i]];

            switch (i){
            case MOVABLE_X_AXIS: this->translateX(direction_distance); break;
            case MOVABLE_Y_AXIS: this->translateY(direction_distance); break;
            case MOVABLE_Z_AXIS: this->translateZ(direction_distance); break;
            }
        }
    }

    // We only rotate on Y and Z axes.
    this->rotateY(angles.x);
    this->rotateZ(angles.y);

    // Return the distance travelled.
    return this->position - previous_position;
}

// Translate entity forward, up or right.
void Movable::translateX(float distance){
    this->position += this->forward * distance;
}
void Movable::translateY(float distance){
    this->position += this->up * distance;
}
void Movable::translateZ(float distance){
    this->position += this->right * distance;
}

// Rotate the entity on yaw and pitch.
void Movable::rotateY(float angle){
    // In order to avoid rotating on X axis, we first have to revert the Z
    // rotation, rotate on Y and then rotate back to the original Z offset.
    this->rotateRawZ(-this->z_angle);
    this->forward = glm::rotate(this->forward, angle, this->up);
    this->right = glm::rotate(this->right, angle, this->up);
    this->rotateRawZ(this->z_angle);
}
void Movable::rotateZ(float angle){
    this->z_angle += angle;
    this->rotateRawZ(angle);
}
void Movable::rotateRawZ(float angle){
    this->forward = glm::rotate(this->forward, angle, this->right);
    this->up = glm::rotate(this->up, angle, this->right);
}

// Activate / de-activate controls.
void Movable::setControl(int control) {
    this->controls[MOVABLE_CONTROL_AXIS[control]] = control;
}
void Movable::unsetControl(int control) {
    this->controls[MOVABLE_CONTROL_AXIS[control]] = MOVABLE_NO_CONTROL;
}

// Retrieve various vectors.
glm::vec3 Movable::getPosition() { return this->position; }
glm::vec3 Movable::getForward() { return this->forward; }
glm::vec3 Movable::getRight() { return this->right; }
glm::vec3 Movable::getUp() { return this->up; }