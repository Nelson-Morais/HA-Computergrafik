#version 400
uniform vec3 Color;

in vec3 Position;
in vec3 Normal;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
layout (location = 2) out vec4 DepthColor;

void main()
{
    FragColor = vec4(Color.rgb,1);
    DepthColor = vec4(1,1,1,0);
};