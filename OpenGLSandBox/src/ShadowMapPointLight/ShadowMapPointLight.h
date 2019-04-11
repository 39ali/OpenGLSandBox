#pragma once
#include <glad/glad.h>
#include "../Application.h"
#include "../Camera.h"
#include "../Mesh.h"
#include "../Shader.h"
#include "../Transform.h"
#include "../math/Matrix4f.h"
#include "ShadowMapPointFbo.h"
#include <limits> 
#include <vector>
class ShadowMapPointLight : public Application {
 public:
  ShadowMapPointLight();
  void Init() override;
  ~ShadowMapPointLight();
 private:
  virtual void Update() override;
  void Render() override;
  void RenderScene(Transform& t, Shader* shader,
	  bool isShadowPass=false);
  void ShadowPass();
  void RenderPass();
  void UpdateShadowMatrices();
  void RenderSkyBox();
 private:
  GLuint m_WVPlocation;
  GLuint m_TextureLocation;
  Shader* m_ShadowShader = nullptr;
  Shader* m_LightingShader = nullptr;
  Shader* m_SkyShader = nullptr;
  Mesh m_QuadMesh, m_ModelMesh , m_Sphere,m_SkySphere, m_Building;
  ShadowMapPointFbo m_ShadowMapFbo;
  vec3 m_spotLightDir = {1.0, -1.0, 0.0f};
  vec3 m_spotLightPos = {-20.0, 20.0, 5.0f};
  Transform m_ModelTransfrom;
  Transform m_PlaneTransform;
  Texture m_cement;
  Texture m_SkyBox;
  Texture m_TreeTexture;
  std::vector<mat4>m_ShadowTransforms;

  

  struct PointLight {
	  float AmbientIntensity;
	  float DiffuseIntensity;
	  vec3 Color;
	  vec3 Position;
	  struct
	  {
		  float Constant =1.f;
		  float Linear=0.0f;
		  float Exp=0.0f;
	  } Attenuation;

  } m_PointLight;
};
