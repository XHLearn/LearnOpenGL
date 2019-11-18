#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
    // linearly interpolate between both textures (80% container, 20% awesomeface)
    vec2 reface = vec2(1-TexCoord.x, TexCoord.y);
    FragColor = mix(texture(ourTexture1,TexCoord),texture(ourTexture2,reface),0.2);
}
