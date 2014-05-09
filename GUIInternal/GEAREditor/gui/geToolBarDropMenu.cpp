#include "geToolBarDropMenu.h"
#include "geGUIManager.h"
#include "../EditorApp.h"

geToolBarDropMenu::geToolBarDropMenu():
	geButtonBase(GEGUI_TOOLBAR_DROPMENU, "ToolBarButton")
{
	//no implementation
}

geToolBarDropMenu::geToolBarDropMenu(rendererGL10* renderer, const char* name, geGUIBase* parent):
	geButtonBase(GEGUI_TOOLBAR_DROPMENU, name)
{
	createBase(renderer, parent);

	int width=geGUIManager::g_pFontArial10_84Ptr->calculateStringWidthInPixelTillNewLine(name, strlen(name), 0);
	setSize(width+27, GE_TOOLBAR_HEIGHT);

	setClientAreaPrimaryActiveForeColor(0.2f, 0.2f, 0.2f, 1.0f);
	applyPrimaryColorToVBClientArea(EGRADIENT_VERTICAL_UP, 0.45f);
	setClientAreaSecondryActiveForeColor(0.4, 0.4, 0.4, 1.0f);

	m_bImageLoaded=false;
	m_pActiveItemPtr=NULL;
	//create(parent, name, 0, 0);
}

geToolBarDropMenu::~geToolBarDropMenu()
{
	for(int x=0;x<m_vMenuItems.size();x++)
	{
		stDropMenuItem* item=m_vMenuItems[x];
		GE_DELETE(item);
	}
	m_vMenuItems.clear();
}

void geToolBarDropMenu::loadImage(const char* filename, int clipx, int clipy)
{
	m_cSprite.loadTexture(&geGUIManager::g_cTextureManager, filename);
	m_cSprite.setClip(clipx, clipy, 16, 16);
	setSize(26, getSize().y);
	m_bImageLoaded=true;
}

void geToolBarDropMenu::draw()
{
	glPushMatrix();
	glTranslatef(m_cPos.x, m_cPos.y, 0);
	drawRect(&m_cVBClientArea);
	drawLine(m_cVBClientAreaLine, 0.1, 0.1, 0.1, 1.0f, 2, false);
	if(m_bImageLoaded)
	{
		geVector2f offsetPos(5, 0);
		m_cSprite.draw(&offsetPos);
	}
	else
	{
		geGUIManager::g_pFontArial10_84Ptr->drawString(m_szName, 17, geGUIManager::g_pFontArial10_84Ptr->getLineHeight()-4, m_cSize.x);
	}

	drawTriangle(m_cVBLayoutToggleButtonLine, 0.4f, 0.4f, 0.4f, 1.0f, 3);
	glPopMatrix();
}
	
void geToolBarDropMenu::onPosition(float x, float y, int flag)
{
}

void geToolBarDropMenu::onSize(float cx, float cy, int flag)
{
	const float title_vertLst[8] =
	{
		cx-1,	0,
		-1,	0,
		cx-1,	cy,
		-1,	cy,
	};

	memcpy(m_cVBClientArea.m_cszVertexList, title_vertLst, sizeof(title_vertLst));

	const float clientarea_linevertLst[4] =
	{
		cx-1,	0,
		cx-1,	cy,
	};
	memcpy(m_cVBClientAreaLine, clientarea_linevertLst, sizeof(clientarea_linevertLst));

	int h=GE_WND_TITLE_HEIGHT-6;
	const float togglebutton_linevertLst[3*2] =
	{
		5+4,	(GE_WND_TITLE_HEIGHT-7),
		5+8,	(GE_WND_TITLE_HEIGHT-8)-(h>>1),
		5,		(GE_WND_TITLE_HEIGHT-8)-(h>>1),
	};
	memcpy(m_cVBLayoutToggleButtonLine, togglebutton_linevertLst, sizeof(togglebutton_linevertLst));
}

void geToolBarDropMenu::onButtonStateChanged(EBUTTON_STATE eFromState)
{
	switch(m_eState)
	{
	case BTN_STATE_NORMAL:
		applyPrimaryColorToVBClientArea(EGRADIENT_VERTICAL_UP, 0.45f);
		break;
	case BTN_STATE_PRESSED:
		applySecondryColorToVBClientArea(EGRADIENT_VERTICAL_DOWN, 0.45f);
		break;
	}
}

bool calculatePosOfMyChild(geGUIBase* compareme, geGUIBase* parent, int& x, int& y)
{
	if(parent)
	{
		if(parent->getGUIID()==GEGUI_TREEVIEW_NODE)
		{
			y+=parent->getSize().y;

			if(((geTreeNode*)parent)->isOpenNode())
			{
				std::vector<geGUIBase*>* childList=parent->getChildControls();
				for(std::vector<geGUIBase*>::iterator it = childList->begin(); it != childList->end(); ++it)
				{
					geGUIBase* tvnode = *it;
					if(compareme==tvnode)
					{
						x+=compareme->getPos().x-((geTreeNode*)parent)->getXOffset();
						y+=compareme->getSize().y;
						y-=parent->getSize().y;

						compareme=compareme;
						//calculate and return
						return true;
					}
					if(calculatePosOfMyChild(compareme, tvnode, x, y))
						return true;
				}
				x+=((geTreeNode*)parent)->getXOffset();
			}
		}
	}

	return false;
}

void geToolBarDropMenu::onButtonClicked()
{
	for(int x=0;x<m_vMenuItems.size();x++)
	{
		stDropMenuItem* item=m_vMenuItems[x];
		item->menu_handle=NULL;
		item->sub_menu_handle=NULL;
	}

	HMENU hPopupMenu = CreatePopupMenu();
	for(int x=m_vMenuItems.size()-1;x>=0;x--)
	{
		stDropMenuItem* item=m_vMenuItems[x];
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
			//InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, item->menuid, item->name);
			minfo.wID = item->menuid;
			minfo.fMask = MIIM_ID | MIIM_STRING | MIIM_DATA; 
			//MIIM_SUBMENU
			if(item->sub_menu_handle!=NULL)
			{
				minfo.hSubMenu=item->sub_menu_handle;
				minfo.fMask|=MIIM_SUBMENU;
			}

			minfo.fType = MFT_STRING;
			minfo.dwTypeData = item->name;
			minfo.cch = strlen(item->name);
			InsertMenuItem(hCurrentPopupMenu, 0, true, &minfo);
		}
		else if(item->type==1)
		{
			//InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
			minfo.wID = 0;
			minfo.fMask = MIIM_ID | MIIM_STRING | MIIM_DATA; 
			minfo.fType = MFT_SEPARATOR;
			InsertMenuItem(hCurrentPopupMenu, 0, true, &minfo);
		}
	}
	//
	geGUIBase* baseGUI=this;
	geGUIBase* rootTVNode=this;
	geVector2f absPos;
	while(baseGUI)
	{
		rootTVNode=baseGUI;
		baseGUI=baseGUI->getParent();
	}

	if(rootTVNode && rootTVNode->getGUIID()==GEGUI_TREEVIEW_NODE)
	{
		geGUIBase* treeView = ((geTreeNode*)rootTVNode)->getParentTreeView();
		int x, y;
		x=y=0;
		calculatePosOfMyChild(this, rootTVNode, x, y);

		POINT pt;
		pt.x=x+treeView->getPositionOnScreen().x;
		pt.y=y-treeView->getPositionOnScreen().y+((geTreeView*)treeView)->getVirtualYPos();
		ClientToScreen(EditorApp::getMainWindowHandle(), &pt);
		TrackPopupMenu(hPopupMenu, TPM_LEFTALIGN, pt.x, pt.y, 0, EditorApp::getMainWindowHandle(), NULL);

	}
	else if(rootTVNode)
	{
		POINT pt;
		pt.x=getPositionOnScreen().x;
		pt.y=-getPositionOnScreen().y;
		ClientToScreen(EditorApp::getMainWindowHandle(), &pt);
		TrackPopupMenu(hPopupMenu, TPM_LEFTALIGN, pt.x, pt.y, 0, EditorApp::getMainWindowHandle(), NULL);
	}

	if(m_pGUIObserver)
		m_pGUIObserver->onButtonClicked(this);
	buttonNormal();
}

bool geToolBarDropMenu::onMouseLButtonDown(float x, float y, int nFlag)
{
	if(m_eState==BTN_STATE_NORMAL)
	{
		buttonPressed();
	}
	else if(m_eState==BTN_STATE_PRESSED)
	{
		buttonNormal();
	}

	return true;
}

bool geToolBarDropMenu::onMouseLButtonUp(float x, float y, int nFlag)
{
	onButtonClicked();
	return true;
}

void geToolBarDropMenu::onMouseEnterClientArea()
{
	//char buffer[256];
	//sprintf(buffer, "%s onMouseEnterClientArea\n",m_szName);
	//OutputDebugString(buffer);
	//setColor(&m_cVBClientArea, 0.3, 0.3, 0.3, 1.0f, EGRADIENT_VERTICAL_UP, 0.4f);
}

void geToolBarDropMenu::onMouseExitClientArea()
{
	//char buffer[256];
	//sprintf(buffer, "%s onMouseExitClientArea\n",m_szName);
	//OutputDebugString(buffer);
	//setColor(&m_cVBClientArea, 0.2, 0.2, 0.2, 1.0f, EGRADIENT_VERTICAL_UP, 0.45f);
}

void geToolBarDropMenu::onCancelEngagedControls()
{
	//setColor(&m_cVBClientArea, 0.3, 0.3, 0.3, 1.0f, EGRADIENT_VERTICAL_UP, 0.3f);
}

geToolBarDropMenu::stDropMenuItem* geToolBarDropMenu::appendMenuItem(const char* name, int menuID, stDropMenuItem* parent, bool bSeperator)
{
	stDropMenuItem* newItem = new stDropMenuItem();
	strcpy(newItem->name, name);
	newItem->menuid=menuID;
	newItem->parent=(parent)?parent:NULL;
	newItem->type=bSeperator?1:0;
	m_vMenuItems.push_back(newItem);

	return newItem;
}

void geToolBarDropMenu::onSetName()
{
	int width=geGUIManager::g_pFontArial10_84Ptr->calculateStringWidthInPixelTillNewLine(m_szName, strlen(m_szName), 0);
	setSize(width+27, GE_TOOLBAR_HEIGHT);
}

void geToolBarDropMenu::setMenuItem(int menuID)
{
	for(std::vector<stDropMenuItem*>::iterator it = m_vMenuItems.begin(); it != m_vMenuItems.end(); ++it)
	{
		stDropMenuItem* menuitem = *it;
		if(menuitem->menuid==menuID)
		{
			setName(menuitem->name);
			m_pActiveItemPtr=menuitem;
			break;
		}
	}
}

void geToolBarDropMenu::setMenuItem(const char* itemname)
{
	for(std::vector<stDropMenuItem*>::iterator it = m_vMenuItems.begin(); it != m_vMenuItems.end(); ++it)
	{
		stDropMenuItem* menuitem = *it;
		if(strcmp(menuitem->name, itemname)==0)
		{
			setName(menuitem->name);
			m_pActiveItemPtr=menuitem;
			break;
		}
	}
}