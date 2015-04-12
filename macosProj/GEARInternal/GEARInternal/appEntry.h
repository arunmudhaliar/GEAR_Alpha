//
//  AppDelegate.h
//  testMenu
//
//  Created by Mudaliar, Arun on 4/8/15.
//  Copyright (c) 2015 ea. All rights reserved.
//
#ifndef APPENTRY_H
#define APPENTRY_H
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

#endif