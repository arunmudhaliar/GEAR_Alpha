#include "windowMessagePump.h"
#include <algorithm>

windowMessagePump& windowMessagePump::getInstance()
{
    static windowMessagePump instance;
    return instance;
}

windowMessagePump::~windowMessagePump()
{
    for(auto it : secondryWindowMessageLoops)
    {
        GE_DELETE(it.second);
    }
    secondryWindowMessageLoops.clear();
}

void windowMessagePump::appendMessagePump(SDL_Window* wnd, geSecondryView* secondryView, std::function<void(SDL_Window*, SDL_Event&, void*)>& processEventFunc)
{
    auto it = secondryWindowMessageLoops.find(wnd);
    if(it!=secondryWindowMessageLoops.end())
        return;
    
    std::function<void(SDL_Window*, SDL_Event&, void* userdata)>* f = new std::function<void(SDL_Window*, SDL_Event&, void* userdata)>();
    *f = processEventFunc;

    stWindowMessageLoop* messageLoop = new stWindowMessageLoop(secondryView, f);
    secondryWindowMessageLoops[wnd] = messageLoop;
}

void windowMessagePump::removeMessagePump(SDL_Window* wnd)
{
    auto it = secondryWindowMessageLoops.find(wnd);
    GE_DELETE(it->second);
    secondryWindowMessageLoops.erase(it);
}

std::function<void(SDL_Window*, SDL_Event&, void*)>* windowMessagePump::getMessageLoopFunction(SDL_Window* wnd)
{
    auto it = secondryWindowMessageLoops.find(wnd);
    if(it!=secondryWindowMessageLoops.end())
    {
        return it->second->messagePump;
    }
    return nullptr;
}
