/**
* SPG Assignment #2 - 2013
*
* Author: Stamate Cosmin, 342C4
*
* Description: Movable entity on X, Y, Z axes.
*/

#pragma once

#include "dependencies\glm\glm.hpp"
#include "dependencies\glm\gtx\rotate_vector.hpp"

#define MOVABLE_MOVE_SPEED 150

// Axes identifiers
#define MOVABLE_X_AXIS 0
#define MOVABLE_Y_AXIS 1
#define MOVABLE_Z_AXIS 2

#define MOVABLE_AXIS_COUNT 3

// Control identifiers.
#define MOVABLE_NO_CONTROL -1
#define MOVABLE_CONTROL_FORWARD 0
#define MOVABLE_CONTROL_BACKWARD 1
#define MOVABLE_CONTROL_UP 2
#define MOVABLE_CONTROL_DOWN 3
#define MOVABLE_CONTROL_RIGHT 4
#define MOVABLE_CONTROL_LEFT 5

// Link control identifiers to axis identifiers.
const int MOVABLE_CONTROL_AXIS[] = {
    MOVABLE_X_AXIS, MOVABLE_X_AXIS,
    MOVABLE_Y_AXIS, MOVABLE_Y_AXIS,
    MOVABLE_Z_AXIS, MOVABLE_Z_AXIS
};

// Axis movement direction.
const int MOVABLE_CONTROL_AXIS_DIRECTION[] = { 1, -1, 1, -1, 1, -1 };

class Movable {
public:
    Movable();
    Movable(glm::vec3 position, glm::vec3 forward, glm::vec3 right,
        glm::vec3 up);
    ~Movable();

    void initialize(glm::vec3 position, glm::vec3 forward, glm::vec3 right,
        glm::vec3 up);

    glm::vec3 move(float time, glm::vec2 angles);

    void translateX(float distance);
    void translateY(float distance);
    void translateZ(float distance);

    void rotateY(float angle);
    void rotateZ(float angle);

    void setControl(int control);
    void unsetControl(int control);

    glm::vec3 getPosition();
    glm::vec3 getForward();
    glm::vec3 getRight();
    glm::vec3 getUp();

private:
    void rotateRawZ(float angle);

public:
    glm::vec3 forward;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 position;
    float z_angle;
    int controls[MOVABLE_AXIS_COUNT];
};