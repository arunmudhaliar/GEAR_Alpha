#pragma once

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
    int menuID;
    stDropMenuItem* parent;
    int type;
    bool hasCheck;
};

void* cpp_createMenu(std::vector<stDropMenuItem*>& list);
void cpp_showPopupMenu(void* menuobj, int x, int y);