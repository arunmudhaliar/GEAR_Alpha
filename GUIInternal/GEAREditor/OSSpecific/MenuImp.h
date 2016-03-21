#pragma once

#include "../../../GEAREngine/src/GEARFirstInclude.h"

#ifdef GEAR_WINDOWS

#include <stdio.h>
#include <vector>
#include "MenuCPPInterface.h"

class MenuWin32
{
public:
	HMENU createMenu(std::vector<stDropMenuItem*>& list, unsigned int wndID);
	void showMenu(float x, float y);
	HMENU popupMenuHandle;
    unsigned int sdlWndID;
};

#else
#import <AppKit/AppKit.h>
#import "MenuCPPInterface.h"

@interface MenuObjC : NSObject  //<NSMenuDelegate>
{
    NSMenu* popupMenuHandle;
    unsigned int sdlWndID;
}

- (NSMenu*) createMenu: (std::vector<stDropMenuItem*>&) list: (unsigned int) wndID;
- (void) showMenu:(NSPoint)pt;
- (IBAction) onMenuItemClick:(id)sender;
@end

#endif