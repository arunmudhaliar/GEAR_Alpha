#pragma once
//
//  appentry.h
//  Copyright 2017 GEAR3D
//  Gear3D
//
//  Created by Arun Mudaliar.
//
//

#include "../../../GEAREngine/src/GEARFirstInclude.h"

#ifdef GEAR_WINDOWS
    #include <SDL.h>
#elif defined(GEAR_APPLE)
    #include <SDL2/SDL.h>
#else
    #error SDL not included
#endif

#include "../../../GUIInternal/GEAREditor/EditorApp.h"
#include "../../../GUIInternal/GEAREditor/OSSpecific/MenuCPPInterface.h"

#ifdef GEAR_WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

int appEntry();
void processEvent(SDL_Window * window, SDL_Event& e, void* userdata);
