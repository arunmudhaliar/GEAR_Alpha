#pragma once

#ifdef _WIN32

#include <stdio.h>
#include <vector>
#include "MenuCPPInterface.h"

class MenuWin32
{
public:
	HMENU createMenu(std::vector<stDropMenuItem*>& list);
	void showMenu(float x, float y);
	HMENU popupMenuHandle;
};

#else
#import <AppKit/AppKit.h>
#import "MenuCPPInterface.h"

@interface MenuObjC : NSObject  //<NSMenuDelegate>
{
    NSMenu* popupMenuHandle;
}

- (NSMenu*) createMenu: (std::vector<stDropMenuItem*>&) list;
- (void) showMenu:(NSPoint)pt;
- (IBAction) onMenuItemClick:(id)sender;
@end

#endif