#pragma once

#include "basicIncludes.h"
#include <vector>
#include "gxDebug.h"

class DECLSPEC FBO
{
public:
	FBO();
	~FBO();

	void ReInitFBO(int w, int h);				//init will create and automatically bind the fbo
	void ResetFBO();

    GLuint& CreateDepthOnlyBuffer();		//return the reference of the depth buffer
	GLuint& CreateDepthBuffer();			//return the reference of the depth buffer
	GLuint& CreateTextureBuffer();			//return the reference of the texture buffer
	GLuint& CreateDepthShadowTextureBuffer();

	GLuint& AttachDepthBuffer();			//return the reference of the depth buffer
	GLuint& AttachTextureBuffer(int index);	//return the reference of the texture buffer
	GLuint& AttachShadowTextureBuffer();	//return the reference of the texture buffer

	void BindFBO(int flag=0);		//0-default, 1-read
	void UnBindFBO();

    void DrawFBO(int index);

	unsigned int getFBOTextureBuffer(int index)		{	return *m_szTexture.at(index);	}
	unsigned int getFBODepthBuffer()				{	return m_depthbuffer;			}
	unsigned int getFBOTextureDepthShadowBuffer()	{	return m_depthShadowbuffer;		}
    
	int getFBOWidth()	{    return m_width;  }
	int getFBOHeight()	{    return m_height; }

private:
	//vars

	//handle to fbo
	GLuint m_fbo;

	//render buffer
	GLuint m_depthbuffer;
	GLuint m_depthShadowbuffer;

	int		m_width;
	int		m_height;

	std::vector<unsigned int*>	m_szTexture;

};