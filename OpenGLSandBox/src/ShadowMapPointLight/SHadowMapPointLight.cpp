#include "ShadowMapPointLight.h"

ShadowMapPointLight::ShadowMapPointLight()
    : Application(800, 600, "ShadowMapDemo") {

  m_ShadowShader = new Shader{"../../rec/shaders/pointLightShadowMap/ShadowMap.vs",
							 "../../rec/shaders/pointLightShadowMap/ShadowMap.fs" };
  m_LightingShader = new Shader{ "../../rec/shaders/pointLightShadowMap/Lighting.vs",
							 "../../rec/shaders/pointLightShadowMap/Lighting.fs" };

  m_ShadowShader->Use();
  m_ShadowMapFbo.Init(1024, 1024);

  m_QuadMesh.LoadMesh("../../rec/models/quad.obj");

  m_ModelMesh.LoadMesh("../../rec/models/bike.fbx");

  m_Sphere.LoadMesh("../../rec/models/sphere.obj");

  m_cement = TextureFactory::CreateTexture("../../rec/textures/wood.jpg");

  m_PointLight.AmbientIntensity = 0.1f;
  m_PointLight.DiffuseIntensity = 0.9f;
  m_PointLight.Color = {1.0f, 1.f, 1.f};
  m_PointLight.Attenuation.Linear = 0.0f;
  m_PointLight.Position = {5.0f, 12.0f, 0.0f};
}

ShadowMapPointLight::~ShadowMapPointLight() {
  delete m_ShadowShader;
  delete m_LightingShader;
}
void ShadowMapPointLight::Init() {

  m_LightingShader->Use();
  GLuint uloc =
      glGetUniformLocation(m_LightingShader->GetProgram(), "Pointlight.Color");
  CheckUniform(uloc);
  glUniform3fv(uloc, 1, &m_PointLight.Color);
  
  uloc = glGetUniformLocation(m_LightingShader->GetProgram(),
                              "Pointlight.AmbientIntensity");
  CheckUniform(uloc);
  glUniform1f(uloc, m_PointLight.AmbientIntensity);

  uloc = glGetUniformLocation(m_LightingShader->GetProgram(),
                              "Pointlight.DiffuseIntensity");
  CheckUniform(uloc);
  glUniform1f(uloc, m_PointLight.DiffuseIntensity);

  uloc = glGetUniformLocation(m_LightingShader->GetProgram(),
                              "Pointlight.Constant");
  CheckUniform(uloc);
  glUniform1f(uloc, m_PointLight.Attenuation.Constant);

  uloc =
      glGetUniformLocation(m_LightingShader->GetProgram(), "Pointlight.Linear");
  CheckUniform(uloc);
  glUniform1f(uloc, m_PointLight.Attenuation.Linear);

  uloc = glGetUniformLocation(m_LightingShader->GetProgram(), "Pointlight.Exp");
  CheckUniform(uloc);
  glUniform1f(uloc, m_PointLight.Attenuation.Exp);

  uloc = glGetUniformLocation(m_LightingShader->GetProgram(),
                              "Pointlight.Position");
  CheckUniform(uloc);
  glUniform3fv(uloc, 1, &m_PointLight.Position);

  uloc = glGetUniformLocation(m_LightingShader->GetProgram(), "ColorMap");
  CheckUniform(uloc);
  glUniform1i(uloc, 0);

  uloc = glGetUniformLocation(m_LightingShader->GetProgram(), "ShadowMap");
  CheckUniform(uloc);
  glUniform1i(uloc, 1);

  uloc = glGetUniformLocation(m_LightingShader->GetProgram(),
                              "MatSpecularIntensity");
  CheckUniform(uloc);
  glUniform1f(uloc, 1.0f);

  uloc = glGetUniformLocation(m_LightingShader->GetProgram(), "SpecularPower");
  CheckUniform(uloc);
  glUniform1f(uloc, 32.0f);
  CheckERR();
  // uniform vec3 EyeWorldPos;
}

void ShadowMapPointLight::Update() {}
void ShadowMapPointLight::Render() {
	ShadowPass();  
	RenderPass();
};
void ShadowMapPointLight::RenderScene(Transform &t, bool isShadowPass) {


  m_LightingShader->Use();
  t.SetScale({0.05f});
  t.SetTranslate({0, 3, 0});
  t.SetRotate({-90, 0, 0});
  
  GLuint uloc, uloc1;
  if (!isShadowPass) {
	  uloc = glGetUniformLocation(m_LightingShader->GetProgram(), "MVP");
	  CheckUniform(uloc);

	  uloc1 = glGetUniformLocation(m_LightingShader->GetProgram(), "World");
	  CheckUniform(uloc1);
	  m_LightingShader->Validate();
  }
  else {
  
	  uloc = glGetUniformLocation(m_ShadowShader->GetProgram(), "MVP");
	  CheckUniform(uloc);

	  uloc1 = glGetUniformLocation(m_ShadowShader->GetProgram(), "World");
	  CheckUniform(uloc1);
	  m_ShadowShader->Validate();
  
  }
  glUniformMatrix4fv(uloc, 1, GL_TRUE, t.GetMVPTransform());
  glUniformMatrix4fv(uloc1, 1, GL_TRUE, t.GetWorldTransform());

  
  m_ModelMesh.Render();

  //// quad
  t.SetScale({25.f});
  t.SetTranslate({0, 0, 0});
  t.SetRotate({90, 0, 0});
  glUniformMatrix4fv(uloc, 1, GL_TRUE, t.GetMVPTransform());
  glUniformMatrix4fv(uloc1, 1, GL_TRUE, t.GetWorldTransform());

  glBindTexture(GL_TEXTURE_2D, m_cement.TexID);

  m_QuadMesh.Render();
  ////
  t.SetScale({1});
  t.SetTranslate(m_PointLight.Position);

  glUniformMatrix4fv(uloc, 1, GL_TRUE, t.GetMVPTransform());
  glUniformMatrix4fv(uloc1, 1, GL_TRUE, t.GetWorldTransform());

  uloc1 = glGetUniformLocation(m_LightingShader->GetProgram(), "IsWhite");
  CheckUniform(uloc1);
  glUniform1i(uloc1, 1);

  m_Sphere.Render();
  glUniform1i(uloc1, 0);
  CheckERR();
}

void ShadowMapPointLight::ShadowPass() {

  glCullFace(GL_FRONT);
  Transform t;
  t.SetPerspectiveProj(1.0f, 100.f, m_Width, m_Height, 90.f);
   
  m_ShadowShader->Use();

  const float maxFloat = std::numeric_limits<float>::max();
  glClearColor(maxFloat, maxFloat, maxFloat, maxFloat);

  GLuint uloc = glGetUniformLocation(m_ShadowShader->GetProgram(), "LightWOrldPos");
  CheckUniform(uloc);
  glUniform3fv(uloc, 1, &m_PointLight.Position);
  CheckERR();
  for (uint32_t i = 0; i < 6; i++) {
    m_ShadowMapFbo.BindForWriting(m_Cameras[i].Face);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    t.SetCamera(m_PointLight.Position, m_Cameras[i].Target, m_Cameras[i].Up);
    RenderScene(t ,true);
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void ShadowMapPointLight::RenderPass() {

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);
 
  glClearColor(0.5, 0.5, 0.5, 0.0);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	// enable lighting shader

  m_ShadowMapFbo.BindForReading(GL_TEXTURE1);

 
  Transform t;

  t.SetPerspectiveProj(1.0f, 100.f, m_Width, m_Height, 70.f);
  t.SetCamera(m_Camera->m_Origin, m_Camera->m_Target, m_Camera->m_Up);

 RenderScene(t);
}
