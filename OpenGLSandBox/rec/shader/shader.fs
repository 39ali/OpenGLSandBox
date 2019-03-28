#version 400
layout(location=0) out vec4 Fragcolor;

uniform sampler2D sampler;

in vec2 texCoordO;
void main(){

Fragcolor = texture2D(sampler,texCoordO.xy);
}