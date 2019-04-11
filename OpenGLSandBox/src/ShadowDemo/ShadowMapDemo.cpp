#include "ShadowMapDemo.h"
#include <assert.h>
ShadowMapDemo::ShadowMapDemo() :Application(800, 600, "ShadowMapDemo") {}

void ShadowMapDemo::Init() {

	m_ShadowShader = new Shader{ "src/ShadowDemo/shaders/ShadowMap.vs", "src/ShadowDemo/shaders/ShadowMap.fs" };
	m_ShadowShader->Use();
	m_ShadowMapFbo.Init(800, 600);


	m_QuadMesh.LoadMesh("rec/models/quad.obj");

	m_ModelMesh.LoadMesh("rec/models/bike.fbx");

	m_cement = TextureFactory::CreateTexture("rec/textures/wood.jpg");

}





void ShadowMapDemo::ShadowPass() {
	glCullFace(GL_FRONT);
	m_ShadowMapFbo.BindForWriting();
	static float scale = 0.0f;
	scale += 0.1f;

	
	m_ModelTransfrom.SetScale({ 0.05f });
	m_ModelTransfrom.SetRotate({ -90.0f ,scale, 0.0f });
	m_ModelTransfrom.SetTranslate({ 0.0f,0.5f,3.f });
	m_ModelTransfrom.SetCamera(m_spotLightPos, m_spotLightDir, { 0.0f,1.0f,0.0f });
	m_ModelTransfrom.SetPerspectiveProj(1.0f, 100.f, 800, 600, 30.f);

	glUniformMatrix4fv(
		glGetUniformLocation(m_ShadowShader->GetProgram(), "MVP")
		, 1, GL_TRUE, m_ModelTransfrom.GetMVPTransform());

	m_ModelMesh.Render();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);
}

void ShadowMapDemo::RenderPass() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//bind the shadowmap texture
	m_ShadowMapFbo.BindForReading(GL_TEXTURE1);
	GLuint tex = glGetUniformLocation(m_ShadowShader->GetProgram(), "ShadowMap");
//	CheckUniform(tex);
	glUniform1i(tex, 1);

	//render all  the thingss
	Transform tr;
	tr.SetScale({ 10.0f });
	tr.SetRotate({ 90.0f,0,0 });
	tr.SetTranslate({ 0.0f,0.0f,1.f });
	tr.SetCamera(m_Camera->m_Origin, m_Camera->m_Target, m_Camera->m_Up);
	tr.SetPerspectiveProj(1.f, 100.f, 800, 600, 30.f);

	glUniformMatrix4fv(
		glGetUniformLocation(m_ShadowShader->GetProgram(), "MVP")
		, 1, GL_TRUE, tr.GetMVPTransform());

	tr.SetCamera(m_spotLightPos, m_spotLightDir, { 0.0f,1.0f,0.0f });
	glUniformMatrix4fv(
		glGetUniformLocation(m_ShadowShader->GetProgram(), "MVPLight")
		, 1, GL_TRUE, tr.GetMVPTransform());


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_cement.TexID);
	GLuint tex1 = glGetUniformLocation(m_ShadowShader->GetProgram(), "Sampler");
//	CheckUniform(tex1);
	glUniform1i(tex1, 0);

	m_QuadMesh.Render();





	static float scale = 0.0f;
	scale += 0.1f;
	m_ModelTransfrom.SetRotate({-90.0f ,scale, 0.0f });
	m_ModelTransfrom.SetCamera(m_Camera->m_Origin, m_Camera->m_Target, m_Camera->m_Up);
	glUniformMatrix4fv(
		glGetUniformLocation(m_ShadowShader->GetProgram(), "MVP")
		, 1, GL_TRUE, m_ModelTransfrom.GetMVPTransform());

	m_ModelTransfrom.SetCamera(m_spotLightPos, m_spotLightDir, { 0.0f,1.0f,0.0f });
	glUniformMatrix4fv(
		glGetUniformLocation(m_ShadowShader->GetProgram(), "MVPLight")
		, 1, GL_TRUE, m_ModelTransfrom.GetMVPTransform());




	m_ModelMesh.Render();


}


void ShadowMapDemo::Update() {
}
void ShadowMapDemo::Render() {


	ShadowPass();
	RenderPass();
}