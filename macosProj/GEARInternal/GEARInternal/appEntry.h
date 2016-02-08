#pragma once

#ifdef _WIN32
	#include <SDL.h>
#elif defined(__APPLE__)
	#include <SDL2/SDL.h>
#else
	#error SDL not included
#endif
#include "../../../GUIInternal/GEAREditor/EditorApp.h"


int macos_main();
void processEvent(SDL_Window * window, SDL_Event& e, EditorApp& editorApp);