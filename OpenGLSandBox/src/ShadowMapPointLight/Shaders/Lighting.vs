#version 400

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCrood;
layout(location = 2) in vec3 Normal;

out vec2 TexCoordO;
out vec3 NormalO;
out vec3 WorldPos;

uniform mat4 MVP;
uniform mat4 World;

void main() {
  gl_Position = MVP * vec4(Position, 1.0);
  TexCoordO= TexCoord;
  NormalO =( World*vec4(Normal,0.0)).xyz;
  WorldPos = World*vec4(Position,1.0).xyz;
}
