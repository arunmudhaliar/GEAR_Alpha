#include "cursorUtil.h"

SDL_Cursor* cursorUtil::g_cszCursors[5];

void cursorUtil::init()
{
    g_cszCursors[0] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    g_cszCursors[1] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
    g_cszCursors[2] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
    g_cszCursors[3] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
	g_cszCursors[4] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
}

SDL_Cursor* cursorUtil::getCursor(int index)
{
    return g_cszCursors[index];
}
    
void cursorUtil::changeCursor(int index)
{
    SDL_SetCursor(g_cszCursors[index]);
}
