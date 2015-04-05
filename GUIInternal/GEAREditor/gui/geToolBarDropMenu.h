#ifndef GETOOLBARDROPMENU_H
#define GETOOLBARDROPMENU_H

#include "geButton.h"
#include <vector>
#include "../SpriteManager/Sprite2Dx.h"

#define GE_TOOLBAR_HEIGHT	17

class geToolBarDropMenu : public geButtonBase
{
public:
	struct stDropMenuItem
	{
#ifdef _WIN32
		HMENU menu_handle;
		HMENU sub_menu_handle;
#endif
		char name[256];
		int menuid;
		stDropMenuItem* parent;
		int type;
		bool hasCheck;
	};

	geToolBarDropMenu(geFontManager* fontManager);
	geToolBarDropMenu(rendererGL10* renderer, const char* name, geGUIBase* parent, geFontManager* fontManager);
	virtual ~geToolBarDropMenu();

	virtual void draw();
	void loadImage(const char* filename, int clipx, int clipy);

	stDropMenuItem* appendMenuItem(const char* name, int menuID, stDropMenuItem* parent=NULL, bool bSeperator=false, bool bCheck=false);
	void setMenuItem(int menuID);
	void setMenuItem(const char* itemname);
	int getMenuItemCount()	{	return (int)m_vMenuItems.size();	}

	void checkMenuItem(int menuID, bool bCheck);

	void clearMenu();

protected:
	virtual void onPosition(float x, float y, int flag);
	virtual void onSize(float cx, float cy, int flag);

	virtual void onButtonStateChanged(EBUTTON_STATE eFromState, bool dontPassEventToObserver);
	virtual void onButtonClicked();

	bool onMouseLButtonDown(float x, float y, int nFlag);
	bool onMouseLButtonUp(float x, float y, int nFlag);
	virtual void onMouseEnterClientArea();
	virtual void onMouseExitClientArea();

	virtual void onCancelEngagedControls();
	//virtual void onAppendChild(geGUIBase* child);
	virtual void onSetName();

private:
	float m_cVBClientAreaLine[4];
	Sprite2Dx m_cSprite;
	bool m_bImageLoaded;
	float m_cVBLayoutToggleButtonLine[3*2];

	stDropMenuItem* m_pActiveItemPtr;
	std::vector<stDropMenuItem*> m_vMenuItems;
};

#endif