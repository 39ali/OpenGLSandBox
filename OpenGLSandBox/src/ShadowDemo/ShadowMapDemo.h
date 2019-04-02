#pragma once
#include "../math/Matrix4f.h"
#include <glad/glad.h>
#include "../Shader.h"
#include "../Mesh.h"
#include "../Camera.h"
#include "../Transform.h"
#include "ShadowMapFbo.h"
#include "../Application.h"
class ShadowMapDemo :public Application {

public :
	ShadowMapDemo();
	void Init() override;

private:
	virtual void Update() override;
	void Render() override;
	void RenderModels();
	void ShadowPass();
	void RenderPass();
private:
	
	GLuint m_WVPlocation;
	GLuint m_TextureLocation;
	Shader * m_ShadowShader = nullptr;
	
	
	Mesh  m_QuadMesh , m_ModelMesh;
	ShadowMapFbo m_ShadowMapFbo;
	vec3 m_spotLightDir = { 1.0, -1.0, 0.0f };
	vec3 m_spotLightPos = { -20.0, 20.0, 5.0f };
	Transform m_ModelTransfrom;
	Transform m_PlaneTransform;
	Texture m_cement;
};