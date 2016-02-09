#pragma once

#include "geGUIBase.h"

#define SCROLLBAR_SIZE	12

class geScrollBar;

class MScrollBarObserver
{
public:
	virtual void onScrollBarChange(geScrollBar* scrollbar)=0;
};

class geScrollBar : public geGUIBase
{
public:
	geScrollBar(geFontManager* fontmanager);

	void create(rendererGL10* renderer, geGUIBase* parent, MScrollBarObserver* observer);
	virtual ~geScrollBar();

	void resetScrollBar();

	virtual void draw();

	void setConetentHeight(int contentHeight);
	float getScrollGrabberYPos()	{	return scrollGrabberYPosition;	}
	void scrollMouseWheel(int zDelta, int x, int y, int flag);
	float getActualRatio()			{	return actualRatio;			}
	float getContentHeight()		{	return contentHeight;		}
	float getScrollGrabberHeight()	{	return scrollGrabberHeight;	}
	bool isScrollBarVisible()		{	return (heightRatio<1.0f);	}
	bool isScrollBarGrabbed()		{	return is_Grabbed;				}
	void setScrollGrabberYPos(float yPos)	{	scrollGrabberYPosition = yPos;	scrollBarObserver->onScrollBarChange(this);}

protected:
	virtual void onPosition(float x, float y, int flag);
	virtual void onSize(float cx, float cy, int flag);

	virtual void onMouseEnterClientArea();
	virtual void onMouseExitClientArea();

	virtual void onCancelEngagedControls();

	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
	virtual bool onMouseLButtonUp(float x, float y, int nFlag);
	virtual bool onMouseMove(float x, float y, int flag);

private:
	stVertexBuffer vertexBufferGrabberClientArea;
	float heightRatio;

	float actualRatio;
	float scrollGrabberYPosition;
	float scrollGrabberHeight;
	float contentHeight;
	bool is_Grabbed;
	geVector2i mousePrevPosition;
	MScrollBarObserver* scrollBarObserver;
};