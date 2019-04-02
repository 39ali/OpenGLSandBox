#pragma once
#include <glad\glad.h>
#include <cinttypes>
#include <assert.h>
class ShadowMapPointFbo {

public:

	ShadowMapPointFbo() {}
	~ShadowMapPointFbo() {
		glDeleteFramebuffers(1, &m_Fbo);
		glDeleteTextures(1, &m_ShadowMap);
		glDeleteTextures(1 ,& m_Depth);
	
	}

	void Init(unsigned int width, unsigned int height) {
		glGenFramebuffers(1, &m_Fbo);

		//create the depth buffer
		glGenTextures(1, &m_Depth);
		glBindTexture(GL_TEXTURE_2D, m_Depth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);


		//create the cubemap

		glGenTextures(0, &m_ShadowMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ShadowMap);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		for (uint32_t i = 0; i < 6; i++)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, NULL);

		glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_Depth, 0);

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			assert(0);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	void BindForWriting(GLenum face) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, face, m_ShadowMap, 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
	}

	void BindForReading(GLenum texunit){
		glActiveTexture(texunit);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ShadowMap);
	
	}
private :
GLuint	m_Fbo;
GLuint m_ShadowMap;
GLuint m_Depth;

};