#version 400 

layout(location=0) in vec3 Pos; 
layout(location=1) in vec2 TexCoord;
layout(location=2) in vec3 Normal;

uniform mat4 MVP;
uniform mat4 World;

out vec3 WorldPos;

void main (){
vec4 p = vec4(Pos,1.0);
gl_Position = MVP*p;
WorldPos =(World*p).xyz;

}