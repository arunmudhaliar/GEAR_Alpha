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
    
    enum ESCROLLBARTYPE
    {
        VERTICAL,
        HORIZONTAL
    };
    
	geScrollBar(geFontManager* fontmanager);

	void create(rendererGL10* renderer, geGUIBase* parent, MScrollBarObserver* observer, ESCROLLBARTYPE type=VERTICAL);
	virtual ~geScrollBar();

	void resetScrollBar();

	virtual void draw();

	void setConetentSize(int contentSize);
	float getScrollGrabberPos()     {	return scrollGrabberPosition;	}
	void scrollMouseWheel(int zDelta, int x, int y, int flag);
	float getActualRatio()			{	return actualRatio;			}
	float getContentSize()          {	return contentSize;         }
	float getScrollGrabberSize()	{	return scrollGrabberSize;	}
	bool isScrollBarVisible()		{	return (currentRatio<1.0f);	}
	bool isScrollBarGrabbed()		{	return is_Grabbed;				}
	void setScrollGrabberPos(float pos, bool sendEventToObserver=true)
    {
        scrollGrabberPosition = pos;
        if(sendEventToObserver)
            scrollBarObserver->onScrollBarChange(this);
    }
    ESCROLLBARTYPE getScrollBarType()   {   return scrollBarType;   }
    
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
	float currentRatio;

	float actualRatio;
	float scrollGrabberPosition;
	float scrollGrabberSize;
	float contentSize;
	bool is_Grabbed;
	geVector2i mousePrevPosition;
	MScrollBarObserver* scrollBarObserver;
    ESCROLLBARTYPE scrollBarType;
};