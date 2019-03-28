#pragma once
#include <glad/glad.h>
class ShadowMapFbo {
public:
	ShadowMapFbo();
	~ShadowMapFbo();
	void Init(unsigned int WindowWidth, unsigned int windowHeight);
	void BindForWriting();
	void BindForReading(GLenum TexUnit);
private:
	GLuint m_Fbo;
	GLuint m_ShadowMap;
};