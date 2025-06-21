#version 330 core
out vec4 FragColor;

in vec2 TexCoords; //* Texture coordinates from vertex shader

uniform sampler2D diffuse; //* Texture sampler

void main()
{
    FragColor = texture(diffuse, TexCoords); //* Output only the texture color
}
