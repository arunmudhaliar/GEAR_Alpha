#pragma once
#include "../../../GEAREngine/src/GEARFirstInclude.h"

#ifdef GEAR_WINDOWS
#include <SDL.h>
#elif defined(GEAR_APPLE)
#include <SDL2/SDL.h>
#else
#error SDL not included
#endif

class cursorUtil
{
public:
    static void init();
    static SDL_Cursor* getCursor(int index);
    static void changeCursor(int index);

private:
	static SDL_Cursor* g_cszCursors[5];
};