/**
 * SPG Assignment #2 - 2013
 *
 * Author: Stamate Cosmin, 342C4
 *
 * Description: Generated world explorer.
 */

#pragma once
#include "lab_mesh_loader.h"
#include "lab_geometry.h"
#include "lab_shader_loader.h"
#include "lab_glut.h"
#include <ctime>
#include "raw_model.h"
#include "light_system.h"
#include "camera.h"
#include "map.h"
#include <math.h>

// Shaders used.
#define SHADER_FRAGMENT "shaders\\shader.frag"
#define SHADER_VERTEX "shaders\\shader.vert"

// Color values.
#define BACKGROUND_COLOR glm::vec4(0.886f, 0.941f, 0.953f, 1)

#define FOV 50.0f

#define MOUSE_MOVEMENT_FACTOR 0.08f

#define MAP_POSITION glm::vec3(0.0f, 0.0f, 0.0f)

class MainWindowListener : public lab::glut::WindowListener {
private:
    unsigned int shader;
    glm::mat4 model_matrix, projection_matrix;
    int previous_time;
    float time;
    float format;
    int width, height;
    bool wireframe;
    glm::vec2 camera_angles;
    bool warp_event;
    Camera* camera;
    LightSystem* light_system;
    Map* map;
    bool lights_on;
public:

    // Initializes various constants and pre-loads objects and shaders.
    MainWindowListener() {
        // Colorize the background and enable depth.
        glClearColor(BACKGROUND_COLOR.r, BACKGROUND_COLOR.g,
            BACKGROUND_COLOR.b, BACKGROUND_COLOR.a);
        glClearDepth(1);
        glEnable(GL_DEPTH_TEST);
        glutSetCursor(GLUT_CURSOR_NONE);

        RawModelFactory::instantiateModelFactory();

        // Load the shader.
        this->shader = lab::loadShader(SHADER_VERTEX, SHADER_FRAGMENT);

        // View and model matrix.
        this->model_matrix = glm::mat4();

        // Set draw mode.
        this->wireframe = false;
        glPolygonMode(GL_FRONT_AND_BACK, (this->wireframe ? GL_LINE : GL_FILL));

        // Initialize camera movement values.
        this->camera_angles = glm::vec2(0, 0);
        this->warp_event = false;

        this->width = 0;
        this->height = 0;

        this->lights_on = true;

        // Instantiate various objects.
        this->camera = new Camera();

        this->light_system = new LightSystem(LIGHT_OMNI, this->camera);

        this->map = new Map(MAP_POSITION, FOG_END_RADIUS, MAP_MODE_BASE);

        this->previous_time = glutGet(GLUT_ELAPSED_TIME);
    }

    // Unload shaders.
    ~MainWindowListener() {
        glDeleteProgram(this->shader);
        this->camera->~Camera();
        this->light_system->~LightSystem();
        this->map->~Map();
        RawModelFactory::destructModelFactory();
    }

    // Get time from last frame.
    void getTime() {
        int time_interval;
        int current_time;

        current_time = glutGet(GLUT_ELAPSED_TIME);
        time_interval = current_time - this->previous_time;
        previous_time = current_time;
        this->time = (float)time_interval / 1000.0f;
    }

    // Before drawing the actual scene, we do various updates to our logic.
    void notifyBeginFrame(){ this->getTime(); }

    // Draw the scene.
    void notifyDisplayFrame() {
        // Re-initialize buffers.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set the used shader.
        glUseProgram(this->shader);

        // Send global variables.
        glUniform4f(glGetUniformLocation(shader, "background_color"),
            BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b,
            BACKGROUND_COLOR.a);
        glUniformMatrix4fv(
            glGetUniformLocation(this->shader, "model_matrix"),
            1, false, glm::value_ptr(this->model_matrix));
        glUniformMatrix4fv(
            glGetUniformLocation(this->shader, "projection_matrix"),
            1, false, glm::value_ptr(this->projection_matrix));
        glUniform1i(glGetUniformLocation(this->shader, "lights_on"),
            this->lights_on);

        // Set drawing mode to fill, for other elements than the map.
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        this->camera->render(this->shader);

        this->light_system->render(this->shader, this->model_matrix);

        // Render the map as wire or fill, depending on current setting.
        glPolygonMode(GL_FRONT_AND_BACK, (this->wireframe ? GL_LINE : GL_FILL));

        this->map->render(shader, model_matrix, this->camera->getPosition());
    }

    // After the frame finished drawing.
    void notifyEndFrame() {
        // Move the camera based on registered mouse movement and keyboard.
        this->camera->move(this->time, this->camera_angles);

        // Set the light system's relative point and move the light system.
        this->light_system->setRelativePosition(this->camera->getPosition());
        this->light_system->move(this->time, this->camera_angles.x);

        // Reset mouse movement.
        this->camera_angles = glm::vec2(0, 0);
    }

    // When the window is resized.
    void notifyReshape(int width, int height, int previous_width,
        int previous_height) {
        if (height == 0) height = 1;
        glViewport(0, 0, width, height);
        this->width = width;
        this->height = height;
        this->format = (float)width / (float)height;
        this->projection_matrix = glm::perspective(FOV, format, 0.1f, 10000.0f);
    }

    // When a key is pressed.
    void notifyKeyPressed(unsigned char key_pressed, int mouse_x, int mouse_y) {
        switch (key_pressed) {
        case 27: lab::glut::close(); break; // Close the window.
        case 32: this->wireframe = !this->wireframe; break;
        case 'x': // Reload shader.
            glDeleteProgram(this->shader);
            this->shader = lab::loadShader(SHADER_VERTEX, SHADER_FRAGMENT);
            break;

        case 'z': this->light_system->switchFog(); break;
        case 'c': this->lights_on = !this->lights_on; break;
        case 'e': this->light_system->switchType(); break;
        case 'q': this->light_system->addLight(); break;

        case 'i': this->light_system->setControl(MOVABLE_CONTROL_FORWARD); break;
        case 'k': this->light_system->setControl(MOVABLE_CONTROL_BACKWARD); break;
        case 'l': this->light_system->setControl(MOVABLE_CONTROL_RIGHT); break;
        case 'j': this->light_system->setControl(MOVABLE_CONTROL_LEFT); break;
        case 'u': this->light_system->setControl(MOVABLE_CONTROL_DOWN); break;
        case 'o': this->light_system->setControl(MOVABLE_CONTROL_UP); break;

        case 'w': this->camera->setControl(MOVABLE_CONTROL_FORWARD); break;
        case 's': this->camera->setControl(MOVABLE_CONTROL_BACKWARD); break;
        case 'd': this->camera->setControl(MOVABLE_CONTROL_RIGHT); break;
        case 'a': this->camera->setControl(MOVABLE_CONTROL_LEFT); break;

        case '1': this->map->setMode(MAP_MODE_BASE); break;
        case '2': this->map->setMode(MAP_MODE_FRACTAL); break;
        case '3': this->map->setMode(MAP_MODE_FRACTAL_TESSELLATED_1X); break;
        case '4': this->map->setMode(MAP_MODE_FRACTAL_TESSELLATED_2X); break;
        case '5': this->map->setMode(MAP_MODE_FRACTAL_TESSELLATED_4X); break;
        }
    }

    // When a key is released.
    void notifyKeyReleased(unsigned char key_released, int mouse_x, int mouse_y) {
        switch (key_released) {
        case 'i': this->light_system->unsetControl(MOVABLE_CONTROL_FORWARD); break;
        case 'k': this->light_system->unsetControl(MOVABLE_CONTROL_BACKWARD); break;
        case 'l': this->light_system->unsetControl(MOVABLE_CONTROL_RIGHT); break;
        case 'j': this->light_system->unsetControl(MOVABLE_CONTROL_LEFT); break;
        case 'u': this->light_system->unsetControl(MOVABLE_CONTROL_DOWN); break;
        case 'o': this->light_system->unsetControl(MOVABLE_CONTROL_UP); break;

        case 'w': this->camera->unsetControl(MOVABLE_CONTROL_FORWARD); break;
        case 's': this->camera->unsetControl(MOVABLE_CONTROL_BACKWARD); break;
        case 'd': this->camera->unsetControl(MOVABLE_CONTROL_RIGHT); break;
        case 'a': this->camera->unsetControl(MOVABLE_CONTROL_LEFT); break;
        }
    }

    // When a special key is pressed.
    void notifySpecialKeyPressed(int key_pressed, int mouse_x, int mouse_y) {
        if (key_pressed == GLUT_KEY_F1) lab::glut::enterFullscreen();
        if (key_pressed == GLUT_KEY_F2) lab::glut::exitFullscreen();
    }

    // When a special key is released.
    void notifySpecialKeyReleased(int key_released, int mouse_x, int mouse_y) {}

    // Mouse events.
    void notifyMouseDrag(int mouse_x, int mouse_y) {}
    void notifyMouseMove(int mouse_x, int mouse_y) {
        // Move the mouse only if the viewport as normal dimensions.
        if (this->width <= 0 && this->height <= 0) return;

        // Snapping the mouse to the center causes this event to recurse, so
        // we prevent that manually.
        if (this->warp_event) {
            this->warp_event = false;
            return;
        }

        // Compute the screen center and snap the mouse to that.
        int cx = this->width / 2;
        int cy = this->height / 2;

        glutWarpPointer(cx, cy);

        // Compute mouse movement.
        float dx = float(cx - mouse_x) * MOUSE_MOVEMENT_FACTOR;
        float dy = float(cy - mouse_y) * MOUSE_MOVEMENT_FACTOR;

        this->camera_angles = glm::vec2(dx, dy);

        // Mark the event as already registered, to avoid infinite recursion.
        this->warp_event = true;
    }
    void notifyMouseClick(int button, int state, int mouse_x, int mouse_y) {}
    void notifyMouseScroll(int wheel, int direction, int mouse_x, int mouse_y) {}
};

int main() {
    // Initialize Glut.
    lab::glut::WindowInfo window(std::string("World Explorer"), 1000, 600, 100, 100, true);
    lab::glut::ContextInfo context(3, 3, false);
    lab::glut::FramebufferInfo framebuffer(true, true, true, true);
    lab::glut::init(window, context, framebuffer);

    // Initialize Glew.
    glewExperimental = true;
    glewInit();

    // Instantiate the window listener and register it.
    MainWindowListener listener;
    lab::glut::setListener(&listener);

    // Run the engine.
    lab::glut::run();

    return 0;
}