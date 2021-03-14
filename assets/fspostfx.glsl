#version 400
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D MainFrame;
uniform sampler2D BloomFrame;
uniform sampler2D GodRaysFrame;


void main()
{
	vec3 mainColor = texture(MainFrame, TexCoords).rgb;      
    vec3 bloomColor = texture(BloomFrame, TexCoords).rgb;
	vec3 rayscolor = texture(GodRaysFrame, TexCoords).rgb;
   
   mainColor += bloomColor + rayscolor; //add Bloom & god rays
   FragColor = vec4(mainColor , 1.0);
}