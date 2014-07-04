#ifndef GEWINDOW_H
#define GEWINDOW_H

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
	geWindow(const char* name);
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

	bool isMovable()	{	return m_bCanMove;	}
	void clearVarsAfterWindowMoved();

	virtual int getTopMarginOffsetHeight()	{	return GE_WND_TITLE_HEIGHT+((m_pToolBar)?m_pToolBar->getSize().y:0);	}
	virtual int getTitleOffsetHeight()		{	return GE_WND_TITLE_HEIGHT;	}

	void setIamOnLayout(geLayout* layout)	{	m_pIamOnLayout=layout;	}
	geLayout* getIamOnLayout()				{	return m_pIamOnLayout;	}

	virtual geVector2f getAbsolutePositionOnScreen();

	geToolBar* getToolBar()	{	return m_pToolBar;	}

	void drawTitleAndToolBar(float xoff, float yoff, bool bActiveWindow, bool bFirstWindow);
	float getTitleWidth()	{	return m_fTitleWidth;	}

protected:
	virtual void onCreate();
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

	stVertexBuffer m_cVBTitle;
	float m_cVBClientAreaLine[8];	//Check This in openGL side

	geLayout* m_pIamOnLayout;
	geToolBar* m_pToolBar;

	bool m_bCanMove;
	float m_cVBLayoutToggleButtonLine[6*2];

	float m_fTitleWidth;
	RoundedRectControl m_cRoundedRectangle;
};

#endif