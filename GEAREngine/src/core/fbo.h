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

	unsigned int getFBOTextureBuffer(int index)		{	return *textures.at(index);	}
	unsigned int getFBODepthBuffer()				{	return depthBufferID;			}
	unsigned int getFBOTextureDepthShadowBuffer()	{	return depthShadowBufferID;		}
    
	int getFBOWidth()	{    return width;  }
	int getFBOHeight()	{    return height; }

private:
	//vars

	//handle to fbo
	GLuint fboID;

	//render buffer
	GLuint depthBufferID;
	GLuint depthShadowBufferID;

	int		width;
	int		height;

	std::vector<unsigned int*>	textures;

};