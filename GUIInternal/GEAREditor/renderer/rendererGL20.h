#pragma once

#include "renderer.h"

class rendererGL20 : public rendererBase
{
public:
	rendererGL20(SDL_Window* window);
	~rendererGL20();
};