/**
* SPG Assignment #2 - 2013
*
* Author: Stamate Cosmin, 342C4
*
* Description: Light ensemble system, controlling all the light sources and
* the main light source, the sun.
*/

#include <stdlib.h>
#include <time.h>
#include "dependencies\glm\glm.hpp"
#include "dependencies\glm\gtx\vector_angle.hpp"
#include "raw_model.h"
#include "movable.h"
#include "camera.h"
#include "lab_texture_loader.h"

// Fog constants.
#define FOG_START_RADIUS 900.0f
#define FOG_END_RADIUS 2100.0f
#define FOG_COLOR glm::vec4(1, 0.996f, 0.549f, 1)

// Types of lights.
#define LIGHT_OMNI 0
#define LIGHT_SPOT 1

// Light constants.
#define LIGHT_SPEED 10.0f // Movement speed.

// Maximum radius from the relative point, in which lights are instantiated.
#define LIGHT_MAXIMUM_RADIUS 200.0f
static const float LIGHT_MAXIMUM_RADIUS_2 = LIGHT_MAXIMUM_RADIUS * 2.0f;

// Height range in which lights are created from the relative point.
#define LIGHT_MAXIMUM_HEIGHT 15.0f
static const float LIGHT_MAXIMUM_HEIGHT_2 = LIGHT_MAXIMUM_HEIGHT * 2.0f;

// Light color constants.
static const glm::vec4 LIGHT_MINIMUM_COLOR = glm::vec4(0.5f, 0.5f, 0.5f, 1);
static const glm::vec4 LIGHT_MAXIMUM_COLOR = glm::vec4(0.95f, 0.95f, 0.95f, 1);
static const glm::vec4 LIGHT_RANGE_COLOR = LIGHT_MAXIMUM_COLOR - LIGHT_MINIMUM_COLOR;

// Spotlight constants.
static const float LIGHT_MAXIMUM_SPOT_ANGLE = glm::radians(50.0f);
static const float LIGHT_MINIMUM_SPOT_ANGLE = glm::radians(15.0f);
static const float LIGHT_FADE_SPOT_ANGLE = glm::radians(10.0f);
static const float LIGHT_RANGE_SPOT_ANGLE = (LIGHT_MAXIMUM_SPOT_ANGLE -
    LIGHT_MINIMUM_SPOT_ANGLE);

static const glm::vec3 LIGHT_SPOT_DIRECTION = glm::vec3(0, -1, 0);

// Light sizes, influence the light's influence area.
#define LIGHT_MINIMUM_SIZE 5.0f
#define LIGHT_MAXIMUM_SIZE 15.0f
static const float LIGHT_RANGE_SIZE = LIGHT_MAXIMUM_SIZE - LIGHT_MINIMUM_SIZE;

// Shininess of the light objects.
#define LIGHT_SHININESS 100

// Global ambiental color value.
static const glm::vec4 LIGHT_AMBIENTAL = glm::vec4(0.05f, 0.05f, 0.05f, 1);

// Maximum number of lights.
#define LIGHT_MAXIMUM_COUNT 50

// Sun constants.
#define SUN_SIZE 1500.0f
const static glm::vec4 SUN_COLOR = glm::vec4(0.988, 1, 0.678, 1);
const static glm::vec3 SUN_POSITION = glm::vec3(-6000.0f, 800.0f, 0);
static const RawModelMaterial SUN_MATERIAL = RawModelMaterial(
    100, SUN_COLOR,
    glm::vec4(0, 0, 0, 0), glm::vec4(0, 0, 0, 0), glm::vec4(0, 0, 0, 0));

class Sun : public Movable {
public:
    Sun(Camera* camera);
    ~Sun();

    void render(unsigned int shader, glm::mat4 model_matrix);
private:
    Camera* camera;
    unsigned int texture;
};

class Light {
public:
    Light(unsigned int type, glm::vec3 position, RawModelMaterial* material,
        float size);
    ~Light();

    void setType(unsigned int type);
    void move(glm::vec3 movement);
    void render(unsigned int shader, glm::vec3 offset, glm::mat4 model_matrix);

    glm::vec3 getPosition();

private:
    glm::vec3 position;
    glm::vec3 size;
    unsigned int type;
    RawModelMaterial* material;
};

class LightSystem : public Movable {
public:
    LightSystem(unsigned int type, Camera* camera);
    ~LightSystem();

    void addLight();
    void switchType();
    void setRelativePosition(glm::vec3 position);
    void switchFog();

    void move(float time, float angle_y);
    void render(unsigned int shader, glm::mat4 model_matrix);

private:
    glm::vec3 relative_position;
    Light* lights[LIGHT_MAXIMUM_COUNT];
    glm::vec3 light_positions[LIGHT_MAXIMUM_COUNT];
    glm::vec4 light_colors[LIGHT_MAXIMUM_COUNT];
    float light_sizes[LIGHT_MAXIMUM_COUNT];
    float light_inner_angles[LIGHT_MAXIMUM_COUNT];
    float light_outer_angles[LIGHT_MAXIMUM_COUNT];
    int light_count;
    unsigned int type;
    Sun* sun;
    bool fog;
};