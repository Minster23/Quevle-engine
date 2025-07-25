#version 330 core
out vec4 FragColor;

in vec2 TexCoords; // Passed from vertex shader

uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1, TexCoords);
}
