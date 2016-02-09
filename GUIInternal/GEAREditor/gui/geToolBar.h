#pragma once

#include "geButton.h"
#include <vector>
#include "../SpriteManager/Sprite2Dx.h"

#define GE_TOOLBAR_HEIGHT	17

class geToolBarButton : public geButtonBase
{
public:
	geToolBarButton(geFontManager* fontManager);
	geToolBarButton(rendererGL10* renderer, const char* name, geGUIBase* parent, geFontManager* fontManager);
	virtual ~geToolBarButton();

	virtual void draw();
	void loadImage(const char* filename, int clipx, int clipy);

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

private:
	float vertexBufferClientAreaArray[4];
	Sprite2Dx sprite;
	bool isImageLoaded;
};

class geToolBar : public geGUIBase
{
public:
	geToolBar(geFontManager* fontmanager);
	geToolBar(unsigned short uGUIID, const char* name, geFontManager* fontmanager);
	virtual ~geToolBar();

	void create(rendererGL10* renderer, geGUIBase* parent, float x, float y, float cx, float cy);

	virtual void draw();

	virtual void onPosition(float x, float y, int flag);
	virtual void onSize(float cx, float cy, int flag);

	geGUIBase* appendToolBarControl(geGUIBase* ctrl);
	
	virtual void onAppendChild(geGUIBase* child);

private:
	std::vector<geToolBarButton*> toolBarButtonList;
};