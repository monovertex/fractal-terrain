/**
* SPG Assignment #2 - 2013
*
* Author: Stamate Cosmin, 342C4
*
* Description: Generate 2D noise, based on the Perlin algorithm.
* Tutorial found here: http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
*/

#pragma once
#include "noise.h"

namespace noise {
    float generate(float x, float y) {
        float noise = 0;
        float persistence = NOISE_PERSISTENCE;
        float frequency, amplitude;

        for (int i = 0; i < NOISE_OCTAVE_COUNT; i++) {
            frequency = pow(2.0f, i);
            amplitude = pow(persistence, i);

            noise += noise::_interpolateNoise(x * frequency, y * frequency) *
                amplitude;
        }

        return noise;
    }

    float _interpolateNoise(float x, float y) {
        float x_int = floor(x);
        float y_int = floor(y);
        float x_fract = x - x_int;
        float y_fract = y - y_int;

        float v1 = noise::_smooth(x_int, y_int);
        float v2 = noise::_smooth(x_int + 1, y_int);
        float v3 = noise::_smooth(x_int, y_int + 1);
        float v4 = noise::_smooth(x_int + 1, y_int + 1);

        float i1 = noise::_interpolate(v1, v2, x_fract);
        float i2 = noise::_interpolate(v3, v4, x_fract);

        return noise::_interpolate(i1, i2, y_fract);
    }

    float _interpolate(float a, float b, float x) {
        float ft = x * 3.1416f;
        float f = (1.0f - cos(ft)) * 0.5f;

        return  a * (1.0f - f) + b * f;
    }

    float _smooth(float x, float y) {
        float corners = (
            noise::_random((int)x - 1, (int)y - 1) +
            noise::_random((int)x + 1, (int)y - 1) +
            noise::_random((int)x - 1, (int)y + 1) +
            noise::_random((int)x + 1, (int)y + 1)
            ) / 16.0f;
        float sides = (
            noise::_random((int)x - 1, (int)y) +
            noise::_random((int)x + 1, (int)y) +
            noise::_random((int)x, (int)y - 1) +
            noise::_random((int)x, (int)y + 1)
            ) / 8.0f;
        float center = noise::_random((int)x, (int)y) / 4.0f;

        return corners + sides + center;
    }

    float _random(int x, int y) {
        int n = x + y * 57;
        n = (n << 13) ^ n;
        return (float)(((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) /
            1073741824.0);
    }
}