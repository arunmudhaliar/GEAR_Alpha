#ifndef RENDERERGL20_H
#define RENDERERGL20_H

#include "renderer.h"

class rendererGL20 : public rendererBase
{
public:
#ifdef _WIN32
	rendererGL20(HWND hWnd);
#else
	rendererGL20();
#endif
	~rendererGL20();
};
#endif