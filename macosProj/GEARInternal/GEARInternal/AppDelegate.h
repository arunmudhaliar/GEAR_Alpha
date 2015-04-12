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
#include "appEntry.h"

@interface AppDelegate : NSObject <NSApplicationDelegate>

-(IBAction) newScene:(id)sender;
-(IBAction) saveScene:(id)sender;
-(IBAction) openScene:(id)sender;
-(IBAction) openCameraProperty:(id)sender;
-(IBAction) openOctreeProperty:(id)sender;
-(IBAction) openLayersProperty:(id)sender;

-(IBAction) buildForAndroid:(id)sender;
@end

