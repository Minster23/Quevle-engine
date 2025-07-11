#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 billboardPos;
uniform vec3 cameraPos;

void main()
{
    vec3 lookDir = normalize(cameraPos - billboardPos);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(up, lookDir));
    up = normalize(cross(lookDir, right));

    vec3 worldPos = billboardPos + right * aPos.x + up * aPos.y;

    gl_Position = projection * view * vec4(worldPos, 1.0);
    TexCoords = aTexCoords;
}
