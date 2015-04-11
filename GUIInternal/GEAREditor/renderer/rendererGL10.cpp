#include "rendererGL10.h"

//rendererGL10* rendererGL10::g_pRendererGL10=NULL;

#if DEPRECATED
rendererGL10::rendererGL10(HWND hWnd):rendererBase(hWnd, rendererBase::gl_fixed_pipeline)
#else
rendererGL10::rendererGL10(SDL_Window* window):rendererBase(window, rendererBase::gl_fixed_pipeline)
#endif
{
}

rendererGL10::~rendererGL10()
{
}
