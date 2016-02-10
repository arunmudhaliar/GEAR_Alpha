#pragma once

#include "renderer.h"

class rendererGL10 : public rendererBase
{
public:
#if DEPRECATED
	rendererGL10(HWND hWnd);
#else
	rendererGL10(SDL_Window* window);
#endif
	~rendererGL10();
};