#pragma once
#include "../../../GEAREngine/src/GEARFirstInclude.h"

#ifdef GEAR_WINDOWS
#include <SDL.h>
#elif defined(GEAR_APPLE)
#include <SDL2/SDL.h>
#else
#error SDL not included
#endif

#include "../../../GUIInternal/GEAREditor/EditorApp.h"
#include <map>

struct stWindowMessageLoop
{
    stWindowMessageLoop(geSecondryView* view, std::function<void(SDL_Window*, SDL_Event&, void*)>* func)
    {
        secondryView = view;
        messagePump = func;
    }
    
    ~stWindowMessageLoop()
    {
        GE_DELETE(messagePump);
    }
    geSecondryView* secondryView;
    std::function<void(SDL_Window*, SDL_Event&, void*)>* messagePump;
};

class windowMessagePump
{
public:
    static windowMessagePump& getInstance();
    ~windowMessagePump();
    
    void appendMessagePump(SDL_Window* wnd, geSecondryView* secondryView, std::function<void(SDL_Window*, SDL_Event&, void*)>& processEventFunc);
    void removeMessagePump(SDL_Window* wnd);
    
    std::function<void(SDL_Window*, SDL_Event&, void*)>* getMessageLoopFunction(SDL_Window* wnd);
    
    std::map<SDL_Window*, stWindowMessageLoop*>* getWindowMessageLoops()    {   return &secondryWindowMessageLoops;  }
private:
    windowMessagePump(){};
    std::map<SDL_Window*, stWindowMessageLoop*> secondryWindowMessageLoops;
};
