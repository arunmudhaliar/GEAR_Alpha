#pragma once

#include "renderer.h"

class rendererGL10 : public rendererBase
{
public:
	rendererGL10(SDL_Window* window);
	~rendererGL10();
};