#ifndef GESCROLLBAR_H
#define GESCROLLBAR_H

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
	geScrollBar();

	void create(rendererGL10* renderer, geGUIBase* parent, MScrollBarObserver* observer);
	virtual ~geScrollBar();

	void resetScrollBar();

	virtual void draw();

	void setConetentHeight(int contentHeight);
	float getScrollGrabberYPos()	{	return m_fScrollGrabberYPos;	}
	void scrollMouseWheel(int zDelta, int x, int y, int flag);
	float getActualRatio()			{	return m_fActualRatio;			}
	float getContentHeight()		{	return m_fContentHeight;		}
	float getScrollGrabberHeight()	{	return m_fScrollGrabberHeight;	}
	bool isScrollBarVisible()		{	return (m_fHeightRatio<1.0f);	}
	bool isScrollBarGrabbed()		{	return m_bGrabbed;				}

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
	stVertexBuffer m_cVBGrabberClientArea;
	float m_fHeightRatio;

	float m_fActualRatio;
	float m_fScrollGrabberYPos	;
	float m_fScrollGrabberHeight;
	float m_fContentHeight;
	bool m_bGrabbed;
	geVector2i m_cMousePrevPos;
	MScrollBarObserver* m_pObserverPtr;
};

#endif