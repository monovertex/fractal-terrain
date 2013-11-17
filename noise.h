/**
* SPG Assignment #2 - 2013
*
* Author: Stamate Cosmin, 342C4
*
* Description: Generate 2D noise, based on the Perlin algorithm.
* Tutorial found here: http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
*/

#pragma once;
#include <math.h>

#define NOISE_PERSISTENCE 0.6f
#define NOISE_OCTAVE_COUNT 3

namespace noise {
    float generate(float x, float y);

    float _interpolateNoise(float x, float y);
    float _interpolate(float a, float b, float x);
    float _smooth(float x, float y);
    float _random(int x, int y);
}