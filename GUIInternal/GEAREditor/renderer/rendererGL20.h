#ifndef RENDERERGL20_H
#define RENDERERGL20_H

#include "renderer.h"

class rendererGL20 : public rendererBase
{
public:
#if DEPRECATED
	rendererGL20(HWND hWnd);
#else
	rendererGL20(SDL_Window* window);
#endif
	~rendererGL20();
};
#endif