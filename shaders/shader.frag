#version 330
layout(location = 0) out vec4 out_color;

uniform vec3 eye_position;
in vec3 world_pos;
in vec3 world_normal;
in vec2 texcoord;

//==============================================================================
// Simplex Noise function, used in mountain snow and grass generation.

// Description : Array and textureless GLSL 2D simplex noise function.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
//

vec3 mod289(vec3 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec2 mod289(vec2 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec3 permute(vec3 x) { return mod289(((x*34.0)+1.0)*x); }

float snoise(vec2 v)
  {
  const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                      0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                     -0.577350269189626,  // -1.0 + 2.0 * C.x
                      0.024390243902439); // 1.0 / 41.0
// First corner
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);

// Other corners
  vec2 i1;
  //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
  //i1.y = 1.0 - i1.x;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  // x0 = x0 - 0.0 + 0.0 * C.xx ;
  // x1 = x0 - i1 + 1.0 * C.xx ;
  // x2 = x0 - 1.0 + 2.0 * C.xx ;
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;

// Permutations
  i = mod289(i); // Avoid truncation effects in permutation
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
        + i.x + vec3(0.0, i1.x, 1.0 ));

  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;

// Gradients: 41 points uniformly over a line, mapped onto a diamond.
// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;

// Normalise gradients implicitly by scaling m
// Approximation of: m *= inversesqrt( a0*a0 + h*h );
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

// Compute final noise value at P
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}

//==============================================================================
// Noise function, applying multiple octaves with various frequencies and
// amplitudes.

float noise(vec2 position) {
    float noise = 0;
    float frequency, amplitude;

    // Apply 3 octaves. The first one applies large spots, with high
    // intensity. The following ones fill in smaller details.
    for (int i = 0; i < 3; i++) {
        frequency = 0.01 * i;
        amplitude = 3.4 / i;

        noise += snoise(position.xy * frequency) * amplitude;
    }

    return noise * 11; // We multiply the noise for a more pronounced effect.
}


//==============================================================================

const int max_lights = 50;

uniform int light_count;
uniform int light_type;
uniform vec3 spotlight_direction;
uniform vec3 light_positions[max_lights];
uniform vec4 light_colors[max_lights];
uniform float light_sizes[max_lights];
uniform float light_inner_angles[max_lights];
uniform float light_outer_angles[max_lights];

uniform vec3 sun_position;
uniform vec4 sun_color;
uniform float sun_size;
uniform bool draw_sun;
uniform sampler2D sun_texture;

uniform int material_shininess;
uniform vec4 material_ke;
uniform vec4 material_ka;
uniform vec4 material_kd;
uniform vec4 material_ks;

uniform bool lights_on;

uniform vec4 ambiental_light;
uniform vec4 background_color;

// Compute attenuation based on distance and light size.
// More information: http://imdoingitwrong.wordpress.com/2011/01/31/light-attenuation/
float computeAttenuation(float distance, float light_size) {
    float size = light_size * 10.0;

    return 1.0 / (
        1.0 +
        (2.0 / size) * distance +
        (1.0 / (size * size)) * distance * distance);
}

// Compute the light for a vertex, coming from a single light source.
vec4 computeLight(vec3 position, vec3 V, vec4 color, float inner_angle,
    float outer_angle, float light_size) {

    // If lights are disabled, we no longer have to compute this.
    if (lights_on) {
        // Compute light direction.
        vec3 L = position - world_pos;
        vec3 Ln = normalize(L);
        float spot_falloff = 1.0;

        // Compute angle between spotlight direction and light direction.
        float current_angle = dot(-Ln, spotlight_direction);

        // If the light is spotlight, we also compute the falloff.
        if (light_type == 1) {
            float difference_angle = inner_angle - outer_angle;
            spot_falloff = clamp((current_angle - outer_angle) /
                difference_angle, 0.0, 1.0);
        }

        // If our vertex is inside the spotlight or the light is omni.
        if ((light_type == 1 && current_angle > outer_angle) ||
            light_type == 0) {

            vec3 H = normalize(Ln + V);

            float dist = length(L);
            // Compute attenuation.
            float attenuation = computeAttenuation(dist, light_size);

            vec4 diffuseLight, specularLight;

            // Calculate the diffuse component.
            diffuseLight = material_kd * color * max(dot(world_normal, Ln), 0);

            // Compute the specular component and return attenuated and reduced
            // color (reduced only if spotlight).
            if (diffuseLight.x > 0 || diffuseLight.y > 0 || diffuseLight.z > 0) {
                specularLight = (material_ks * color *
                    pow(max(dot(world_normal, H), 0), material_shininess));

                return spot_falloff * attenuation * (diffuseLight + specularLight);
            }
        }
    }

    return vec4(0, 0, 0, 0);
}

//==============================================================================

uniform vec4 fog_color;
uniform float fog_start;
uniform float fog_end;
uniform bool fog_switch;

// Compute the current fog value.
float fog(float dist) {
    if (fog_switch)
        return clamp(1 - (dist - fog_start) / (fog_end - fog_start), 0, 1);
    return 1;
}

//==============================================================================

uniform bool draw_mountain;
uniform vec4 color_top;
uniform vec2 boundary_top;
uniform vec4 color_bottom;
uniform vec2 boundary_bottom;
uniform float height_offset;

// Compute current mountain color.
vec4 computeMountainColor(vec3 position, vec4 original_color) {
    // Retrieve the current height and introduce the noise component.
    float height = (position.y - height_offset) - noise(position.xz);

    // Calculate the percentage for the top color.
    float top_percentage = clamp(1 - (height - boundary_top.x) /
        (boundary_top.y-boundary_top.x), 0, 1);

    // Calculate the percentage for the bottom color.
    float bottom_percentage = clamp(1 - (height - boundary_bottom.x) /
        (boundary_bottom.y-boundary_bottom.x), 0, 1);

    // Mix colors.
    vec4 color = mix(original_color, color_bottom, bottom_percentage);
    return mix(color_top, color, top_percentage);
}

//==============================================================================

void main(){

    int i;
    vec3 V = eye_position - world_pos;
    vec4 color = background_color;

    // If we're drawing the sun, apply texture.
    if (draw_sun) {
        vec3 tex = texture(sun_texture, texcoord).xyz;
        out_color = vec4(tex, 1);
    } else {
        // Compute distance in XZ plane only.
        float dist = distance(eye_position.xz, world_pos.xz);

        V = normalize(V);

        // If we're not drawing fog or the vertex is within fog radius.
        if (!fog_switch || dist <= fog_end) {

            // Compute emisive and ambiental components.
            color = material_ke + material_ka * ambiental_light;

            // Compute basic object color with lighting from the sun.
            color += computeLight(sun_position, V, sun_color, 0, 0, sun_size);

            // If we're drawing mountains, combine the color.
            if (draw_mountain) {
                color = computeMountainColor(world_pos, color);
            }

            // Compute the color, considering every other light in the scene.
            for (i = 0; i < light_count; i++) {
                color += computeLight(light_positions[i], V, light_colors[i],
                    light_inner_angles[i], light_outer_angles[i],
                    light_sizes[i]);
            }

            // If we're drawing fog and the vertex is inside fog falloff.
            if (fog_switch && dist >= fog_start) {
                // Compute fog value and mix fog values. The fog color is also
                // mixed with background color, to achieve a transition between
                // the sky and the fog.
                float original_color = fog(dist);
                vec4 fog_color = mix(background_color, fog_color, original_color);
                color = mix(fog_color, color, original_color);
            }
        }

        out_color = color;
    }
}