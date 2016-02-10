#pragma once

#include "glincludes.h"
#include "matrix4x4f.h"
#include "../util/geVector2.h"
#ifdef _WIN32
	#include <SDL.h>
#elif defined(__APPLE__)
	#include <SDL2/SDL.h>
#else
	#error SDL not included
#endif

class rendererBase
{
public:

	enum ERENDERER
	{
		gl_fixed_pipeline,
		gl_programmable_pipeline
	};

	rendererBase(SDL_Window* window, ERENDERER technique);
	virtual ~rendererBase();

	bool setupRenderer(rendererBase* mainRenderer=NULL);
	void destroyRenderer();

	void setViewPort(float cx, float cy);
	void loadDefaultRenderState();

	ERENDERER getRenderingTechnique()           {	return g_eRenderingTechnique;       }
	const geVector2f& getViewPortSz()           {	return viewPortSize;                }
	geMatrix4x4f* getProjectionMatrix()         {	return projectionMatrix;            }
	geMatrix4x4f* getOrthoProjectionMatrix()	{	return &orthogonalProjectionMatrix;	}

    void swapGLBuffer();
	bool makeCurrent();
    SDL_Window* getWindow()                     {   return sdlWindow;   }
    
private:
	bool killGL();

	geVector2f	viewPortSize;
	geMatrix4x4f* projectionMatrix;             //must not delete this pointer
	//geMatrix4x4f* viewMatrix;                 //must not delete this pointer
	//geMatrix4x4f* viewProjectionMatrix;       //must not delete this pointer
    geMatrix4x4f  orthogonalProjectionMatrix;
	bool isSecondryRenderer;
    
    SDL_GLContext sdlGLContext;
    SDL_Window* sdlWindow;
    
public:
	static ERENDERER g_eRenderingTechnique;
    static unsigned int g_nTrisRendered;
	static int g_iOGLMajorVersion;
	static int g_iOGLMinorVersion;
};