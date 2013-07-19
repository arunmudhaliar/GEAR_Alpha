#ifndef GETOOLBAR_H
#define GETOOLBAR_H

#include "geButton.h"
#include <vector>
#include "../SpriteManager/Sprite2Dx.h"

#define GE_TOOLBAR_HEIGHT	17

class geToolBarButton : public geButtonBase
{
public:
	geToolBarButton();
	geToolBarButton(const char* name, geGUIBase* parent);
	virtual ~geToolBarButton();

	virtual void draw();
	void loadImage(const char* filename, int clipx, int clipy);

protected:
	virtual void onPosition(float x, float y, int flag);
	virtual void onSize(float cx, float cy, int flag);

	virtual void onButtonStateChanged(EBUTTON_STATE eFromState);
	virtual void onButtonClicked();

	bool onMouseLButtonDown(float x, float y, int nFlag);
	bool onMouseLButtonUp(float x, float y, int nFlag);
	virtual void onMouseEnterClientArea();
	virtual void onMouseExitClientArea();

	virtual void onCancelEngagedControls();
	//virtual void onAppendChild(geGUIBase* child);

private:
	stVertexBuffer m_cVBClientArea;
	float m_cVBClientAreaLine[4];
	Sprite2Dx m_cSprite;
	bool m_bImageLoaded;
};

class geToolBar : public geGUIBase
{
public:
	geToolBar();
	geToolBar(unsigned short uGUIID, const char* name);
	virtual ~geToolBar();

	void create(geGUIBase* parent, float x, float y, float cx, float cy);

	virtual void draw();

	virtual void onPosition(float x, float y, int flag);
	virtual void onSize(float cx, float cy, int flag);

	geGUIBase* appendToolBarControl(geGUIBase* ctrl);
	
	virtual void onAppendChild(geGUIBase* child);

private:
	stVertexBuffer m_cVBClientArea;
	std::vector<geToolBarButton*> m_vToolBarButtons;
};

#endif