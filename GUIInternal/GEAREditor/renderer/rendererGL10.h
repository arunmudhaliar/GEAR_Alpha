#ifndef RENDERERGL10_H
#define RENDERERGL10_H

#include "renderer.h"

class rendererGL10 : public rendererBase
{
public:
#ifdef _WIN32
	rendererGL10(HWND hWnd);
#else
	rendererGL10(SDL_Window* window);
#endif
	~rendererGL10();

	//static rendererGL10* g_pRendererGL10;
};
#endif