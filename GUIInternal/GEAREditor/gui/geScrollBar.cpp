#include "geScrollBar.h"

geScrollBar::geScrollBar():
	geGUIBase(GEGUI_SCROLLBAR, "scrollBar")
{
	m_fHeightRatio=1.0f;
	m_fScrollGrabberYPos=0.0f;
	m_fScrollGrabberHeight=0.0f;
	m_fActualRatio=1.0f;
	m_bGrabbed=false;
	m_pObserverPtr=NULL;
	m_fContentHeight=0.0f;
}

void geScrollBar::create(rendererGL10* renderer, geGUIBase* parent, MScrollBarObserver* observer)
{
	m_pRenderer=renderer;
	//createBase(parent);
	m_pObserverPtr=observer;

	setPos(parent->getSize().x-SCROLLBAR_SIZE, 0);
	setColor(&m_cVBClientArea, 0.1f, 0.1f, 0.1f, 1.0f);
	setColor(&m_cVBGrabberClientArea, 0.3f, 0.3f, 0.3f, 1.0f, EGRADIENT_HORIZONTAL_LEFT, 0.4f);
	//setSize(parent->getSize());
	setSizable(true);
	m_fHeightRatio=1.0f;
	m_fScrollGrabberYPos=0.0f;
	m_fScrollGrabberHeight=0.0f;
	m_fActualRatio=1.0f;
	m_fContentHeight=0.0f;
	m_bGrabbed=false;
	setMouseBoundCheck(false);
}

void geScrollBar::resetScrollBar()
{
	m_fHeightRatio=1.0f;
	m_fScrollGrabberYPos=0.0f;
	m_fScrollGrabberHeight=0.0f;
	m_fActualRatio=1.0f;
	m_fContentHeight=0.0f;
	m_bGrabbed=false;
}

geScrollBar::~geScrollBar()
{
}

void geScrollBar::draw()
{
	if(m_fHeightRatio<1.0f)
	{
		glPushMatrix();
		glTranslatef(m_cPos.x, m_cPos.y, 0);
		drawRect(&m_cVBClientArea);
		glTranslatef(0, m_fScrollGrabberYPos, 0);
		drawRect(&m_cVBGrabberClientArea);
		glPopMatrix();
	}
}

void geScrollBar::onPosition(float x, float y, int flag)
{
}

void geScrollBar::onSize(float cx, float cy, int flag)
{
	const float clientarea_vertLst[8] =
	{
		SCROLLBAR_SIZE,	0,
		0,	0,
		SCROLLBAR_SIZE,	cy,
		0,	cy,
	};
	memcpy(m_cVBClientArea.m_cszVertexList, clientarea_vertLst, sizeof(clientarea_vertLst));

	geGUIBase::onSize(cx, cy, flag);
}

void geScrollBar::onMouseEnterClientArea()
{
	if(m_fHeightRatio<1.0f)
		setColor(&m_cVBClientArea, 0.12f, 0.12f, 0.12f, 1.0f);
}

void geScrollBar::onMouseExitClientArea()
{
	if(m_fHeightRatio<1.0f)
		setColor(&m_cVBClientArea, 0.1f, 0.1f, 0.1f, 1.0f);
}

void geScrollBar::onCancelEngagedControls()
{
	setColor(&m_cVBClientArea, 0.1f, 0.1f, 0.1f, 1.0f);
	//m_bGrabbed=false;

	geGUIBase::onCancelEngagedControls();
}

void geScrollBar::setConetentHeight(int contentHeight)
{
	m_fHeightRatio=m_cSize.y/contentHeight;
	m_fContentHeight=contentHeight;

	if(m_fHeightRatio<1.0f)
	{
		m_fScrollGrabberHeight=m_cSize.y*m_fHeightRatio;
		m_fActualRatio=m_fHeightRatio;
		if(m_fScrollGrabberHeight<20.0f)
			m_fScrollGrabberHeight=20.0f;
		const float clientarea_vertLst[8] =
		{
			SCROLLBAR_SIZE,	0,
			0,	0,
			SCROLLBAR_SIZE,	m_fScrollGrabberHeight,
			0,	m_fScrollGrabberHeight,
		};
		memcpy(m_cVBGrabberClientArea.m_cszVertexList, clientarea_vertLst, sizeof(clientarea_vertLst));
	}

	m_pObserverPtr->onScrollBarChange(this);
}

bool geScrollBar::onMouseLButtonDown(float x, float y, int nFlag)
{
	m_cMousePrevPos.set(x, y);
	if(isPointInsideClientArea(x, y))
	{
		m_bGrabbed=true;
		return true;
	}

	return false;
}

bool geScrollBar::onMouseLButtonUp(float x, float y, int nFlag)
{
	//if(isPointInsideWindow(x, y))
	{
		m_bGrabbed=false;
	}
	m_cMousePrevPos.set(x, y);

	return true;
}

bool geScrollBar::onMouseMove(float x, float y, int flag)
{
	//
	if(m_bGrabbed && (flag&0x0001))
	{
		geVector2i diff(geVector2i(x, y)-m_cMousePrevPos);

		m_fScrollGrabberYPos+=diff.y;
		if(m_fScrollGrabberYPos<=0.0f)
			m_fScrollGrabberYPos=0.0f;

		if(m_fScrollGrabberYPos+m_fScrollGrabberHeight>m_cSize.y)
			m_fScrollGrabberYPos=m_cSize.y-m_fScrollGrabberHeight;

		m_pObserverPtr->onScrollBarChange(this);
	}
	m_cMousePrevPos.set(x, y);

	return true;
}

void geScrollBar::scrollMouseWheel(int zDelta, int x, int y, int flag)
{
	float dir_mag=(zDelta>0)?-30.0f:30.0f;

	m_fScrollGrabberYPos+=dir_mag;
	if(m_fScrollGrabberYPos<=0.0f)
		m_fScrollGrabberYPos=0.0f;

	if(m_fScrollGrabberYPos+m_fScrollGrabberHeight>m_cSize.y)
		m_fScrollGrabberYPos=m_cSize.y-m_fScrollGrabberHeight;

	m_pObserverPtr->onScrollBarChange(this);
}