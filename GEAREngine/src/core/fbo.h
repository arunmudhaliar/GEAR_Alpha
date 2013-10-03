#ifndef _FBO_H
#define _FBO_H

#include "basicIncludes.h"
#include <vector>
#include "gxDebug.h"

class DllExport FBO
{
public:
	FBO();
	~FBO();

	void ReInitFBO(int w, int h);				//init will create and automatically bind the fbo
	void ResetFBO();

	GLuint& CreateDepthBuffer();			//return the reference of the depth buffer
	GLuint& CreateTextureBuffer();			//return the reference of the texture buffer

	GLuint& AttachDepthBuffer();			//return the reference of the depth buffer
	GLuint& AttachTextureBuffer(int index);	//return the reference of the texture buffer

	void BindFBO();
	void UnBindFBO();

    void DrawFBO(int index);
    
	//vars

	//handle to fbo
	GLuint m_fbo;

	//render buffer
	GLuint m_depthbuffer;

	int		m_width;
	int		m_height;

	std::vector<unsigned int*>	m_szTexture;

};
#endif