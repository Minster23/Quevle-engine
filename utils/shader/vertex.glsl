#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;        // Pass texture coordinates to fragment shader
out vec3 FragPos;          // World-space fragment position
out vec3 Normal;           // World-space normal
out vec4 FragPosLightSpace; // ✅ Pass fragment position in light space

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightMatrix; // ✅ lightProjection * lightView

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;
    TexCoords = aTexCoords;
    Normal = mat3(transpose(inverse(model))) * aNormal;
    FragPosLightSpace = lightMatrix * worldPos; // ✅ Transform to light space
    gl_Position = projection * view * worldPos;
}
