//
//  MenuImp.m
//  GEARInternal
//
//  Created by Mudaliar, Arun on 4/6/15.
//  Copyright (c) 2015 ea. All rights reserved.
//
#ifndef MENUIMP
#define MENUIMP

//#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import "MenuCPPInterface.h"

@interface MenuObjC : NSObject  //<NSMenuDelegate>
{
    NSMenu* m_pPopupMenu;
}

- (NSMenu*) createMenu: (std::vector<stDropMenuItem*>&) list;
- (void) showMenu:(NSPoint)pt;
- (IBAction) onMenuItemClick:(id)sender;
@end


#endif