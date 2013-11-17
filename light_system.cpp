/**
* SPG Assignment #2 - 2013
*
* Author: Stamate Cosmin, 342C4
*
* Description: Light ensemble system, controlling all the light sources and
* the main light source, the sun.
*/

#include "light_system.h"

// Sun constructor. Loads the texture and initializes the vectors.
Sun::Sun(Camera* camera)
:Movable(SUN_POSITION, glm::vec3(), glm::vec3(), glm::vec3()) {
    this->camera = camera;

    this->texture = lab::loadTextureBMP("resources\\sun.bmp");
}

// Deconstructor.
Sun::~Sun() {
    glDeleteBuffers(1, &(this->texture));
}

// Render the sun. The sun is composed of a plane with a texture applied to it.
// For realism, it is always facing the the camera.
// The sun is also placed always at a fixed position, relative to the camera,
// so it always stays away enough from the camera.
void Sun::render(unsigned int shader, glm::mat4 model_matrix) {
    // Update the sun's position.
    glm::vec3 sun_position = this->position +
        glm::vec3(this->camera->position.x, 0, 0);

    // Compute the sun vectors such that it faces the camera.
    this->forward = glm::normalize(this->camera->position - sun_position);
    this->right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), this->forward));
    this->up = glm::normalize(glm::cross(this->forward, this->right));

    // Build the rotation matrix based on previous calculated vectors.
    glm::mat4 rotation_matrix;
    rotation_matrix[0] = glm::vec4(this->right, 0);
    rotation_matrix[1] = glm::vec4(this->up, 0);
    rotation_matrix[2] = glm::vec4(this->forward, 0);

    // Send information to the shader.
    glUniform1i(glGetUniformLocation(shader, "draw_sun"), true);

    glUniform3f(glGetUniformLocation(shader, "sun_position"),
        sun_position.x, sun_position.y, sun_position.z);
    glUniform4f(glGetUniformLocation(shader, "sun_color"),
        SUN_COLOR.r, SUN_COLOR.g, SUN_COLOR.b, SUN_COLOR.a);
    glUniform1f(glGetUniformLocation(shader, "sun_size"), SUN_SIZE);

    // We're no longer drawing fog for the sun plane, to ensure realism.
    glUniform1i(glGetUniformLocation(shader, "fog_switch"), false);

    // Send texture data to the shader.
    glActiveTexture(GL_TEXTURE0 + 4);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glUniform1i(glGetUniformLocation(shader, "sun_texture"), 4);

    // Render the basic plane.
    RawModelFactory::renderModel(RAW_MODEL_PLANE,
        (RawModelMaterial*)&SUN_MATERIAL,
        sun_position,
        glm::vec3(SUN_SIZE, SUN_SIZE, 1),
        model_matrix, rotation_matrix, shader);

    // Notify the shader we're no longer rendering the sun plane.
    glUniform1i(glGetUniformLocation(shader, "draw_sun"), false);
}

// Instantiate a simple light, with its variables.
Light::Light(unsigned int type, glm::vec3 position, RawModelMaterial* material,
    float size) {
    this->position = position;
    this->type = type;
    this->material = material;
    this->size = glm::vec3(size, size, size);
}

Light::~Light() {}

// Set a light's type.
void Light::setType(unsigned int type) { this->type = type; }

// Set a light's position.
void Light::move(glm::vec3 movement) {
    this->position = this->position + movement;
}

// Get a light's position.
glm::vec3 Light::getPosition() { return this->position; }

// Render a simple model to give a hint as what the light is.
void Light::render(unsigned int shader, glm::vec3 offset,
    glm::mat4 model_matrix) {
    // If it is a point light, draw a sphere.
    if (this->type == LIGHT_OMNI) {
        RawModelFactory::renderModel(RAW_MODEL_SPHERE, material,
            this->position + offset, this->size, model_matrix, glm::mat4(),
            shader);
    } // If it is a spotlight, draw a cone.
    else {
        RawModelFactory::renderModel(RAW_MODEL_CONE, material,
            this->position + offset, this->size, model_matrix, glm::mat4(),
            shader);
    }
}

// Assign variables, initialize the simple random number generator from C++.
LightSystem::LightSystem(unsigned int type, Camera* camera)
: Movable(glm::vec3(0, 0, 0), camera->forward, camera->right, camera->up) {
    this->type = type;
    this->setRelativePosition(camera->position);
    this->light_count = 0;
    this->fog = true;

    // Initialize random seed.
    srand((unsigned int)time(NULL));

    this->sun = new Sun(camera);
}

// Deconstructor.
LightSystem::~LightSystem() {}

// Adds a new light to the system.
void LightSystem::addLight() {
    // Add a light only if there's still enough space.
    if (this->light_count < LIGHT_MAXIMUM_COUNT) {
        // Compute the random position.
        glm::vec3 position = this->position + glm::vec3(
            (rand() % 100) / 100.0f *
            LIGHT_MAXIMUM_RADIUS_2 - LIGHT_MAXIMUM_RADIUS,
            (rand() % 100) / 100.0f *
            LIGHT_MAXIMUM_HEIGHT_2 - LIGHT_MAXIMUM_HEIGHT,
            (rand() % 100) / 100.0f *
            LIGHT_MAXIMUM_RADIUS_2 - LIGHT_MAXIMUM_RADIUS);

        // Compute the basic color.
        glm::vec4 color = glm::vec4(
            LIGHT_MINIMUM_COLOR.r + (rand() % 100) / 100.0f * LIGHT_RANGE_COLOR.r,
            LIGHT_MINIMUM_COLOR.g + (rand() % 100) / 100.0f * LIGHT_RANGE_COLOR.g,
            LIGHT_MINIMUM_COLOR.b + (rand() % 100) / 100.0f * LIGHT_RANGE_COLOR.b,
            LIGHT_MINIMUM_COLOR.a + (rand() % 100) / 100.0f * LIGHT_RANGE_COLOR.a);

        // Compute the size.
        float size = LIGHT_MINIMUM_SIZE +
            (rand() % 100) / 100.0f * LIGHT_MAXIMUM_SIZE;

        // Compute the spotlight angles.
        float inner_angle = LIGHT_MINIMUM_SPOT_ANGLE +
            (rand() % 100) / 100.0f * LIGHT_RANGE_SPOT_ANGLE;
        float outer_angle = inner_angle + LIGHT_FADE_SPOT_ANGLE;

        // Assign the material, based on the color.
        RawModelMaterial* material = new RawModelMaterial(LIGHT_SHININESS,
            color * 1.2f, color, color, color * 1.4f);

        // Instantiate the light objec.
        Light* new_light = new Light(this->type, position, material, size);

        // Assign the light variables, for later use.
        this->lights[this->light_count] = new_light;
        this->light_colors[this->light_count] = color;
        this->light_sizes[this->light_count] = size;
        this->light_inner_angles[this->light_count] = glm::cos(inner_angle);
        this->light_outer_angles[this->light_count] = glm::cos(outer_angle);

        this->light_count++;
    }
}

// Switch the lighting system from point to spotlight and vice-versa.
void LightSystem::switchType() {
    if (this->type == LIGHT_OMNI) this->type = LIGHT_SPOT;
    else this->type = LIGHT_OMNI;

    for (int i = 0; i < this->light_count; i++) {
        this->lights[i]->setType(type);
    }
}

// Update the system's relative position.
void LightSystem::setRelativePosition(glm::vec3 position) {
    this->relative_position = position;
}

// Move the light system on the intended path. The system is actually rendered
// based on the relative position.
void LightSystem::move(float time, float angle_y) {
    glm::vec3 movement = Movable::move(time, glm::vec2(0, 0));

    // Move all the lights.
    for (int i = 0; i < this->light_count; i++) {
        this->lights[i]->move(movement);
    }

    this->rotateY(angle_y);
}

// Switch the fog on and off.
void LightSystem::switchFog() { this->fog = !this->fog; }

// Render the light system.
void LightSystem::render(unsigned int shader, glm::mat4 model_matrix) {
    glm::vec3 offset = this->relative_position;

    // First render the sun's model and light.
    this->sun->render(shader, model_matrix);

    // Turn the fog on or off and send fog variables.
    glUniform1i(glGetUniformLocation(shader, "fog_switch"), this->fog);
    glUniform1f(glGetUniformLocation(shader, "fog_start"), FOG_START_RADIUS);
    glUniform1f(glGetUniformLocation(shader, "fog_end"), FOG_END_RADIUS);

    glUniform4f(glGetUniformLocation(shader, "fog_color"),
        FOG_COLOR.x, FOG_COLOR.y, FOG_COLOR.z, FOG_COLOR.a);

    // Send ambiental light color.
    glUniform4f(glGetUniformLocation(shader, "ambiental_light"),
        LIGHT_AMBIENTAL.x, LIGHT_AMBIENTAL.y, LIGHT_AMBIENTAL.z,
        LIGHT_AMBIENTAL.a);

    // Send global light information.
    glUniform1i(glGetUniformLocation(shader, "light_type"), this->type);
    glUniform3f(glGetUniformLocation(shader, "spotlight_direction"),
        LIGHT_SPOT_DIRECTION.x, LIGHT_SPOT_DIRECTION.y,
        LIGHT_SPOT_DIRECTION.z);

    // Render all the individual light models.
    for (int i = 0; i < this->light_count; i++) {
        this->lights[i]->render(shader, offset, model_matrix);
        this->light_positions[i] = offset + this->lights[i]->getPosition();
    }

    // Send light sources information to the shader.
    glUniform1i(glGetUniformLocation(shader, "light_count"), this->light_count);
    glUniform1i(glGetUniformLocation(shader, "light_type"), this->type);
    glUniform3fv(glGetUniformLocation(shader, "light_positions"),
        this->light_count, (GLfloat*)this->light_positions);
    glUniform4fv(glGetUniformLocation(shader, "light_colors"),
        this->light_count, (GLfloat*)this->light_colors);
    glUniform1fv(glGetUniformLocation(shader, "light_inner_angles"),
        this->light_count, (GLfloat*)this->light_inner_angles);
    glUniform1fv(glGetUniformLocation(shader, "light_outer_angles"),
        this->light_count, (GLfloat*)this->light_outer_angles);
    glUniform1fv(glGetUniformLocation(shader, "light_sizes"),
        this->light_count, (GLfloat*)this->light_sizes);
}