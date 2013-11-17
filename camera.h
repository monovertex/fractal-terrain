/**
* SPG Assignment #2 - 2013
*
* Author: Stamate Cosmin, 342C4
*
* Description: Camera entity, based on the movable class. Used mainly for
* the principal viewer camera.
*/

#pragma once

#include "movable.h"
#include "dependencies\glm\glm.hpp"
#include "dependencies\glm\gtc\type_ptr.hpp"
#include "dependencies\glew\glew.h"

class Camera : public Movable {
public:
    Camera();
    Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 right, glm::vec3 up);
    ~Camera();

    void render(unsigned int shader);
};