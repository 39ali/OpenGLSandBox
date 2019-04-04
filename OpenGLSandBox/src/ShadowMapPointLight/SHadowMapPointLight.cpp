#include "ShadowMapPointLight.h"


ShadowMapPointLight::ShadowMapPointLight():Application(800, 600, "ShadowMapDemo") {

	m_ShadowShader = new Shader{ "src/ShadowMapPointLight/Shaders/ShadowMap.vs", "src/ShadowMapPointLight/Shaders/ShadowMap.fs" };
	m_LightingShader = new Shader{ "src/ShadowMapPointLight/Shaders/Lighting.vs", "src/ShadowMapPointLight/Shaders/Lighting.fs" };
	
	m_ShadowShader->Use();
	m_ShadowMapFbo.Init(800, 600);


	m_QuadMesh.LoadMesh("rec/models/quad.obj");

	m_ModelMesh.LoadMesh("rec/models/bike.fbx");

	m_cement = TextureFactory::CreateTexture("rec/textures/wood.jpg");


	m_PointLight.AmbientIntensity = 0.1f;
	m_PointLight.DiffuseIntensity = 0.9f;
	m_PointLight.Color = { 1.0f,1.f,1.f };
	m_PointLight.Attenuation.Linear = 0.0f;
	m_PointLight.Position = { 0.0f,5.0f,0.0f };

}

ShadowMapPointLight::~ShadowMapPointLight() {
	delete m_ShadowShader;
	delete m_LightingShader;

}
void ShadowMapPointLight::Init() {



	
}

void ShadowMapPointLight::Update() {}
void ShadowMapPointLight::Render(){};



void ShadowMapPointLight::ShadowPass() {

	Transform t;
	t.SetPerspectiveProj(1.0f, 100.f, m_Width, m_Height, 90.f);
	glCullFace(GL_FRONT);
	m_ShadowShader->Use();
	const float maxFloat = std::numeric_limits<float>::max();
	glClearColor(maxFloat, maxFloat, maxFloat, maxFloat);
	for (uint32_t i = 0; i < 6; i++) {
		m_ShadowMapFbo.BindForWriting(m_Cameras[i].Face);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		t.SetCamera(m_PointLight.Position, m_Cameras[i].Target, m_Cameras[i].Up);

		
		

		GLuint uloc = glGetUniformLocation(m_ShadowShader->GetProgram(), "MVP");
		ERRU(uloc);
		glUniform4fv(uloc,1,t.GetMVPTransform());
		
		GLuint uloc1 = glGetUniformLocation(m_ShadowShader->GetProgram(), "World");
		ERRU(uloc1);
		glUniform4fv(uloc1, 1, t.GetWorldTransform());
		
		m_ModelMesh.Render();

	}



}
void ShadowMapPointLight::RenderPass() {
	
	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.5, 0.5, 0.5, 0.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	// enable lighting shader

	m_ShadowMapFbo.BindForReading(GL_TEXTURE1);
	GLuint uloc = glGetUniformLocation(m_ShadowShader->GetProgram(), "CameraPos");
	ERRU(uloc);
	glUniform3fv(uloc, 1, &m_Camera->m_Origin);

	Transform t;

	t.SetPerspectiveProj(1.0f, 100.f, m_Width, m_Height, 90.f);
	t.SetCamera(m_Camera->m_Origin,m_Camera->m_Target, m_Camera->m_Up);

   glActiveTexture(GL_TEXTURE0);
	m_QuadMesh.Render();
	m_ModelMesh.Render();


}
