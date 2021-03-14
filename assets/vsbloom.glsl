#version 400
/*
Author: Nelson Morais
This file is part of SpaceTrials ComputerGrafik


*/

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;


void main()
{
FragColor = vec4(lighting, 1.0);

}