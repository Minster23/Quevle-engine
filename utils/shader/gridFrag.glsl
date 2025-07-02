#version 330 core

in vec2 vUV;
out vec4 FragColor;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 camPos;

uniform float gridSpacing;
uniform vec3 gridColor;

float gridLine(vec2 coord, float spacing) {
    vec2 pos = coord / spacing;
    vec2 grid = abs(fract(pos - 0.5) - 0.5);
    vec2 d = fwidth(pos);
    vec2 line = grid / d;
    return 1.0 - clamp(min(line.x, line.y), 0.0, 1.0);
}

void main()
{
    // Screen to NDC
    vec2 screenUV = vUV * 2.0 - 1.0;

    // Get ray direction from screen
    mat4 invProj = inverse(projection);
    mat4 invView = inverse(view);

    vec4 eye = invProj * vec4(screenUV, -1.0, 1.0);
    vec3 rayDir = normalize((invView * vec4(eye.xy, -1.0, 0.0)).xyz);

    // Intersect with ground plane (Y = 0)
    float t = -camPos.y / rayDir.y;
    if (t <= 0.0) discard; // Behind camera

    vec3 worldPos = camPos + rayDir * t;

    // Optional hard cutoff (you can remove for true infinite look)
    if (length(worldPos.xz - camPos.xz) > 300.0) discard;

    // Parallax offset — makes grid trail camera
    float parallaxStrength = 0.15;
    vec2 parallaxCoord = worldPos.xz + camPos.xz * parallaxStrength;

    // Grid generation
    float line = gridLine(parallaxCoord, gridSpacing);
    if (line < 0.01) discard;

    // Fade with distance (soft fade at edge)
    float dist = length(worldPos.xz - camPos.xz);
    float fade = smoothstep(300.0, 200.0, dist);

    FragColor = vec4(gridColor, line * fade);
}
