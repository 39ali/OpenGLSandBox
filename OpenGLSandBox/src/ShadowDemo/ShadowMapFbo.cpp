#include "ShadowMapFbo.h"
#include <assert.h>
#include <cstdio>
ShadowMapFbo::ShadowMapFbo() {

}
ShadowMapFbo::~ShadowMapFbo() {
	glDeleteFramebuffers(1, &m_Fbo);
	glDeleteTextures(1, &m_ShadowMap);


}
void ShadowMapFbo::Init(unsigned int WindowWidth, unsigned int windowHeight) 
{
	glGenFramebuffers(1, &m_Fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);

	glGenRenderbuffers(1, &m_Rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_Fbo);

	glGenTextures(1, &m_ShadowMap);
	glBindTexture(GL_TEXTURE_2D, m_ShadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WindowWidth, windowHeight,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	



	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_ShadowMap, 0);

	//disable color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("FrameBuffer Error! ");
		assert(0);
	}


}

void ShadowMapFbo::BindForWriting() {

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Fbo);
	glClear(GL_DEPTH_BUFFER_BIT);
}
void ShadowMapFbo::BindForReading(GLenum TexUnit){
	glActiveTexture(TexUnit);
	glBindTexture(GL_TEXTURE_2D, m_ShadowMap);
}
