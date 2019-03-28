#include "ShadowMapDemo.h"

ShadowMapDemo::ShadowMapDemo() {}

void ShadowMapDemo::Init() {
	m_ShadowMapFbo.Init(800, 600);
	m_Camera = new Camera({ 0,0,-13 }, { 0,0,0 }, {0,1,0}, 800, 600);
	m_ShadowShader.Use();

	m_QuadMesh.LoadMesh("rec/models/quad.obj");

	m_ModelMesh.LoadMesh("rec/models/phoenix_ugv.md2");
}

void ShadowMapDemo::Update() {}
void ShadowMapDemo::Render() {}