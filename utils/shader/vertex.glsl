#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords; //* Pass texture coordinates to fragment shader
out vec3 FragPos;   //* Pass world-space position to fragment shader
out vec3 Normal;    //* Pass normal vector to fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords; //* Pass through texture coordinates
    FragPos = vec3(model * vec4(aPos, 1.0)); //* Calculate world-space position
    Normal = mat3(transpose(inverse(model))) * aNormal; //* Transform normal to world space
    gl_Position = projection * view * vec4(FragPos, 1.0); //* Standard MVP transform
}
