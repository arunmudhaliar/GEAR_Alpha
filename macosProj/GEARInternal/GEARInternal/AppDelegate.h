#pragma once

//
//  AppDelegate.h
//  Copyright 2017 GEAR3D
//  Gear3D
//
//  Created by Arun Mudaliar.
//
//

#import <Cocoa/Cocoa.h>
#include <SDL2/SDL.h>
#include "EditorApp.h"
#include "appEntry.h"
#import "projectSelectorDialogueWindowController.h"

@interface AppDelegate : NSObject <NSApplicationDelegate>
{
    projectSelectorDialogueWindowController* m_projectSelectorDialogueWindowController;
}

-(IBAction) newScene:(id)sender;
-(IBAction) saveScene:(id)sender;
-(IBAction) openScene:(id)sender;
-(IBAction) openCameraProperty:(id)sender;
-(IBAction) openOctreeProperty:(id)sender;
-(IBAction) openLayersProperty:(id)sender;

-(IBAction) buildForAndroid:(id)sender;


@end

