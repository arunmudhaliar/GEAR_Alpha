#include "fbo.h"

#if defined(WIN32) && 0
#define glDeleteRenderbuffers glDeleteRenderbuffersEXT
#define glGenFramebuffers glGenFramebuffersEXT
#define GL_FRAMEBUFFER GL_FRAMEBUFFER_EXT
#define glBindFramebuffer glBindFramebufferEXT
#define glDeleteFramebuffers glDeleteFramebuffersEXT
#define glGenRenderbuffers glGenRenderbuffersEXT
#define GL_RENDERBUFFER GL_RENDERBUFFER_EXT
#define glBindRenderbuffer glBindRenderbufferEXT
#define glRenderbufferStorage glRenderbufferStorageEXT
#define GL_DEPTH_ATTACHMENT GL_DEPTH_ATTACHMENT_EXT
#define glFramebufferRenderbuffer glFramebufferRenderbufferEXT
#define glCheckFramebufferStatus glCheckFramebufferStatusEXT
#define GL_FRAMEBUFFER_COMPLETE GL_FRAMEBUFFER_COMPLETE_EXT
#define GL_COLOR_ATTACHMENT0 GL_COLOR_ATTACHMENT0_EXT
#define glFramebufferTexture2D glFramebufferTexture2DEXT
#endif

FBO::FBO()
{
	m_fbo=0;
	m_depthbuffer=0;

	m_width=0;
	m_height=0;

	//m_texture=0;
}

FBO::~FBO()
{
	UnBindFBO();
	ResetFBO();
}

void FBO::ReInitFBO(int w, int h)
{
	// Setup our FBO
	if(m_fbo>0)
	{
		glDeleteRenderbuffers(1, &m_depthbuffer);
		for(int x=0;x<(int)m_szTexture.size();x++)
		{
			GLuint* t=m_szTexture.at(x);
			glDeleteTextures(1, t);

			delete t;
		}
		m_szTexture.clear();
	}
	else
	{
		glGenFramebuffers(1, &m_fbo);
	}

	BindFBO();

	m_width=w;
	m_height=h;
}

void FBO::BindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void FBO::UnBindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::ResetFBO()
{
	glDeleteFramebuffers(1, &m_fbo);
	glDeleteRenderbuffers(1, &m_depthbuffer);
	for(int x=0;x<(int)m_szTexture.size();x++)
	{
		GLuint* t=m_szTexture.at(x);
		glDeleteTextures(1, t);

		delete t;
	}

	m_szTexture.clear();
}

GLuint& FBO::CreateDepthBuffer()
{
	//GLuint depthbuffer=0;

	// Create the render buffer for depth	
	glGenRenderbuffers(1, &m_depthbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_width, m_height);
	return m_depthbuffer;
}

GLuint& FBO::CreateTextureBuffer()
{
	m_szTexture.push_back(new GLuint);
	GLuint* t=m_szTexture.at(m_szTexture.size()-1);
	// Now setup a texture to render to
	glGenTextures(1, t);
	glBindTexture(GL_TEXTURE_2D, *t);
	glActiveTexture(GL_TEXTURE0);
	//glClientActiveTexture(GL_TEXTURE0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,  m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, 0);
    
	return *t;
}

GLuint& FBO::AttachDepthBuffer()
{
	// Attach the depth render buffer to the FBO as it's depth attachment
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthbuffer);
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE)
    {
        DEBUG_PRINT("failed to make complete framebuffer object %x", status);
    }

	return m_depthbuffer;
}

GLuint& FBO::AttachTextureBuffer(int index)
{
	GLuint* t=m_szTexture.at(index);
	// And attach it to the FBO so we can render to it
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+index, GL_TEXTURE_2D, *t, 0);
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE)
    {
        DEBUG_PRINT("failed to make complete framebuffer object %x", status);
    }

	return *t;
}

void FBO::DrawFBO(int index)
{
    GLuint* t=m_szTexture.at(index);

#if defined (USE_ProgrammablePipeLine)
    return;
#else
    
    float x=10.0f;
    float y=10.0f;
    float cx=m_width-30;
    float cy=m_height-30;
    
    float cszTileBuffer[]={
        x,      y,
        x,      y+cy,
        x+cx,   y+cy,
        x+cx,   y
    };
    
    float cszTileTexBuffer[]={
        1,1,
        1,0,
        0,0,
        0,1,
    };
    
//    float cszTileTexBuffer[]={
//        0,0,
//        0,1,
//        1,1,
//        1,0
//    };
    
//    const float tex[8] =
//	{
//		(aGLmax.x)	, (aGLmin.y),
//		(aGLmin.x)	, (aGLmin.y),
//		(aGLmax.x)	, (aGLmax.y),
//		(aGLmin.x)	, (aGLmax.y),
//	};

    
 	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, cszTileBuffer);
    
    glActiveTexture(GL_TEXTURE0);
    glClientActiveTexture(GL_TEXTURE0);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, cszTileTexBuffer);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, *t);
    //glTranslatef(0, 0, 0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    //glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
#endif
}


