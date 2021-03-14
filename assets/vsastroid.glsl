#version 400
#define MAX_PARTICLES 255

layout(location=0) in vec4 VertexPos;
layout(location=1) in vec4 VertexNormal;
layout(location=2) in vec2 VertexTexcoord;

out vec3 Position;
out vec3 Normal;
out vec2 Texcoord;
out vec4 depth;

uniform mat4 ModelMats[MAX_PARTICLES];
uniform mat4 ModelViewProjMat;

void main()
{
    Position = (ModelMats[gl_InstanceID] * VertexPos).xyz;
    Normal = (ModelMats[gl_InstanceID] * vec4(VertexNormal.xyz,0)).xyz;
    Texcoord = VertexTexcoord;
    depth =  ModelViewProjMat * ModelMats[gl_InstanceID]* VertexPos;
    gl_Position = depth;
    
}



