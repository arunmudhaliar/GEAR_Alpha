#include "rendererGL10.h"

rendererGL10::rendererGL10(SDL_Window* window):
    rendererBase(window, rendererBase::gl_fixed_pipeline)
{
}

rendererGL10::~rendererGL10()
{
}
