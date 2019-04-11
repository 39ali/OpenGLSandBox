#include "ShadowMapPointLight.h"
#define SHADOW_SIZE std::powf(2,10)
#define FarPlane  50.0f
ShadowMapPointLight::ShadowMapPointLight()
    : Application(800, 600, "Point Light Shadow Demo") {
  m_ShadowShader =
      new Shader{"../../rec/shaders/pointLightShadowMap/ShadowMap.vs",
                 "../../rec/shaders/pointLightShadowMap/ShadowMap.fs",
                 "../../rec/shaders/pointLightShadowMap/ShadowMap.gs"};
  m_LightingShader =
      new Shader{"../../rec/shaders/pointLightShadowMap/Lighting.vs",
                 "../../rec/shaders/pointLightShadowMap/Lighting.fs"};

  m_ShadowShader->Use();
  m_ShadowMapFbo.Init(SHADOW_SIZE, SHADOW_SIZE);

  m_QuadMesh.LoadMesh("../../rec/models/quad.obj");

  m_ModelMesh.LoadMesh("../../rec/models/bike.fbx");

  m_Sphere.LoadMesh("../../rec/models/sphere.obj");

  m_Building.LoadMesh("../../rec/models/tree/tree.obj");
  m_TreeTexture = TextureFactory::CreateTexture("../../rec/models/tree/tree.png");

  m_cement = TextureFactory::CreateTexture("../../rec/textures/wood.jpg");

  m_PointLight.AmbientIntensity = 0.1f;
  m_PointLight.DiffuseIntensity = 0.9f;
  m_PointLight.Color = {1.0f, 1.f, 1.f};
  m_PointLight.Attenuation.Linear = 0.0f;
  m_PointLight.Position = {5.0f, 15.0f, 0.0f};

  const char* filename[6] = {
	  "../../rec/textures/cubeMaps/morning_lf.tga",
	  "../../rec/textures/cubeMaps/morning_rt.tga",
	  "../../rec/textures/cubeMaps/morning_dn.tga",
	  "../../rec/textures/cubeMaps/morning_up.tga", 
	  "../../rec/textures/cubeMaps/morning_ft.tga",
	  "../../rec/textures/cubeMaps/morning_bk.tga",
  };
  m_SkyBox = TextureFactory::CreateTexture3D(filename);
  CheckERR();
  m_SkySphere.LoadMesh("../../rec/models/sphere.obj");
  CheckERR();
  m_SkyShader = new Shader{ "../../rec/shaders/SkyBox.vs",
	  "../../rec/shaders/SkyBox.fs" };
  CheckERR();

}

ShadowMapPointLight::~ShadowMapPointLight() {
  delete m_ShadowShader;
  delete m_LightingShader;
}
void ShadowMapPointLight::Init() {
  m_LightingShader->Use();

  m_LightingShader->SetUniform("Pointlight.Color", m_PointLight.Color);

  m_LightingShader->SetUniform("Pointlight.AmbientIntensity",
                               m_PointLight.AmbientIntensity);

  m_LightingShader->SetUniform("Pointlight.DiffuseIntensity",
                               m_PointLight.DiffuseIntensity);

  m_LightingShader->SetUniform("Pointlight.Constant",
                               m_PointLight.Attenuation.Constant);

  m_LightingShader->SetUniform("Pointlight.Linear",
                               m_PointLight.Attenuation.Linear);

  m_LightingShader->SetUniform("Pointlight.Exp", m_PointLight.Attenuation.Exp);

  m_LightingShader->SetUniform("Pointlight.Position", m_PointLight.Position);

  m_LightingShader->SetUniform("ColorMap", 0);
  m_LightingShader->SetUniform("ShadowMap", 1);
  m_LightingShader->SetUniform("MatSpecularIntensity", 1.0f);
  m_LightingShader->SetUniform("SpecularPower", 32.0f);

  CheckERR();
}

void ShadowMapPointLight::Update() {
	static vec3 origin = m_PointLight.Position;
	static float angle = 0.01f;
	static float radius = 20.f;
	angle += 0.007;
	if (glfwGetKey(m_Window, GLFW_KEY_Q) == GLFW_PRESS)
		angle += 0.01;
	m_PointLight.Position.x = origin.x+ std::cosf(angle)*radius;
	m_PointLight.Position.z = origin.z+ std::sinf(angle)*radius;
	m_LightingShader->Use();
	m_LightingShader->SetUniform("Pointlight.Position", m_PointLight.Position);
	UpdateShadowMatrices();
}

void ShadowMapPointLight::UpdateShadowMatrices() {

	mat4 shadowTransfrom, lookat;
	m_ShadowTransforms.clear();
	shadowTransfrom.makePerspectiveProjection(1.0f, FarPlane, SHADOW_SIZE, SHADOW_SIZE, 90.0f);

	//left right
	m_ShadowTransforms.push_back(
		shadowTransfrom * lookat.LookAt(m_PointLight.Position,
			m_PointLight.Position+vec3(1.0, 0.0, 0.0),
			vec3(0.0, 1.0, 0.0)));
	m_ShadowTransforms.push_back(
		shadowTransfrom * lookat.LookAt(m_PointLight.Position,
			m_PointLight.Position+vec3(-1.0, 0.0, 0.0),
			vec3(0.0, 1.0, 0.0)));

	//up down
	m_ShadowTransforms.push_back(
		shadowTransfrom * lookat.LookAt(m_PointLight.Position,
			m_PointLight.Position + vec3(0.0, -1.0, 0.0),
			vec3(0.0, 0.0, 1.0)));
	m_ShadowTransforms.push_back(shadowTransfrom *
		lookat.LookAt(m_PointLight.Position,
			m_PointLight.Position+vec3(0.0, 1.0, 0.0),
			vec3(0.0, 0.0, 1.0)));

	

	//front back

	m_ShadowTransforms.push_back(
		shadowTransfrom * lookat.LookAt(m_PointLight.Position,
			m_PointLight.Position+vec3(0.0, 0.0, 1.0),
			vec3(0.0, 1.0, 0.0)));

	m_ShadowTransforms.push_back(
		shadowTransfrom * lookat.LookAt(m_PointLight.Position,
			m_PointLight.Position + vec3(0.0, 0.0, -1.0),
			vec3(0.0, 1.0, 0.0)));
	
}
void ShadowMapPointLight::RenderSkyBox()
{
	m_SkyShader->Use();
	GLint prevCullFace;
	glGetIntegerv(GL_CULL_FACE_MODE, &prevCullFace);
	GLint prevDepthFuncMode;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFuncMode);

	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);

	Transform t; 
	t.SetScale({ 20 });
	t.SetCamera(m_Camera->m_Origin, m_Camera->m_Target, m_Camera->m_Up);
	t.SetTranslate(m_Camera->m_Origin);
	t.SetPerspectiveProj(1.0f, 1000.f, m_Width, m_Height, 70.f);
	m_SkyShader->SetUniform("MVP",t.GetMVPTransform());
	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, m_cement.TexID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_SkyBox.TexID);
	
	m_SkyShader->SetUniform("cubeMapTexture",0);
	//render
	m_SkySphere.Render();
	glCullFace(prevCullFace);
	glDepthFunc(prevDepthFuncMode);
}
void ShadowMapPointLight::Render() {
  ShadowPass();
  RenderPass();
  CheckERR();
  RenderSkyBox();
};
void ShadowMapPointLight::RenderScene(Transform& t, Shader* shader,
                                      bool isShadowPass) {
  shader->Use();
  t.SetScale({0.05f});
  t.SetTranslate({0, 0, 0});
  t.SetRotate({-90, 0, 0});

  GLuint uloc, uloc1;

  shader->SetUniform("World", t.GetWorldTransform());
  if (!isShadowPass) shader->SetUniform("MVP", t.GetMVPTransform());

  shader->Validate();

  m_ModelMesh.Render();

  //// quad
  t.SetScale({50.f});
  t.SetTranslate({0, 0, 0});
  t.SetRotate({90, 0, 0});

  if (!isShadowPass) shader->SetUniform("MVP", t.GetMVPTransform());

  shader->SetUniform("World", t.GetWorldTransform());

  glBindTexture(GL_TEXTURE_2D, m_cement.TexID);

  m_QuadMesh.Render();

  //// light
  t.SetScale({1});
  t.SetTranslate(m_PointLight.Position);

  if (!isShadowPass) shader->SetUniform("MVP", t.GetMVPTransform());

  shader->SetUniform("World", t.GetWorldTransform());

  if (!isShadowPass) {
    shader->SetUniform("IsWhite", 1);
  }
  m_Sphere.Render();

  if (!isShadowPass) shader->SetUniform("IsWhite", 0);


  // tree 
  t.SetScale({ 0.5f });
  t.SetTranslate({ -3, 0, 3 });
  t.SetRotate({ 0, 0, 0 });

  if (!isShadowPass) shader->SetUniform("MVP", t.GetMVPTransform());
  shader->SetUniform("World", t.GetWorldTransform());
  glBindTexture(GL_TEXTURE_2D, m_TreeTexture.TexID);
  m_Building.Render();

  CheckERR();
}

void ShadowMapPointLight::ShadowPass() {
  glCullFace(GL_FRONT);
  m_ShadowShader->Use();
  m_ShadowMapFbo.BindForWriting();

  m_ShadowShader->SetUniform("LightWorldPos", m_PointLight.Position);
  m_ShadowShader->SetUniform("FarPlane", FarPlane);

  for (uint32_t i = 0; i < 6; i++) {
    m_ShadowShader->SetUniform(
        ("ShadowMatrices[" + std::to_string(i) + "]").c_str(),
        m_ShadowTransforms[i]);
  }

  CheckERR();
  Transform t;
  RenderScene(t, m_ShadowShader, true);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void ShadowMapPointLight::RenderPass() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glCullFace(GL_BACK);
  glClearColor(0.5, 0.5, 0.5, 0.0);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  // enable lighting shader
  m_ShadowMapFbo.BindForReading(GL_TEXTURE1, m_Width, m_Height);
  CheckERR();
  m_LightingShader->Use();
  CheckERR();
  Transform t;
  t.SetPerspectiveProj(1.0f, 100.f, m_Width, m_Height, 70.f);
  t.SetCamera(m_Camera->m_Origin, m_Camera->m_Target,m_Camera->m_Up);



  CheckERR();
  m_LightingShader->SetUniform("FarPlane", FarPlane);
  CheckERR();
  RenderScene(t, m_LightingShader);
}
