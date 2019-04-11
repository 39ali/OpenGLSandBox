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
		//glDeleteTextures(1 ,& m_Depth);
	
	}

	void Init(unsigned int width, unsigned int height) {
		m_ShadowWidth = width;
		m_ShadowHeight = height;
		
		//CheckERR();
		////create the depth buffer
		//glGenTextures(1, &m_Depth);
		//glBindTexture(GL_TEXTURE_2D, m_Depth);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//glBindTexture(GL_TEXTURE_2D, 0);

		//CheckERR();

		glGenFramebuffers(1, &m_Fbo);
		//create the cubemap
		
	    glGenTextures(1, &m_ShadowMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ShadowMap);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		;
	
		CheckERR();
		for (uint32_t i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,GL_DEPTH_COMPONENT,  width, height,
				0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			CheckERR();
		}


		
		glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,m_ShadowMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		CheckERR();
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			assert(0);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		CheckERR();
	}

	void BindForWriting() {
		glViewport(0, 0, m_ShadowWidth, m_ShadowHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		CheckERR();

	}

	void BindForReading(GLenum texunit , unsigned int srcWidth , unsigned int srcHeight){
		
		glViewport(0, 0, srcWidth, srcHeight);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(texunit);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ShadowMap);
		CheckERR();

	}

	vec2 GetShadowMapSize()const { return {(float)m_ShadowWidth,(float)m_ShadowHeight}; }

private :
GLuint	m_Fbo;
GLuint m_ShadowMap;

unsigned int m_ShadowWidth, m_ShadowHeight;
};