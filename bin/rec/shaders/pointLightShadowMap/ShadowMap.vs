#version 400 

layout(location=0) in vec3 Pos; 
layout(location=1) in vec2 TexCoord;
layout(location=2) in vec3 Normal;

uniform mat4 Model;



void main ()
{
gl_Position = Model*vec4(Pos,1.0);
}