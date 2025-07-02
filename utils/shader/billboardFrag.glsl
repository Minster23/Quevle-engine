#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D billboardTex;

void main()
{
    FragColor = texture(billboardTex, TexCoord);
}
