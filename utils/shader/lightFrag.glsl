#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;

void main()
{
    //FragColor = vec4(1.0, 1.0, 1.0, 1.0); // set all 4 vector values to 1.0
    FragColor = vec4(objectColor, 1.0);
}