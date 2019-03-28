#include "ShadowMapDemo.h"
#include <assert.h>
ShadowMapDemo::ShadowMapDemo():Application(800,600,"ShadowMapDemo") {}

void ShadowMapDemo::Init() {
	
	m_ShadowShader = new Shader{ "src/ShadowDemo/shaders/ShadowMap.vs", "src/ShadowDemo/shaders/ShadowMap.fs" };
	m_ShadowShader->Use();
	m_ShadowMapFbo.Init(800, 600);
	
	
	m_QuadMesh.LoadMesh("rec/models/quad.obj");

	m_ModelMesh.LoadMesh("rec/models/phoenix_ugv.md2");
	
}



void ShadowMapDemo::ShadowPass() {
	m_ShadowShader->Use();
	static float scale=0.0f;
	scale += 0.1f;
	m_ShadowMapFbo.BindForWriting();
	glClear(GL_DEPTH_BUFFER_BIT);
	m_Transform.SetScale({0.1f});
	m_Transform.SetRotate({ 0.0f ,scale, 0.0f});
	m_Transform.SetTranslate({ 0.0f,0.0f,5.f });
	m_Transform.SetCamera(m_spotLightPos, m_spotLightDir, { 0.0f,1.0f,0.0f });
	m_Transform.SetPerspectiveProj(1.0f, 50.f, 800, 600, 60.f);
	
	glUniformMatrix4fv(
						glGetUniformLocation(m_ShadowShader->GetProgram(),"MVP")
						, 1, GL_TRUE, m_Transform.GetMVPTransform());

	m_ModelMesh.Render();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void ShadowMapDemo::RenderPass() {

 	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	GLuint tex = glGetUniformLocation(m_ShadowShader->GetProgram(), "ShadowMap");
	glUniform1i(tex,0);
	m_ShadowMapFbo.BindForReading(GL_TEXTURE0);

	m_Transform.SetScale({ 5.0f });
 	m_Transform.SetRotate({ 0.0f  });
	m_Transform.SetTranslate({ 0.0f,0.0f,10.f });
	m_Transform.SetCamera(m_Camera->m_Origin, m_Camera->m_Target,m_Camera->m_Up);
	m_Transform.SetPerspectiveProj(1.f, 50.f, 800, 600, 60.f);
	
	glUniformMatrix4fv(
		glGetUniformLocation(m_ShadowShader->GetProgram(), "MVP")
		, 1, GL_TRUE, m_Transform.GetMVPTransform());

	
	m_QuadMesh.Render();

}


void ShadowMapDemo::Update()  {
}
void ShadowMapDemo::Render() {

	ShadowPass();
	RenderPass();
}