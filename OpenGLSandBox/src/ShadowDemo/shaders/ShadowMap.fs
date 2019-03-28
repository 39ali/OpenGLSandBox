#version 400

out vec4 color;

in vec2 TexCoordO;
uniform sampler2D ShadowMap;

void main (){

float depth= texture(ShadowMap,TexCoordO).x;
//depth = 1.0-(1.0-depth)*25.0;
color =vec4(depth);
}