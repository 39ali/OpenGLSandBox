#version 400
in  vec4 FragPos;

uniform vec3 LightWorldPos;
uniform float far_plane;

void main(){

float lightDistance = length(FragPos.xyz - LightWorldPos);

lightDistance = lightDistance/far_plane;

gl_FragDepth= lightDistance;
}