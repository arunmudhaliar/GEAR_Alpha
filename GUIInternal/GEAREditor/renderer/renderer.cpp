#include "renderer.h"

rendererBase::ERENDERER rendererBase::g_eRenderingTechnique=gl_fixed_pipeline;
unsigned int rendererBase::g_nTrisRendered=0;
int rendererBase::g_iOGLMajorVersion=0;
int rendererBase::g_iOGLMinorVersion=0;

rendererBase::rendererBase(SDL_Window* window, ERENDERER technique)
{
    sdlWindow=window;
	isSecondryRenderer=false;
	g_eRenderingTechnique=technique;
	//projectionMatrix=NULL;
	//viewMatrix=NULL;
	//viewProjectionMatrix=NULL;
	//viewPortSize.set(1.0f, 1.0f);
}
	
rendererBase::~rendererBase()
{
	destroyRenderer();
}

bool rendererBase::setupRenderer(rendererBase* mainRenderer)
{
#if DEPRECATED
	isSecondryRenderer=(mainRenderer)?true:false;

	static	PIXELFORMATDESCRIPTOR pfd=							// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),							// Size Of This Pixel Format Descriptor
		1,														// Version Number
		PFD_DRAW_TO_WINDOW |									// Format Must Support Window
		PFD_SUPPORT_OPENGL |									// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,										// Must Support Double Buffering
		PFD_TYPE_RGBA,											// Request An RGBA Format
		24,														// Select Our Color Depth
		0, 0, 0, 0, 0, 0,										// Color Bits Ignored
		0,														// No Alpha Buffer
		0,														// Shift Bit Ignored
		0,														// No Accumulation Buffer
		0, 0, 0, 0,												// Accumulation Bits Ignored
		24,														// 16Bit Z-Buffer (Depth Buffer)  
		0,														// No Stencil Buffer
		0,														// No Auxiliary Buffer
		PFD_MAIN_PLANE,											// Main Drawing Layer
		0,														// Reserved
		0, 0, 0													// Layer Masks Ignored
	};
	
	if (!(m_hDC=GetDC(m_hWnd)))										// Did We Get A Device Context?
	{
		destroyRenderer();												// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;											// Return GX_FALSE
	}
	
	if (!(m_iPixelFormat=ChoosePixelFormat(m_hDC,&pfd)))				// Did Windows Find A Matching Pixel Format?
	{
		destroyRenderer();												// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;											// Return GX_FALSE
	}
	
	if(!SetPixelFormat(m_hDC,m_iPixelFormat,&pfd))					// Are We Able To Set The Pixel Format?
	{
		destroyRenderer();												// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;											// Return GX_FALSE
	}
	
	if(!mainRenderer)
	{
		if (!(m_hRC=wglCreateContext(m_hDC)))							// Are We Able To Get A Rendering Context?
		{
			destroyRenderer();												// Reset The Display
			MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return false;											// Return GX_FALSE
		}
	}
	else
	{
		m_hRC=mainRenderer->getRenderingContext();
	}

	
	if(!makeCurrent())
		return false;

	if(!mainRenderer)
	{
		glewInit();
		if (!GLEW_VERSION_2_0)
		{
			MessageBox(NULL, "OpenGL 2.0 not supported. Shaders won't work !!!", "ERROR", MB_OK|MB_ICONEXCLAMATION);
		}

		glGetIntegerv(GL_MAJOR_VERSION, &g_iOGLMajorVersion);
		glGetIntegerv(GL_MINOR_VERSION, &g_iOGLMinorVersion);
	}
	//glGetIntegerv​(GL_MAJOR_VERSION​, &major_version);
	//glGetIntegerv​(GL_MINOR_VERSION​, &minor_version);
#else
    isSecondryRenderer=(mainRenderer)?true:false;
    if(isSecondryRenderer)
    {
        sdlGLContext = mainRenderer->sdlGLContext;
    }
    else
    {
        sdlGLContext = SDL_GL_CreateContext( sdlWindow );
        printf("%s %s\n", glGetString(GL_RENDERER), glGetString(GL_VERSION));

#ifdef GEAR_WINDOWS
		glewInit();
		if (!GLEW_VERSION_2_0)
		{
			MessageBox(NULL, "OpenGL 2.0 not supported. Shaders won't work !!!", "ERROR", MB_OK|MB_ICONEXCLAMATION);
		}

		glGetIntegerv(GL_MAJOR_VERSION, &g_iOGLMajorVersion);
		glGetIntegerv(GL_MINOR_VERSION, &g_iOGLMinorVersion);
#endif

        if( sdlGLContext == NULL )
        {
            printf("ERROR SDL_GL_CreateContext\n");
            return false;
        }
    }
    
    if(!makeCurrent())
        return false;
#endif

	return true;
}

bool rendererBase::makeCurrent()
{
#if DEPRECATED
	if(!wglMakeCurrent(m_hDC,m_hRC))								// Try To Activate The Rendering Context
	{
		destroyRenderer();												// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;											// Return GX_FALSE
	}
    return true;
#else
    int ret=SDL_GL_MakeCurrent(sdlWindow, sdlGLContext);
    return ret==0;
#endif
}

void rendererBase::destroyRenderer()
{
	killGL();
}


bool rendererBase::killGL()
{
#if DEPRECATED
	bool flag=false;
	
	if(m_hRC && !isSecondryRenderer)													// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))						// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			flag=true;
		}
		
		if (!wglDeleteContext(m_hRC))							// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			flag=true;
		}
		m_hRC=NULL;											// Set RC To NULL
	}
	
	return flag;
#else
    if(!isSecondryRenderer)
    {
        SDL_GL_MakeCurrent(NULL, NULL);
        SDL_GL_DeleteContext(sdlGLContext);
    }
    return true;
#endif
}


void rendererBase::setViewPort(float cx, float cy)
{
    //orthogonalProjectionMatrix.setOrtho(0.0f, cx, cy, 0.0f, 0.0f, 10.0f);
	viewPortSize.set(cx, cy);
	//glViewport(0, 0, (int)cx, (int)cy);
    //glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glOrtho(0.0f, (int)cx, (int)cy, 0.0f, 0.0f, 10.0f);
    //glMatrixMode(GL_MODELVIEW);
}

void rendererBase::loadDefaultRenderState()
{
	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);			// Black Background
	//glShadeModel(GL_FLAT);							// Enable Smooth Shading
	//glClearDepth(1.0f);								// Depth Buffer Setup
	
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);								// Enable culling
	
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do (GL_LEQUAL is must for shadow fonts)
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void rendererBase::swapGLBuffer()
{
#if DEPRECATED
	SwapBuffers(m_hDC);
#else
    SDL_GL_SwapWindow(sdlWindow);
#endif
}