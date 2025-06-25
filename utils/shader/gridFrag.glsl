#version 330 core

in vec2 vUV;
out vec4 FragColor;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 camPos;

uniform float gridSpacing = 1.0;
uniform vec3 gridColor = vec3(0.3);
// backgroundColor not needed, will be transparent

float gridLine(vec2 coord, float spacing) {
    vec2 grid = abs(fract(coord / spacing - 0.5) - 0.5) / fwidth(coord / spacing);
    return 1.0 - min(min(grid.x, grid.y), 1.0);
}

void main()
{
    // Reconstruct world ray
    vec2 screenUV = vUV * 2.0 - 1.0; // [-1, 1]
    vec4 clip = vec4(screenUV, -1.0, 1.0);
    vec4 eye = inverse(projection) * clip;
    eye = vec4(eye.xy, -1.0, 0.0);
    vec3 worldDir = normalize((inverse(view) * eye).xyz);

    // Ray-plane intersection with y = 0
    float t = -camPos.y / worldDir.y;
    if (t < 0.0) discard; // avoid showing anything above horizon

    vec3 worldPos = camPos + worldDir * t;

    float line = gridLine(worldPos.xz, gridSpacing);

    // Output transparent color where no line exists
    if (line < 0.01)
        discard;

    FragColor = vec4(gridColor, line); // alpha = line intensity
}
