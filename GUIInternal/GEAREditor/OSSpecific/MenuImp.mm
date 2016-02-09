//
//  MenuImp.m
//  GEARInternal
//
//  Created by Mudaliar, Arun on 4/6/15.
//  Copyright (c) 2015 ea. All rights reserved.
//

#import "MenuImp.h"
#import "EditorApp.h"

@implementation MenuObjC

void* cpp_createMenu(std::vector<stDropMenuItem*>& list)
{
    MenuObjC* menuObj = [[MenuObjC alloc] init];
    menuObj->popupMenuHandle = [menuObj createMenu: list];
    
    return ( void*)CFBridgingRetain(menuObj);
}

void cpp_showPopupMenu(void* menuobj, int x, int y)
{
    MenuObjC* _menuObj = ( MenuObjC*)CFBridgingRelease(menuobj);
    [_menuObj showMenu: NSMakePoint((CGFloat)x, (CGFloat)y)];
}



- (void) showMenu:(NSPoint)pt
{
    NSRect pp = [[[NSApplication sharedApplication] mainWindow] convertRectToScreen:NSMakeRect(pt.x, pt.y, 0, 0)];
    //NSPoint pt=NSMakePoint(0, 0);
    //[NSMenu popUpContextMenu:popupMenuHandle withEvent:nil forView:nil];
    [popupMenuHandle popUpMenuPositioningItem:nil atLocation:pp.origin inView:nil];
}

- (NSMenu*) createMenu: (std::vector<stDropMenuItem*>&) list
{
//    NSMenu* theMenu = [[NSMenu alloc] initWithTitle:@""];
//    [theMenu setAutoenablesItems:NO];
//    
//    NSMenuItem *tItem = [theMenu addItemWithTitle:@"Quit" action:@selector(test_func:) keyEquivalent:@"q"];
//    //[tItem setKeyEquivalentModifierMask:NSCommandKeyMask];
//    [tItem setRepresentedObject:@"0"];
//    [tItem setTarget:self];
//    //[theMenu setDelegate:self];
//    
//    [NSMenu popUpContextMenu:theMenu withEvent:nil forView:nil];
//    
////    NSMenu *mainMenu = [[NSApplication sharedApplication] mainMenu];
////    mainMenu;

#ifdef _WIN32
    for(int x=0;x<list.size();x++)
    {
        stDropMenuItem* item=list[x];
        item->menu_handle=NULL;
        item->sub_menu_handle=NULL;
    }
#endif
    
#ifdef _WIN32
    HMENU hPopupMenu = CreatePopupMenu();
#else
    NSMenu* hPopupMenu = [[NSMenu alloc] initWithTitle:@""];
#endif
    for(int x=(int)list.size()-1;x>=0;x--)
    {
        stDropMenuItem* item=list[x];
#ifdef _WIN32
        MENUITEMINFO minfo;
        ZeroMemory(&minfo, sizeof(MENUITEMINFO));
        minfo.cbSize = sizeof(MENUITEMINFO);
        minfo.fState = MFS_ENABLED;
        HMENU hCurrentPopupMenu=hPopupMenu;
#else
        NSMenu* hCurrentPopupMenu=hPopupMenu;
#endif
        if(item->parent)
        {
            if(item->parent->sub_menu_handle==NULL)
            {
#ifdef _WIN32
                item->parent->sub_menu_handle=CreatePopupMenu();
#else
                item->parent->sub_menu_handle = ( void*)CFBridgingRetain([[NSMenu alloc] initWithTitle:@""]);
#endif
            }
            hCurrentPopupMenu = ( NSMenu*)CFBridgingRelease(item->parent->sub_menu_handle);
        }
        
        item->menu_handle = (__bridge void*)hCurrentPopupMenu;
        if(item->type==0)
        {
#ifdef _WIN32
            minfo.wID = item->menuID;
            minfo.fMask = MIIM_ID | MIIM_STRING | MIIM_DATA;
            if(item->sub_menu_handle!=NULL)
            {
                minfo.hSubMenu=item->sub_menu_handle;
                minfo.fMask|=MIIM_SUBMENU;
                
            }
            
            minfo.fType = MFT_STRING;
            minfo.dwTypeData = item->name;
            minfo.cch = strlen(item->name);
            InsertMenuItem(hCurrentPopupMenu, 0, true, &minfo);
            bool b=SetMenuItemBitmaps(hCurrentPopupMenu, 0, MF_BYPOSITION, NULL, NULL);
            if(item->hasCheck)
            {
                CheckMenuItem(hCurrentPopupMenu, 0, MF_BYPOSITION | MF_CHECKED);
            }
#else
            NSString *menuitemname = [NSString stringWithUTF8String:item->name];
            NSMenuItem *tItem = [hCurrentPopupMenu addItemWithTitle:menuitemname action:@selector(onMenuItemClick:) keyEquivalent:@""];
            //[tItem setKeyEquivalentModifierMask:NSCommandKeyMask];
            //[tItem setRepresentedObject:@"0"];
            [tItem setTag:item->menuID];
            [tItem setTarget:self];
            if(item->hasCheck)
            {
                [tItem setState: NSOnState];
            }
#endif
        }
        else if(item->type==1)
        {
#ifdef _WIN32
            minfo.wID = 0;
            minfo.fMask = MIIM_ID | MIIM_STRING | MIIM_DATA;
            minfo.fType = MFT_SEPARATOR;
            InsertMenuItem(hCurrentPopupMenu, 0, true, &minfo);
#else
            [hCurrentPopupMenu addItem:[NSMenuItem separatorItem]];
#endif
        }
    }
    //
    
    return hPopupMenu;
}

- (IBAction) onMenuItemClick:(id)sender
{
    if (EditorApp::g_iAppSpecificEventType_MenuItemCmd != ((Uint32)-1)) {
        SDL_Event event;
        SDL_zero(event);
        event.type = EditorApp::g_iAppSpecificEventType_MenuItemCmd;
        event.user.code = (int)[sender tag];
//        event.user.data1 = significant_data;
        event.user.data2 = 0;
        SDL_PushEvent(&event);
    }
    //NSLog(@"hello %ld", (long)[sender tag]);
}

@end