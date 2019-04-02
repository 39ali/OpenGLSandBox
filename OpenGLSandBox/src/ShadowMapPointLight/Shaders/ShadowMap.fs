#version 400

in vec3 WorldPos;

uniform vec3 LightWOrldPos;

out float Color ;

void main(){

Color = length(WorldPos-LightWOrldPos);

}