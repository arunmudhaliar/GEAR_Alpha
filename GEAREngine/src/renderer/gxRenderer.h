#ifndef GXRENDERER_H
#define GXRENDERER_H

#include "../core/matrix4x4f.h"
#include "../core/rect.h"
#include "../core/TextureManager.h"

class DECLSPEC gxRenderer
{
public:
	enum RENDER_PASS_TYPE
	{
		RENDER_NORMAL,
		RENDER_LIGHTING_ONLY,
		RENDER_SHADOWMAP
	};

	gxRenderer();
	~gxRenderer();

	void setViewPort(float x, float y, float cx, float cy);

	void setProjectionMatrixToGL(matrix4x4f* matrix);
	void setViewMatrixToGL(matrix4x4f* matrix);
	void setViewProjectionMatrix(matrix4x4f* matrix);

	const vector2f& getViewPortSz()			{	return m_cViewPortRect.m_size;			}
	const gxRectf& getViewPortRect()		{	return m_cViewPortRect;					}

	matrix4x4f* getProjectionMatrix()		{	return m_pProjectionMatrixPtr;			}
	matrix4x4f* getViewMatrix()				{	return m_pViewMatrixPtr;				}
	matrix4x4f* getViewProjectionMatrix()	{	return m_pViewProjectionMatrixPtr;		}
	matrix4x4f* getOrthoProjectionMatrix()	{	return &m_cOrthogonalProjectionMatrix;	}

	void setGEARTexture1x1(stTexturePacket* tex)	{	m_pGEARTexture1x1Ptr=tex;	}
	stTexturePacket* getGEARTexture1x1()			{	return m_pGEARTexture1x1Ptr;		}

	unsigned int m_nTrisRendered;
	unsigned int m_nDrawCalls;

	void setRenderPassType(RENDER_PASS_TYPE eType)	{	m_eRenderPassType=eType;	}
	RENDER_PASS_TYPE getRenderPassType()			{	return m_eRenderPassType;	}

	void setMainCameraEye(vector3f eye)		{	m_cMainCameraEyePos=eye;	}
	vector3f getMainCameraEye()				{	return m_cMainCameraEyePos;	}

private:

	gxRectf	m_cViewPortRect;
	matrix4x4f* m_pProjectionMatrixPtr;			//must not delete this pointer
	matrix4x4f* m_pViewMatrixPtr;				//must not delete this pointer
	matrix4x4f* m_pViewProjectionMatrixPtr;     //must not delete this pointer
    matrix4x4f  m_cOrthogonalProjectionMatrix;

	stTexturePacket* m_pGEARTexture1x1Ptr;		//must not delete this pointer
	RENDER_PASS_TYPE m_eRenderPassType;
	vector3f m_cMainCameraEyePos;
};

#endif