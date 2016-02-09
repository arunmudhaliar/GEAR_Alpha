#pragma once

#include "geButton.h"
#include <vector>
#include "../SpriteManager/Sprite2Dx.h"
#include "../OSSpecific/MenuCPPInterface.h"

#define GE_TOOLBAR_HEIGHT	17

class geToolBarDropMenu : public geButtonBase
{
public:
	geToolBarDropMenu(geFontManager* fontManager);
	geToolBarDropMenu(rendererGL10* renderer, const char* name, geGUIBase* parent, geFontManager* fontManager);
	virtual ~geToolBarDropMenu();

	virtual void draw();
	void loadImage(const char* filename, int clipx, int clipy);

	stDropMenuItem* appendMenuItem(const char* name, int menuID, stDropMenuItem* parent=NULL, bool bSeperator=false, bool bCheck=false);
	void setMenuItem(int menuID);
	void setMenuItem(const char* itemname);
	int getMenuItemCount()	{	return (int)dropMenuItemList.size();	}

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
	float vertexBufferClientAreaArray[4];
	Sprite2Dx sprite;
	bool isImageLoaded;
	float vertexBufferToggleButtonArray[3*2];

	stDropMenuItem* activeDropMenuItem;
	std::vector<stDropMenuItem*> dropMenuItemList;
};