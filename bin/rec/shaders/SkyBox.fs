#version 400 core 

in vec3 TexCoordO;
out vec4 Color ;
uniform samplerCube cubeMapTexture;
void main(){

Color = texture(cubeMapTexture,TexCoordO);

}