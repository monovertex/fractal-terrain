/**
* SPG Assignment #2 - 2013
*
* Author: Stamate Cosmin, 342C4
*
* Description: Map generator.
* The algorithm used in generation is the diamond square fractal generator:
* http://www.gameprogrammer.com/fractal.html
* http://www.bluh.org/code-the-diamond-square-algorithm/
*/

#pragma once

#include "dependencies\glm\glm.hpp"
#include "lab_mesh_loader.h"
#include "raw_model.h"
#include <math.h>
#include <random>

// Map modes.
#define MAP_MODE_BASE 0
#define MAP_MODE_FRACTAL 1
#define MAP_MODE_FRACTAL_TESSELLATED_1X 2
#define MAP_MODE_FRACTAL_TESSELLATED_2X 3
#define MAP_MODE_FRACTAL_TESSELLATED_4X 4

#define MAP_MODE_COUNT 5

// Number of squares on the side of a terrain block, for the base mode.
#define MAP_SQUARE_COUNT 128

// The range in which the height is displaced for the fractal generation.
#define MAP_FRACTAL_DISPLACEMENT_RANGE 900.0f
static const float MAP_FRACTAL_Y_OFFSET = -MAP_FRACTAL_DISPLACEMENT_RANGE * 0.5f;

// Infinity number to mark un-initialized vertices.
#define MAP_INFINITY -2000.0f

// How big a block is compared to the visible area around the camera.
#define MAP_RADIUS_MULTIPLY 1.5f

// Colors for the mountains.
static const glm::vec4 MAP_TOP_COLOR = glm::vec4(0.95f, 0.95f, 0.95f, 1);
static const glm::vec4 MAP_BOTTOM_COLOR = glm::vec4(0.2f, 0.2f, 0.2f, 1);

// Boundary of the mountain colors, relative to the maximum / minimum height.
#define MAP_BOUNDARY_TOP 0.53f
#define MAP_BOUNDARY_TOP_HIGH 0.8f
#define MAP_BOUNDARY_BOTTOM 0.4f
#define MAP_BOUNDARY_BOTTOM_LOW 0.00f

// Materials used for the various modes.
static const RawModelMaterial material_neutral = RawModelMaterial(50,
    glm::vec4(0.18f, 0.18f, 0.18f, 1),
    glm::vec4(0.08f, 0.08f, 0.08f, 1),
    glm::vec4(0.1f, 0.1f, 0.1f, 1),
    glm::vec4(0.2f, 0.2f, 0.2f, 1));

static const RawModelMaterial material_mountains = RawModelMaterial(10,
    glm::vec4(0.39f, 0.36f, 0.29f, 1),
    glm::vec4(0.1f, 0.1f, 0.1f, 1),
    glm::vec4(0.19f, 0.19f, 0.09f, 1),
    glm::vec4(0.06f, 0.06f, 0.06f, 1));

static const RawModelMaterial* materials[] = {
    &material_neutral,
    &material_mountains, &material_mountains,
    &material_mountains, &material_mountains
};

// A vertex structure, containing position and normal.
struct MapVertex {
    glm::vec3 position;
    glm::vec3 normal;

    MapVertex();
    MapVertex(glm::vec3 position);
    MapVertex(glm::vec3 position, glm::vec3 normal);
};

// Block structure, containing the actual VBO information.
struct MapBlock {
    unsigned int vao;
    unsigned int vbo;
    unsigned int ibo;

    MapVertex* vertices;
    unsigned int* indexes;

    unsigned int square_count;
    unsigned int vertex_count;

    unsigned int total_square_count;
    unsigned int total_triangle_count;
    unsigned int total_index_count;
    unsigned int total_vertex_count;
    float square_size;
};

class Map {
public:
    Map(glm::vec3 position, float radius, unsigned int mode);
    ~Map();

    void setMode(unsigned int mode);
    void render(unsigned int shader, glm::mat4 model_matrix,
        glm::vec3 position);

    void generateBase(unsigned int mode, unsigned int square_count);
    void generateTerrain(unsigned int mode, unsigned int square_count);
    void tessellateTerrain(unsigned int source_mode, unsigned int mode);

    void generateFractal(unsigned int mode, unsigned int iteration);
    MapBlock* initializeBlock(unsigned int mode, unsigned int square_count);
    void bufferData();
    void computeNormals(unsigned int mode);

private:
    int mode;
    glm::vec3 position;
    float length;
    float radius;
    glm::vec2 boundary_top, boundary_bottom;
    MapBlock* blocks[MAP_MODE_COUNT];
};