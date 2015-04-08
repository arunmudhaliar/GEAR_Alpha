//
//  MenuImp.m
//  GEARInternal
//
//  Created by Mudaliar, Arun on 4/6/15.
//  Copyright (c) 2015 ea. All rights reserved.
//

#import "MenuImp.h"

@implementation MenuObjC

void cpp_showMenu()
{
    MenuObjC* menuObj = [[MenuObjC alloc] init];
    [menuObj showMenu];
}

- (void) showMenu
{
    NSMenu* theMenu = [[NSMenu alloc] initWithTitle:@""];
    [theMenu setAutoenablesItems:NO];
    
    NSMenuItem *tItem = [theMenu addItemWithTitle:@"Quit" action:@selector(test_func:) keyEquivalent:@"q"];
    //[tItem setKeyEquivalentModifierMask:NSCommandKeyMask];
    [tItem setTarget:self];
    //[theMenu setDelegate:self];
    
    [NSMenu popUpContextMenu:theMenu withEvent:nil forView:nil];
    
    NSMenu *mainMenu = [[NSApplication sharedApplication] mainMenu];
    mainMenu;
}

- (IBAction) test_func:(id)sender
{
    NSLog(@"hello");
}

@end