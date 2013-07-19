#ifndef RENDERER_H
#define RENDERER_H

#include "glincludes.h"
#include "matrix4x4f.h"
#include "../util/geVector2.h"

class rendererBase
{
public:

	enum ERENDERER
	{
		gl_fixed_pipeline,
		gl_programmable_pipeline
	};

#ifdef _WIN32
	rendererBase(HWND hWnd, ERENDERER technique);
#else
	rendererBase(ERENDERER technique);
#endif
	virtual ~rendererBase();

	bool setupRenderer();
	void destroyRenderer();

	void setViewPort(float cx, float cy);
	void loadDefaultRenderState();

	ERENDERER getRenderingTechnique()	{	return g_eRenderingTechnique;	}

	void swapGLBuffer();

	void setProjectionMatrixToGL(geMatrix4x4f* matrix);
	//void setViewMatrixToGL(geMatrix4x4f* matrix);
	//void setViewProjectionMatrix(geMatrix4x4f* matrix);

	const geVector2f& getViewPortSz()	{	return m_cViewPortSz;	}

	geMatrix4x4f* getProjectionMatrix()		{	return m_pProjectionMatrixPtr;	}
	//geMatrix4x4f* getViewMatrix()				{	return m_pViewMatrixPtr;	}
	//geMatrix4x4f* getViewProjectionMatrix()	{	return m_pViewProjectionMatrixPtr;	}
	geMatrix4x4f* getOrthoProjectionMatrix()	{	return &m_cOrthogonalProjectionMatrix;	}

	//void enableDirectionalLighting()		{	m_bDirectionalLighting=true;	}
	//void disableDirectionalLighting()		{	m_bDirectionalLighting=false;	}

private:

#ifdef _WIN32
	bool killGL();


	unsigned int	m_iPixelFormat;					// Holds The Results After Searching For A Match.
	HDC				m_hDC;							// Private GDI Device Context.
	HGLRC			m_hRC;							// Permanent Rendering Context.
	HWND			m_hWnd;							// Holds Our Window Handle.
#endif

	geVector2f	m_cViewPortSz;
	geMatrix4x4f* m_pProjectionMatrixPtr;			//must not delete this pointer
	//geMatrix4x4f* m_pViewMatrixPtr;				//must not delete this pointer
	//geMatrix4x4f* m_pViewProjectionMatrixPtr;     //must not delete this pointer
    geMatrix4x4f  m_cOrthogonalProjectionMatrix;
	//bool		m_bDirectionalLighting;
public:
	static ERENDERER g_eRenderingTechnique;
    static unsigned int g_nTrisRendered;
};
#endif