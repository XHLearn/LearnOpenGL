#version 330 core
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;


void main()
{
   FragColor = vec4(0.1f * lightColor * objectColor, 1.0f);
}
