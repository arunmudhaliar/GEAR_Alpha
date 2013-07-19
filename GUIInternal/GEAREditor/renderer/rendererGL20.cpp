#include "rendererGL20.h"

#ifdef _WIN32
rendererGL20::rendererGL20(HWND hWnd):rendererBase(hWnd, rendererBase::gl_programmable_pipeline)
#else
rendererGL20::rendererGL20():rendererBase(rendererBase::gl_programmable_pipeline)
#endif
{
}

rendererGL20::~rendererGL20()
{
}
