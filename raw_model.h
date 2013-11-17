/**
* SPG Assignment #2 - 2013
*
* Author: Stamate Cosmin, 342C4
*
* Description: Raw model factory. The factory is a singleton to ensure that
* models are loaded a single time throughout the program.
*/

#pragma once
#include "lab_mesh_loader.h"

#define RAW_MODEL_SPHERE 0 // Model types.
#define RAW_MODEL_CONE 1
#define RAW_MODEL_PLANE 2

#define RAW_MODEL_COUNT 3

struct RawModelInfo {
    char* path;
    glm::vec3 size;
};

// Material structure for proper light computation.
struct RawModelMaterial {
    int shininess;
    glm::vec4 ke;
    glm::vec4 ka;
    glm::vec4 kd;
    glm::vec4 ks;

    RawModelMaterial(int shininess, glm::vec4 ke, glm::vec4 ka, glm::vec4 kd,
        glm::vec4 ks) :
        shininess(shininess), ke(ke), ka(ka), kd(kd), ks(ks) {}
};

const RawModelInfo RAW_MODELS[] = { // Model attributes.
    { "resources\\sphere.obj", glm::vec3(10, 10, 10) } // Sphere
    , { "resources\\cone.obj", glm::vec3(5, 7, 5) } // Cone
    , { "resources\\plane.obj", glm::vec3(10, 10, 1) } // Plane
};

// The raw model class should not be used and contains various information
// about the predefined models, after they have been loaded.
class _RawModel {
public:
    _RawModel(const RawModelInfo* info);
    ~_RawModel();
    void render(RawModelMaterial* material, glm::vec3 position, glm::vec3 size,
        glm::mat4 model_matrix, glm::mat4 transform_matrix,
        unsigned int shader);

private:
    const RawModelInfo* info;
    unsigned int vao, vbo, ibo, index_count;
};

class RawModelFactory {
private:
    RawModelFactory();
    ~RawModelFactory();

public:
    static void instantiateModelFactory();
    static void destructModelFactory();
    static void render(unsigned int vao, unsigned int index_count,
        RawModelMaterial* material,
        glm::vec3 position, glm::vec3 size,
        glm::mat4 model_matrix, glm::mat4 transform_matrix,
        unsigned int shader);
    static void renderModel(int model_id, RawModelMaterial* material,
        glm::vec3 position, glm::vec3 size,
        glm::mat4 model_matrix, glm::mat4 transform_matrix,
        unsigned int shader);

private:
    static RawModelFactory* instance;
    static _RawModel* models[RAW_MODEL_COUNT];
};