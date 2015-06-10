//
//  MenuCPPInterface.h
//  GEARInternal
//
//  Created by Mudaliar, Arun on 4/6/15.
//  Copyright (c) 2015 ea. All rights reserved.
//

#ifndef GEARInternal_MenuCPPInterface_h
#define GEARInternal_MenuCPPInterface_h

#ifdef _WIN32
    #include <windows.h>
#endif
#include <vector>

struct stDropMenuItem
{
#ifdef _WIN32
    HMENU menu_handle;
    HMENU sub_menu_handle;
#else
    void* menu_handle;
    void* sub_menu_handle;
#endif
    char name[256];
    int menuid;
    stDropMenuItem* parent;
    int type;
    bool hasCheck;
};

void* cpp_createMenu(std::vector<stDropMenuItem*>& list);
void cpp_showPopupMenu(void* menuobj, int x, int y);

#endif
