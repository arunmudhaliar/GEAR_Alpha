#include "geScrollBar.h"

geScrollBar::geScrollBar(geFontManager* fontmanager):
	geGUIBase(GEGUI_SCROLLBAR, "scrollBar", fontmanager)
{
	heightRatio=1.0f;
	scrollGrabberYPosition=0.0f;
	scrollGrabberHeight=0.0f;
	actualRatio=1.0f;
	is_Grabbed=false;
	scrollBarObserver=NULL;
	contentHeight=0.0f;
}

void geScrollBar::create(rendererGL10* renderer, geGUIBase* parent, MScrollBarObserver* observer)
{
	rendererGUI=renderer;
	//createBase(parent);
	scrollBarObserver=observer;

	setPos(parent->getSize().x-SCROLLBAR_SIZE, 0);

	setClientAreaPrimaryActiveForeColor(0.1f, 0.1f, 0.1f, 1.0f);
	applyPrimaryColorToVBClientArea();
	setClientAreaSecondryActiveForeColor(0.12f, 0.12f, 0.12f, 1.0f);
	setColor(&vertexBufferGrabberClientArea, 0.3f, 0.3f, 0.3f, 1.0f, EGRADIENT_HORIZONTAL_LEFT, 0.4f);

	//setSize(parent->getSize());
	setSizable(true);
	heightRatio=1.0f;
	scrollGrabberYPosition=0.0f;
	scrollGrabberHeight=0.0f;
	actualRatio=1.0f;
	contentHeight=0.0f;
	is_Grabbed=false;
	setMouseBoundCheck(false);
}

void geScrollBar::resetScrollBar()
{
	heightRatio=1.0f;
	scrollGrabberYPosition=0.0f;
	scrollGrabberHeight=0.0f;
	actualRatio=1.0f;
	contentHeight=0.0f;
	is_Grabbed=false;
}

geScrollBar::~geScrollBar()
{
}

void geScrollBar::draw()
{
	if(heightRatio<1.0f)
	{
		glPushMatrix();
		glTranslatef(m_cPos.x, m_cPos.y, 0);
		drawRect(&vertexBufferClientArea);
		glTranslatef(0, scrollGrabberYPosition, 0);
		drawRect(&vertexBufferGrabberClientArea);
		glPopMatrix();
	}
}

void geScrollBar::onPosition(float x, float y, int flag)
{
}

void geScrollBar::onSize(float cx, float cy, int flag)
{
//	const float clientarea_vertLst[8] =
//	{
//		SCROLLBAR_SIZE,	0,
//		0,	0,
//		SCROLLBAR_SIZE,	cy,
//		0,	cy,
//	};
//	memcpy(vertexBufferClientArea.vertexArray, clientarea_vertLst, sizeof(clientarea_vertLst));
    vertexBufferClientArea.updateRect(0, 0, SCROLLBAR_SIZE, cy);
    
	geGUIBase::onSize(cx, cy, flag);
}

void geScrollBar::onMouseEnterClientArea()
{
	if(heightRatio<1.0f)
		applySecondryColorToVBClientArea();
}

void geScrollBar::onMouseExitClientArea()
{
	if(heightRatio<1.0f)
		applyPrimaryColorToVBClientArea();
}

void geScrollBar::onCancelEngagedControls()
{
	applyPrimaryColorToVBClientArea();
	//is_Grabbed=false;

	geGUIBase::onCancelEngagedControls();
}

void geScrollBar::setConetentHeight(int contentHeight)
{
	this->contentHeight=contentHeight;
    heightRatio=m_cSize.y/this->contentHeight;

	if(heightRatio<1.0f)
	{
		scrollGrabberHeight=m_cSize.y*heightRatio;
		actualRatio=heightRatio;
		if(scrollGrabberHeight<20.0f)
			scrollGrabberHeight=20.0f;
//		const float clientarea_vertLst[8] =
//		{
//			SCROLLBAR_SIZE,	0,
//			0,	0,
//			SCROLLBAR_SIZE,	scrollGrabberHeight,
//			0,	scrollGrabberHeight,
//		};
//		memcpy(vertexBufferGrabberClientArea.vertexArray, clientarea_vertLst, sizeof(clientarea_vertLst));
        vertexBufferGrabberClientArea.updateRect(0, 0, SCROLLBAR_SIZE, scrollGrabberHeight);
	}
	else
		scrollGrabberYPosition=0;	//need to check

	scrollBarObserver->onScrollBarChange(this);
}

bool geScrollBar::onMouseLButtonDown(float x, float y, int nFlag)
{
	mousePrevPosition.set(x, y);
	if(isPointInsideClientArea(x, y))
	{
		is_Grabbed=true;
		return true;
	}

	return false;
}

bool geScrollBar::onMouseLButtonUp(float x, float y, int nFlag)
{
	//if(isPointInsideWindow(x, y))
	{
		is_Grabbed=false;
	}
	mousePrevPosition.set(x, y);

	return true;
}

bool geScrollBar::onMouseMove(float x, float y, int flag)
{
	//
	if(is_Grabbed && (flag&0x0001))
	{
		geVector2i diff(geVector2i(x, y)-mousePrevPosition);

		scrollGrabberYPosition+=diff.y;
		if(scrollGrabberYPosition<=0.0f)
			scrollGrabberYPosition=0.0f;

		if(scrollGrabberYPosition+scrollGrabberHeight>m_cSize.y)
			scrollGrabberYPosition=m_cSize.y-scrollGrabberHeight;

		scrollBarObserver->onScrollBarChange(this);
	}
	mousePrevPosition.set(x, y);

	return true;
}

void geScrollBar::scrollMouseWheel(int zDelta, int x, int y, int flag)
{
	float dir_mag=(zDelta>0)?-30.0f:30.0f;

	scrollGrabberYPosition+=dir_mag;
	if(scrollGrabberYPosition<=0.0f)
		scrollGrabberYPosition=0.0f;

	if(scrollGrabberYPosition+scrollGrabberHeight>m_cSize.y)
		scrollGrabberYPosition=m_cSize.y-scrollGrabberHeight;

	scrollBarObserver->onScrollBarChange(this);
}