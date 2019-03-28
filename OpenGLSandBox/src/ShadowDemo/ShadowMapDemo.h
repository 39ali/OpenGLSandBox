#pragma once
#include "../math/Matrix4f.h"
#include <glad/glad.h>
#include "../Shader.h"
#include "../Mesh.h"
#include "../Camera.h"
#include "ShadowMapFbo.h"
class ShadowMapDemo {

public :
	ShadowMapDemo();
	void Init();
	void Update();
	void Render();
public :
	void ShadowPass();
	void RenderPass();
private:
	GLuint m_WVPlocation;
	GLuint m_TextureLocation;
	Shader m_ShadowShader = { "src/ShadowDemo/ShadowMap.vs", "src/ShadowDemo/ShadowMap.fs" };
	Camera * m_Camera;
	Mesh  m_QuadMesh , m_ModelMesh;
	ShadowMapFbo m_ShadowMapFbo;

};