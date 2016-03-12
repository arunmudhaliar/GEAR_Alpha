#pragma once

#ifdef _WIN32
	#include <SDL.h>
#elif defined(__APPLE__)
	#include <SDL2/SDL.h>
#else
	#error SDL not included
#endif

#include "../../../GUIInternal/GEAREditor/EditorApp.h"
#include "../../../GUIInternal/GEAREditor/OSSpecific/MenuCPPInterface.h"

#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

int appEntry();
void processEvent(SDL_Window * window, SDL_Event& e, void* userdata);