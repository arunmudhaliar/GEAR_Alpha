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
}

- (void) showMenu;
- (IBAction) test_func:(id)sender;
@end


#endif