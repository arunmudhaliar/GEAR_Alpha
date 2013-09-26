#ifndef GETOOLBARDROPMENU_H
#define GETOOLBARDROPMENU_H

#include "geButton.h"
#include <vector>
#include "../SpriteManager/Sprite2Dx.h"

#define GE_TOOLBAR_HEIGHT	17

class geToolBarDropMenu : public geButtonBase
{
public:
	geToolBarDropMenu();
	geToolBarDropMenu(const char* name, geGUIBase* parent);
	virtual ~geToolBarDropMenu();

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
	float m_cVBLayoutToggleButtonLine[3*2];
};

#endif