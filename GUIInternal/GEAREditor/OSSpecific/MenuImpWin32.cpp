#include "MenuImp.h"
#include "../EditorApp.h"

void* cpp_createMenu(std::vector<stDropMenuItem*>& list, unsigned int wndID)
{
	MenuWin32* menuobj = new MenuWin32();
    menuobj->sdlWndID = wndID;
	menuobj->popupMenuHandle = menuobj->createMenu(list, wndID);
	return (void*)menuobj;
}

void cpp_showPopupMenu(void* menuobj, int x, int y)
{
    MenuWin32* _menuObj = (MenuWin32*)menuobj;
    _menuObj->showMenu(x, y);
}

HMENU MenuWin32::createMenu(std::vector<stDropMenuItem*>& list, unsigned int wndID)
{
	for(int x=0;x<list.size();x++)
    {
        stDropMenuItem* item=list[x];
        item->menu_handle=NULL;
        item->sub_menu_handle=NULL;
    }

	HMENU hPopupMenu = CreatePopupMenu();

	for(int x=(int)list.size()-1;x>=0;x--)
    {
        stDropMenuItem* item=list[x];
        MENUITEMINFO minfo;
        ZeroMemory(&minfo, sizeof(MENUITEMINFO));
        minfo.cbSize = sizeof(MENUITEMINFO);
        minfo.fState = MFS_ENABLED;
        HMENU hCurrentPopupMenu=hPopupMenu;

        if(item->parent)
        {
            if(item->parent->sub_menu_handle==NULL)
            {
                item->parent->sub_menu_handle=CreatePopupMenu();
            }
            hCurrentPopupMenu=item->parent->sub_menu_handle;
        }
        
        item->menu_handle=hCurrentPopupMenu;
        if(item->type==0)
        {
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
        }
        else if(item->type==1)
        {
            minfo.wID = 0;
            minfo.fMask = MIIM_ID | MIIM_STRING | MIIM_DATA;
            minfo.fType = MFT_SEPARATOR;
            InsertMenuItem(hCurrentPopupMenu, 0, true, &minfo);
        }
    }
    //
    
    return hPopupMenu;
}

void MenuWin32::showMenu(float x, float y)
{
	POINT pt;
	pt.x=x;
	pt.y=y;

	ClientToScreen(EditorApp::getMainWindowHandle(), &pt);
	TrackPopupMenu(popupMenuHandle, TPM_LEFTALIGN, pt.x, pt.y, 0, EditorApp::getMainWindowHandle(), NULL);
}