#version 400 core
in  vec4 FragPos;

uniform vec3 LightWorldPos;
uniform float FarPlane;

void main(){

float lightDistance = length(FragPos.xyz - LightWorldPos);

lightDistance = lightDistance/FarPlane;

gl_FragDepth= lightDistance;
}