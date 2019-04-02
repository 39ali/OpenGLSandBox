#version 400

out vec4 color;

in vec2 TexCoordO;
in vec3 NormalO;
in vec3 WorldPosO;
in vec4 LightSpacePos;

uniform sampler2D ShadowMap;
uniform sampler2D Sampler;

float CalcShadow(vec4 lightPos) {
  vec3 projCoords = lightPos.xyz / lightPos.w;
  vec2 uvCoords;
  uvCoords.x = 0.5 * (projCoords.x + 1.0);
  uvCoords.y = 0.5 * (projCoords.y + 1.0);
  float z = 0.5 * (projCoords.z + 1.0);

  float bias =0.0005;
  float shadow = 0.0;
  vec2 texelSize = 1.0 / textureSize(ShadowMap, 0);
  for(int x = -1; x <= 1; ++x)
{
    for(int y = -1; y <= 1; ++y)
    {
        float pcfDepth = texture(ShadowMap, uvCoords + vec2(x, y) * texelSize).r; 
        shadow += z - bias > pcfDepth ? .5 : 1.0;        
    }    
}
  shadow /= 9.0;

  return shadow;
}

void main() {
	
  vec4 sampledColor = texture(Sampler, TexCoordO);
  float shadowFactor =CalcShadow(LightSpacePos);
  color =( shadowFactor )* sampledColor  ;
}

