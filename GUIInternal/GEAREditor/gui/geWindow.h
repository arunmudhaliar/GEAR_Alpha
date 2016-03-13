#pragma once

#include "geGUIBase.h"
#include "geToolBar.h"
#include "geLayout.h"
#include "geButton.h"
#include "geTreeView.h"
#include "gePushButton.h"
#include "RoundedRectControl.h"

#define GE_WND_TITLE_HEIGHT	20.0f

class geLayout;

class geWindow : public geGUIBase
{
public:
	geWindow(const char* name, geFontManager* fontmanager);
	virtual ~geWindow();

	void create(rendererGL10* renderer, geGUIBase* parent, float x, float y, float cx, float cy, bool bCreateToolBar=false);
	void show();
	void hide();
	void enable();
	void disable();
	void update(float dt);
	virtual void draw();

	void setTitleColor(float r, float g, float b, float a);
	void setClientColor(float r, float g, float b, float a);

	bool isPointInsideWindowTitle(int x, int y);
	//bool selectWindow(int x, int y);

	bool isMovable()	{	return is_Movable;	}
	void clearVarsAfterWindowMoved();

	virtual int getTopMarginOffsetHeight()	{	return GE_WND_TITLE_HEIGHT+((toolBar)?toolBar->getSize().y:0);	}
	virtual int getTitleOffsetHeight()		{	return GE_WND_TITLE_HEIGHT;	}

	void setIamOnLayout(geLayout* layout)	{	iamOnLayout=layout;	}
	geLayout* getIamOnLayout()				{	return iamOnLayout;	}

	virtual geVector2f getAbsolutePositionOnScreen();

	geToolBar* getToolBar()	{	return toolBar;	}

	void drawTitleAndToolBar(float xoff, float yoff, bool bActiveWindow, bool bFirstWindow);
	float getTitleWidth()	{	return titleWidth;	}

protected:
	virtual void onCreate(float cx, float cy);
	virtual void onPosition(float x, float y, int flag);
	virtual void onSize(float cx, float cy, int flag);
	virtual void onDraw();
	virtual void onDestroy();

	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
	virtual bool onMouseLButtonUp(float x, float y, int nFlag);
	virtual bool onMouseMove(float x, float y, int flag);

	virtual void onMouseEnterClientArea();
	virtual void onMouseExitClientArea();
private:

	stVertexBuffer vertexBufferTitle;
	float vertexBufferClientAreaArray[8];	//Check This in openGL side

	geLayout* iamOnLayout;
	geToolBar* toolBar;

	bool is_Movable;
	float vertexBufferToggleButtonArray[6*2];

	float titleWidth;
	RoundedRectControl roundedRectControl;
};