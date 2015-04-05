#ifndef RENDERER_H
#define RENDERER_H

#include "glincludes.h"
#include "matrix4x4f.h"
#include "../util/geVector2.h"
#include <SDL2/SDL.h>

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
	rendererBase(SDL_Window* window, ERENDERER technique);
#endif
	virtual ~rendererBase();

	bool setupRenderer(rendererBase* mainRenderer=NULL);
	void destroyRenderer();

	void setViewPort(float cx, float cy);
	void loadDefaultRenderState();

	ERENDERER getRenderingTechnique()	{	return g_eRenderingTechnique;	}

	void swapGLBuffer();

	const geVector2f& getViewPortSz()	{	return m_cViewPortSz;	}
	geMatrix4x4f* getProjectionMatrix()		{	return m_pProjectionMatrixPtr;	}
	geMatrix4x4f* getOrthoProjectionMatrix()	{	return &m_cOrthogonalProjectionMatrix;	}

	bool makeCurrent();

#ifdef _WIN32
	HGLRC getRenderingContext()	{	return m_hRC;	}
	HWND getWindowHandle()		{	return m_hWnd;	}
#endif
    
private:
	bool killGL();

#ifdef _WIN32
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
	bool m_bSecondryRenderer;
    SDL_GLContext m_pContext;
    SDL_Window* m_pWindow;
    
public:
	static ERENDERER g_eRenderingTechnique;
    static unsigned int g_nTrisRendered;
	static int g_iOGLMajorVersion;
	static int g_iOGLMinorVersion;
};
#endif