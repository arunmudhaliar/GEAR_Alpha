//
//  AppDelegate.h
//  testMenu
//
//  Created by Mudaliar, Arun on 4/8/15.
//  Copyright (c) 2015 ea. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include <SDL2/SDL.h>
#include "EditorApp.h"

@interface AppDelegate : NSObject <NSApplicationDelegate>

-(IBAction) newScene:(id)sender;
-(IBAction) saveScene:(id)sender;
-(IBAction) openScene:(id)sender;
-(IBAction) openCameraProperty:(id)sender;
-(IBAction) openOctreeProperty:(id)sender;
-(IBAction) openLayersProperty:(id)sender;

int macos_main();
void processEvent(SDL_Window * window, SDL_Event& e, EditorApp& editorApp);
@end

