//
//  MenuImp.m
//  GEARInternal
//
//  Created by Mudaliar, Arun on 4/6/15.
//  Copyright (c) 2015 ea. All rights reserved.
//
#ifndef MENUIMP
#define MENUIMP

#ifdef _WIN32

#include <stdio.h>
#include <vector>
#include "MenuCPPInterface.h"

class MenuWin32
{
public:
	HMENU createMenu(std::vector<stDropMenuItem*>& list);
	void showMenu(float x, float y);
	HMENU m_pPopupMenu;
};

#else
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

#endif