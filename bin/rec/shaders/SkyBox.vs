#version 400 core

layout(location=0) in vec3 Position;

uniform mat4 MVP;
out vec3 TexCoordO;

void main(){
vec4 pos= MVP *vec4(Position,1.0);
gl_Position = pos.xyww;
TexCoordO=vec3(Position.x,-Position.y,Position.z);
}