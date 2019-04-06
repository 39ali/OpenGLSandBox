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
class ShadowMapPointLight : public Application {
 public:
  ShadowMapPointLight();
  void Init() override;
  ~ShadowMapPointLight();
 private:
  virtual void Update() override;
  void Render() override;
  void RenderScene(Transform& t, bool isShadowPass = false);
  void ShadowPass();
  void RenderPass();

 private:
  GLuint m_WVPlocation;
  GLuint m_TextureLocation;
  Shader* m_ShadowShader = nullptr;
  Shader* m_LightingShader = nullptr;

  Mesh m_QuadMesh, m_ModelMesh , m_Sphere;
  ShadowMapPointFbo m_ShadowMapFbo;
  vec3 m_spotLightDir = {1.0, -1.0, 0.0f};
  vec3 m_spotLightPos = {-20.0, 20.0, 5.0f};
  Transform m_ModelTransfrom;
  Transform m_PlaneTransform;
  Texture m_cement;

  struct FaceCamera {
    GLenum Face;
    vec3 Target;
    vec3 Up;
  };

  FaceCamera m_Cameras[6] = {
      {GL_TEXTURE_CUBE_MAP_POSITIVE_X, {1.0f, 0.0f, 0.f}, {0.0f, -1.0f, 0.0f}},
      {GL_TEXTURE_CUBE_MAP_NEGATIVE_X, {-1.0f, 0.0f, 0.f}, {0.0f, -1.0f, 0.0f}},
      {GL_TEXTURE_CUBE_MAP_POSITIVE_Y, {0.0f, 1.0f, 0.f}, {0.0f, 0.0f,1.0f}},
      {GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, {0.0f, -1.0f, 0.f}, {0.0f, 0.0f,-1.0f}},
      {GL_TEXTURE_CUBE_MAP_POSITIVE_Z, {0.0f, 0.0f, 1.f}, {0.0f, -1.0f, 0.0f}},
      {GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, {0.0f, 0.0f, -1.f}, {0.0f, -1.0f, 0.0f}},

  };

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
