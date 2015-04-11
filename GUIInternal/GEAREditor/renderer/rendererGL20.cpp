#include "rendererGL20.h"

#if DEPRECATED
rendererGL20::rendererGL20(HWND hWnd):rendererBase(hWnd, rendererBase::gl_programmable_pipeline)
#else
rendererGL20::rendererGL20(SDL_Window* window):rendererBase(window, rendererBase::gl_programmable_pipeline)
#endif
{
}

rendererGL20::~rendererGL20()
{
}
