#include "rendererGL20.h"

rendererGL20::rendererGL20(SDL_Window* window):
    rendererBase(window, rendererBase::gl_programmable_pipeline)
{
}

rendererGL20::~rendererGL20()
{
}
