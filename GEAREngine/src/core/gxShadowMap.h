#ifndef GXSHADOWMAP_H
#define GXSHADOWMAP_H

#include "fbo.h"
#include "matrix4x4f.h"

#define GX_SHADOW_MAP_SIZE	1024

class DECLSPEC gxShadowMap
{
public:
	gxShadowMap();
	~gxShadowMap();
	void createShadowMap();
	void updateMatrix(vector3f& light_pos);

	matrix4x4f& getDepthMVP()		{ return m_cDepthMVP;	 }
	matrix4x4f& getDepthBiasMVP()	{ return m_cDepthBiasMVP; }
	FBO&	getFBO()				{ return m_cFBO;	 }

private:
	FBO m_cFBO;
	matrix4x4f m_cDepthMVP;
	matrix4x4f m_cBiasMatrix;
	matrix4x4f m_cDepthBiasMVP;
	matrix4x4f m_cDepthProjectionMatrix;
};
#endif