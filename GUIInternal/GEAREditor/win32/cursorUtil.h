#ifndef CURSORUTIL_H
#define CURSORUTIL_H

#ifdef _WIN32
#include <SDL.h>
#elif defined(__APPLE__)
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
	static SDL_Cursor* g_cszCursors[4];
};

#endif