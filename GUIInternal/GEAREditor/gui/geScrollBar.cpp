#include "geScrollBar.h"

geScrollBar::geScrollBar(geFontManager* fontmanager):
	geGUIBase(GEGUI_SCROLLBAR, "scrollBar", fontmanager)
{
	currentRatio=1.0f;
	scrollGrabberPosition=0.0f;
	scrollGrabberSize=0.0f;
	actualRatio=1.0f;
	is_Grabbed=false;
	scrollBarObserver=NULL;
	contentSize=0.0f;
}

void geScrollBar::create(rendererGL10* renderer, geGUIBase* parent, MScrollBarObserver* observer, ESCROLLBARTYPE type)
{
    scrollBarType = type;
	rendererGUI=renderer;
	//createBase(parent);   //Note:- scrollbar now doesn't use their parent. Need to implement a generic system.
	scrollBarObserver=observer;

    if(scrollBarType==VERTICAL)
        setPos(parent->getSize().x-SCROLLBAR_SIZE, 0);
    else
        setPos(0, parent->getSize().y-SCROLLBAR_SIZE);

	setClientAreaPrimaryActiveForeColor(0.1f, 0.1f, 0.1f, 1.0f);
	applyPrimaryColorToVBClientArea();
	setClientAreaSecondryActiveForeColor(0.12f, 0.12f, 0.12f, 1.0f);
    if(scrollBarType==VERTICAL)
        setColor(&vertexBufferGrabberClientArea, 0.3f, 0.3f, 0.3f, 1.0f, EGRADIENT_HORIZONTAL_LEFT, 0.4f);
    else
        setColor(&vertexBufferGrabberClientArea, 0.3f, 0.3f, 0.3f, 1.0f, EGRADIENT_VERTICAL_UP, 0.4f);

	setSizable(true);
	currentRatio=1.0f;
	scrollGrabberPosition=0.0f;
	scrollGrabberSize=0.0f;
	actualRatio=1.0f;
	contentSize=0.0f;
	is_Grabbed=false;
	setMouseBoundCheck(false);
}

void geScrollBar::resetScrollBar()
{
	currentRatio=1.0f;
	scrollGrabberPosition=0.0f;
	scrollGrabberSize=0.0f;
	actualRatio=1.0f;
	contentSize=0.0f;
	is_Grabbed=false;
}

geScrollBar::~geScrollBar()
{
}

void geScrollBar::draw()
{
	if(currentRatio<1.0f)
	{
		glPushMatrix();
		glTranslatef(m_cPos.x, m_cPos.y, 0);
		drawRect(&vertexBufferClientArea);
        if(scrollBarType==VERTICAL)
            glTranslatef(0, scrollGrabberPosition, 0);
        else
            glTranslatef(scrollGrabberPosition, 0, 0);
        
		drawRect(&vertexBufferGrabberClientArea);
		glPopMatrix();
	}
}

void geScrollBar::onPosition(float x, float y, int flag)
{
}

void geScrollBar::onSize(float cx, float cy, int flag)
{
    if(scrollBarType==VERTICAL)
        vertexBufferClientArea.updateRect(0, 0, SCROLLBAR_SIZE, cy);
    else
        vertexBufferClientArea.updateRect(0, 0, cx, SCROLLBAR_SIZE);
    
	geGUIBase::onSize(cx, cy, flag);
}

void geScrollBar::onMouseEnterClientArea()
{
	if(currentRatio<1.0f)
		applySecondryColorToVBClientArea();
}

void geScrollBar::onMouseExitClientArea()
{
	if(currentRatio<1.0f)
		applyPrimaryColorToVBClientArea();
}

void geScrollBar::onCancelEngagedControls()
{
	applyPrimaryColorToVBClientArea();

	geGUIBase::onCancelEngagedControls();
}

void geScrollBar::setConetentSize(int contentSize)
{
	this->contentSize=contentSize;
    
    float size_val = m_cSize.x;
    if(scrollBarType==VERTICAL)
        size_val=m_cSize.y;

    currentRatio=size_val/this->contentSize;
	if(currentRatio<1.0f)
	{
		scrollGrabberSize=size_val*currentRatio;
		actualRatio=currentRatio;
		if(scrollGrabberSize<20.0f)
			scrollGrabberSize=20.0f;

        if(scrollBarType==VERTICAL)
            vertexBufferGrabberClientArea.updateRect(0, 0, SCROLLBAR_SIZE, scrollGrabberSize);
        else
            vertexBufferGrabberClientArea.updateRect(0, 0, scrollGrabberSize, SCROLLBAR_SIZE);
	}
	else
		scrollGrabberPosition=0;	//need to check

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
	if(is_Grabbed && (flag&0x0001))
	{
		geVector2i diff(geVector2i(x, y)-mousePrevPosition);

        if(scrollBarType==VERTICAL)
            scrollGrabberPosition+=diff.y;
        else
            scrollGrabberPosition+=diff.x;
        
		if(scrollGrabberPosition<=0.0f)
			scrollGrabberPosition=0.0f;

        if(scrollBarType==VERTICAL)
        {
            if(scrollGrabberPosition+scrollGrabberSize>m_cSize.y)
                scrollGrabberPosition=m_cSize.y-scrollGrabberSize;
        }
        else
        {
            if(scrollGrabberPosition+scrollGrabberSize>m_cSize.x)
                scrollGrabberPosition=m_cSize.x-scrollGrabberSize;
        }
		scrollBarObserver->onScrollBarChange(this);
	}
	mousePrevPosition.set(x, y);

	return true;
}

void geScrollBar::scrollMouseWheel(int zDelta, int x, int y, int flag)
{
	float dir_mag=(zDelta>0)?-30.0f:30.0f;

	scrollGrabberPosition+=dir_mag;
	if(scrollGrabberPosition<=0.0f)
		scrollGrabberPosition=0.0f;

    if(scrollBarType==VERTICAL)
    {
        if(scrollGrabberPosition+scrollGrabberSize>m_cSize.y)
            scrollGrabberPosition=m_cSize.y-scrollGrabberSize;
    }
    else
    {
        if(scrollGrabberPosition+scrollGrabberSize>m_cSize.x)
            scrollGrabberPosition=m_cSize.x-scrollGrabberSize;
    }

	scrollBarObserver->onScrollBarChange(this);
}