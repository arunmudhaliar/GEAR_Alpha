#include "geLayout.h"

geLayout::geLayout(const char* name, geFontManager* fontmanager):
	geGUIBase(GEGUI_LAYOUT, name, fontmanager)
{
	activeWindow=NULL;
}

geLayout:: ~geLayout()
{
	for(std::vector<geLayout*>::iterator it = childTopLayoutList.begin(); it != childTopLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		GE_DELETE(obj);
	}
	childTopLayoutList.clear();

	for(std::vector<geLayout*>::iterator it = childBottomLayoutList.begin(); it != childBottomLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		GE_DELETE(obj);
	}
	childBottomLayoutList.clear();

	for(std::vector<geLayout*>::iterator it = childRightLayoutList.begin(); it != childRightLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		GE_DELETE(obj);
	}
	childRightLayoutList.clear();

	for(std::vector<geLayout*>::iterator it = childLeftLayoutList.begin(); it != childLeftLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		GE_DELETE(obj);
	}
	childLeftLayoutList.clear();
}

void geLayout::create(rendererGL10* renderer, geLayout* pParentLayout, float x, float y, float cx, float cy)
{
	rendererGUI=renderer;
	parentLayout=pParentLayout;
	activeWindow = NULL;
	setPos(x, y);
	setSize(cx, cy);

	setClientAreaPrimaryActiveForeColor(0.12f, 0.12f, 0.12f, 1.0f);
	applyPrimaryColorToVBClientArea();

	setSizable(true);
}

//void geLayout::appendChildLayout(geLayout* childLayout)
//{
//	childLayout->setParentLayout(this);
//	m_vChildLayouts.push_back(childLayout);
//}

void geLayout::appendLeftChildLayout(geLayout* childLayout)
{
	childLayout->setParentLayout(this);
	childLeftLayoutList.push_back(childLayout);
}

void geLayout::appendRightChildLayout(geLayout* childLayout)
{
	childLayout->setParentLayout(this);
	childRightLayoutList.push_back(childLayout);
}

void geLayout::appendTopChildLayout(geLayout* childLayout)
{
	childLayout->setParentLayout(this);
	childTopLayoutList.push_back(childLayout);
}

void geLayout::appendBottomChildLayout(geLayout* childLayout)
{
	childLayout->setParentLayout(this);
	childBottomLayoutList.push_back(childLayout);
}

void geLayout::appendWindow(geWindow* window)
{
	setActiveWindow(window);
	activeWindow->setIamOnLayout(this);
	activeWindow->setParent(this);

	//reposition the window
	activeWindow->setPos(BORDER_LAYOUT_OFFSET+LEFT_LAYOUT_MARGIN, BORDER_LAYOUT_OFFSET);

	//resize the window
	activeWindow->setSize(getSize().x-(BORDER_LAYOUT_OFFSET<<1), getSize().y-((BORDER_LAYOUT_OFFSET<<1)+LEFT_LAYOUT_MARGIN));

	childWindowList.push_back(window);
}

void geLayout::draw()
{
#ifdef LOG_GLERROR
	int err=glGetError();
	if(err!=GL_NO_ERROR)
	{
		printf("glGetError 0x%x\n", err);
	}
#endif

	if(parentLayout!=NULL)
	{
		glViewport(m_cPos.x+(BORDER_LAYOUT_OFFSET>>1), rendererGUI->getViewPortSz().y-(m_cSize.y+m_cPos.y+(BORDER_LAYOUT_OFFSET>>1)), m_cSize.x-(BORDER_LAYOUT_OFFSET), m_cSize.y-(BORDER_LAYOUT_OFFSET));	
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0.0f, (int)(m_cSize.x-(BORDER_LAYOUT_OFFSET)), (int)(m_cSize.y-(BORDER_LAYOUT_OFFSET)), 0.0f);
		glMatrixMode(GL_MODELVIEW);
		drawLine(vertexBufferClientAreaArray, 0.13f, 0.13f, 0.13f, 1.0f, 3, false); 
		//drawRect(&vertexBufferClientArea);

		float _temp_pos=0;
		for(std::vector<geWindow*>::iterator it = childWindowList.begin(); it != childWindowList.end(); ++it)
		{
			geWindow* wnd = *it;
			if(activeWindow==wnd)
			{
				wnd->drawTitleAndToolBar(_temp_pos, 0.0f, true, (*childWindowList.begin()==wnd));
				_temp_pos+=31;	//30+1 for rounded rectangle
			}
			else
			{
				wnd->drawTitleAndToolBar(_temp_pos, 0.0f, false, (*childWindowList.begin()==wnd));
				_temp_pos+=20;
			}
			_temp_pos+=(wnd->getTitleWidth());
		}
		activeWindow->draw();
	}

	//draw childs
	for(std::vector<geLayout*>::iterator it = childTopLayoutList.begin(); it != childTopLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->draw();
	}
	for(std::vector<geLayout*>::iterator it = childBottomLayoutList.begin(); it != childBottomLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->draw();
	}
	for(std::vector<geLayout*>::iterator it = childRightLayoutList.begin(); it != childRightLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->draw();
	}
	for(std::vector<geLayout*>::iterator it = childLeftLayoutList.begin(); it != childLeftLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->draw();
	}
}

void geLayout::onPosition(float x, float y, int flag)
{
	if(activeWindow)
	{
		//reposition the window
		activeWindow->setPos(BORDER_LAYOUT_OFFSET+LEFT_LAYOUT_MARGIN, BORDER_LAYOUT_OFFSET);

		//resize the window
		//activeWindow->setSize(getSize().x-(BORDER_LAYOUT_OFFSET<<1), getSize().y-(BORDER_LAYOUT_OFFSET<<1));
	}
}

void geLayout::resize(double xScale, double yScale)
{
	setPos(m_cPos.x*xScale, m_cPos.y*yScale);
	setSize(m_cSize.x*xScale, m_cSize.y*yScale);
	resizeComplete();

	for(std::vector<geLayout*>::iterator it = childTopLayoutList.begin(); it != childTopLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->resize(xScale, yScale);
	}

	for(std::vector<geLayout*>::iterator it = childBottomLayoutList.begin(); it != childBottomLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->resize(xScale, yScale);
	}

	for(std::vector<geLayout*>::iterator it = childRightLayoutList.begin(); it != childRightLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->resize(xScale, yScale);
	}

	for(std::vector<geLayout*>::iterator it = childLeftLayoutList.begin(); it != childLeftLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->resize(xScale, yScale);
	}
}

geLayout* geLayout::dropWindowOnMe(geWindow* window)
{
	if(parentLayout!=NULL /*&& isPointInsideWindow(window->getPos().x, window->getPos().y)*/)
	{
		if(activeWindow==window)
		{
			//reposition the window
			activeWindow->setPos(BORDER_LAYOUT_OFFSET+LEFT_LAYOUT_MARGIN, BORDER_LAYOUT_OFFSET);

			////resize the window
			//activeWindow->setSize(getSize().x-(BORDER_LAYOUT_OFFSET<<1), getSize().y-(BORDER_LAYOUT_OFFSET<<1));
			//activeWindow->setSize(getSize().x-(BORDER_LAYOUT_OFFSET<<1), activeWindow->getSize().y);
			return NULL;
		}
	}

	return NULL;
}

void geLayout::onSize(float cx, float cy, int flag)
{
	//const float title_vertLst[8] =
	//{
	//	cx-1,	1,
	//	1,	1,
	//	cx-1,	cy-1,
	//	1,	cy-1,
	//};
    vertexBufferClientArea.updateRect(0, 0, cx, cy);

	const float clientarea_linevertLst[6] =
	{
		cx,	1,
		1,	1,
		1,	cy
	};
	memcpy(vertexBufferClientAreaArray, clientarea_linevertLst, sizeof(clientarea_linevertLst));

	if(activeWindow)
	{
		//reposition the window
		activeWindow->setPos(BORDER_LAYOUT_OFFSET+LEFT_LAYOUT_MARGIN, BORDER_LAYOUT_OFFSET);

		////resize the window
		activeWindow->setSize(getSize().x-(BORDER_LAYOUT_OFFSET<<1), getSize().y-((BORDER_LAYOUT_OFFSET<<1)+LEFT_LAYOUT_MARGIN));

	}
}

bool geLayout::removeChildLayout(geLayout* childLayout)
{
	childTopLayoutList.erase(std::remove(childTopLayoutList.begin(), childTopLayoutList.end(), childLayout), childTopLayoutList.end());
	childBottomLayoutList.erase(std::remove(childBottomLayoutList.begin(), childBottomLayoutList.end(), childLayout), childBottomLayoutList.end());
	childRightLayoutList.erase(std::remove(childRightLayoutList.begin(), childRightLayoutList.end(), childLayout), childRightLayoutList.end());
	childLeftLayoutList.erase(std::remove(childLeftLayoutList.begin(), childLeftLayoutList.end(), childLayout), childLeftLayoutList.end());

	return true;
}

geLayout* geLayout::getRightMostParentLayout()
{
	geLayout* patrntLayout = this;
	while(patrntLayout && patrntLayout->getLayoutDirection()==RIGHT_TO_PARENT)
		patrntLayout=patrntLayout->getParentLayout();

	return patrntLayout;
}

geLayout* geLayout::getLeftMostParentLayout()
{
	geLayout* patrntLayout = this;
	while(patrntLayout && patrntLayout->getLayoutDirection()==LEFT_TO_PARENT)
		patrntLayout=patrntLayout->getParentLayout();

	return patrntLayout;
}

geLayout* geLayout::getTopMostParentLayout()
{
	geLayout* patrntLayout = this;
	while(patrntLayout && patrntLayout->getLayoutDirection()==TOP_TO_PARENT)
		patrntLayout=patrntLayout->getParentLayout();

	return patrntLayout;
}

geLayout* geLayout::getBottomMostParentLayout()
{
	geLayout* patrntLayout = this;
	while(patrntLayout && patrntLayout->getLayoutDirection()==BOTTOM_TO_PARENT)
		patrntLayout=patrntLayout->getParentLayout();

	return patrntLayout;
}

geLayout* geLayout::createLeft(geWindow* window, float ratio)
{
#ifdef _WIN32
	OutputDebugString("createLeft\n");
#endif

	geLayout* pParentOfNewLayout = this;
	//check if i am on the right side of my parent
	if(getLayoutDirection()==RIGHT_TO_PARENT)
	{
		pParentOfNewLayout=getRightMostParentLayout();
	}
	
	geLayout* pLayout = new geLayout("layout", fontManagerGUI);
	pLayout->create(rendererGUI, pParentOfNewLayout, this->getPos().x, this->getPos().y, this->getSize().x*ratio,  this->getSize().y);
	pLayout->setLayoutDirection((pParentOfNewLayout==this)?LEFT_TO_PARENT:RIGHT_TO_PARENT);

	this->setPos(this->getPos().x+this->getSize().x*ratio, this->getPos().y);
	this->setSize(this->getSize().x*(1.0f-ratio),  this->getSize().y);

	if(pParentOfNewLayout==this)
		childLeftLayoutList.push_back(pLayout);
	else
	{
		pParentOfNewLayout->appendRightChildLayout(pLayout);
		//remove me from the existing parent and add to child as new layout
		getParentLayout()->removeChildLayout(this);
		pLayout->appendLeftChildLayout(this);
	}

	pLayout->appendWindow(window);

	return pLayout;
}

geLayout* geLayout::createRight(geWindow* window, float ratio)
{
#ifdef _WIN32
	OutputDebugString("createRight\n");
#endif
    
	geLayout* pParentOfNewLayout = this;
	//check if i am on the right side of my parent
	if(getLayoutDirection()==LEFT_TO_PARENT)
	{
		pParentOfNewLayout=getLeftMostParentLayout();
	}

	geLayout* pLayout = new geLayout("layout", fontManagerGUI);
	pLayout->create(rendererGUI, pParentOfNewLayout, this->getPos().x+this->getSize().x*(1.0f-ratio), this->getPos().y, this->getSize().x*ratio,  this->getSize().y);
	pLayout->setLayoutDirection((pParentOfNewLayout==this)?RIGHT_TO_PARENT:LEFT_TO_PARENT);

	this->setSize(this->getSize().x*(1.0f-ratio),  this->getSize().y);

	if(pParentOfNewLayout==this)
		childRightLayoutList.push_back(pLayout);
	else
	{
		pParentOfNewLayout->appendLeftChildLayout(pLayout);
		//remove me from the existing parent and add to child as new layout
		getParentLayout()->removeChildLayout(this);
		pLayout->appendRightChildLayout(this);
	}

	pLayout->appendWindow(window);

	return pLayout;
}

geLayout* geLayout::createTop(geWindow* window, float ratio)
{
#ifdef _WIN32
	OutputDebugString("createTop\n");
#endif
    
	geLayout* pParentOfNewLayout = this;
	//check if i am on the right side of my parent
	if(getLayoutDirection()==BOTTOM_TO_PARENT)
	{
		pParentOfNewLayout=getBottomMostParentLayout();
	}

	geLayout* pLayout = new geLayout("layout", fontManagerGUI);
	pLayout->create(rendererGUI, pParentOfNewLayout, this->getPos().x, this->getPos().y, this->getSize().x,  this->getSize().y*ratio);
	pLayout->setLayoutDirection((pParentOfNewLayout==this)?TOP_TO_PARENT:BOTTOM_TO_PARENT);

	this->setPos(this->getPos().x, this->getPos().y+this->getSize().y*ratio);
	this->setSize(this->getSize().x,  this->getSize().y*(1.0f-ratio));

	if(pParentOfNewLayout==this)
		childTopLayoutList.push_back(pLayout);
	else
	{
		pParentOfNewLayout->appendBottomChildLayout(pLayout);
		//remove me from the existing parent and add to child as new layout
		getParentLayout()->removeChildLayout(this);
		pLayout->appendTopChildLayout(this);
	}

	pLayout->appendWindow(window);

	return pLayout;
}

geLayout* geLayout::createBottom(geWindow* window, float ratio)
{
#ifdef _WIN32
	OutputDebugString("createBottom\n");
#endif
    
	geLayout* pParentOfNewLayout = this;
	//check if i am on the right side of my parent
	if(getLayoutDirection()==TOP_TO_PARENT)
	{
		pParentOfNewLayout=getTopMostParentLayout();
	}

	geLayout* pLayout = new geLayout("layout", fontManagerGUI);
	pLayout->create(rendererGUI, pParentOfNewLayout, this->getPos().x, this->getPos().y+this->getSize().y*(1.0f-ratio), this->getSize().x,  this->getSize().y*ratio);
	pLayout->setLayoutDirection((pParentOfNewLayout==this)?BOTTOM_TO_PARENT:TOP_TO_PARENT);

	this->setSize(this->getSize().x,  this->getSize().y*(1.0f-ratio));

	if(pParentOfNewLayout==this)
		childBottomLayoutList.push_back(pLayout);
	else
	{
		pParentOfNewLayout->appendTopChildLayout(pLayout);
		//remove me from the existing parent and add to child as new layout
		getParentLayout()->removeChildLayout(this);
		pLayout->appendBottomChildLayout(this);
	}

	pLayout->appendWindow(window);

	return pLayout;
}

geLayout* geLayout::createAsParent(geWindow* window)
{
	geLayout* pLayout = new geLayout("layout", fontManagerGUI);
	pLayout->create(rendererGUI, this, this->getPos().x, this->getPos().y, this->getSize().x,  this->getSize().y);
	pLayout->setLayoutDirection(LAYOUT_PARENT);
	this->setSize(this->getSize().x,  this->getSize().y);
	appendLeftChildLayout(pLayout);
	pLayout->appendWindow(window);

	return pLayout;
}

bool geLayout::onMouseLButtonDown(float x, float y, int nFlag)
{
	if(y<GE_WND_TITLE_HEIGHT)
	{
		float _temp_pos=0;
		for(std::vector<geWindow*>::iterator it = childWindowList.begin(); it != childWindowList.end(); ++it)
		{
			geWindow* wnd = *it;
			float start_x=_temp_pos;
			if(activeWindow==wnd)
			{
				_temp_pos+=31;	//30+1 for rounded rectangle
			}
			else
			{
				_temp_pos+=20;
			}

			_temp_pos+=(wnd->getTitleWidth());

			float end_x=_temp_pos;
			if(x>start_x && x<end_x)
			{
				if(activeWindow!=wnd)
				{
					setActiveWindow(wnd);
				}
			}
		}
	}

	mousePreviousPos.set(x, y);
	return false;
}

bool geLayout::onMouseLButtonUp(float x, float y, int nFlag)
{
	mousePreviousPos.set(x, y);
	return true;
}

bool geLayout::onMouseMove(float x, float y, int flag)
{
	mousePreviousPos.set(x, y);
	return false;
}

void geLayout::traverseMouseMoveEvent(int x, int y, int flag)
{
	if(parentLayout!=NULL /*&& isPointInsideWindow(x, y)*/)
	{
		MouseMove(x, y, flag);
	}

	for(std::vector<geLayout*>::iterator it = childTopLayoutList.begin(); it != childTopLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->traverseMouseMoveEvent(x, y, flag);
	}

	for(std::vector<geLayout*>::iterator it = childBottomLayoutList.begin(); it != childBottomLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->traverseMouseMoveEvent(x, y, flag);
	}

	for(std::vector<geLayout*>::iterator it = childRightLayoutList.begin(); it != childRightLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->traverseMouseMoveEvent(x, y, flag);
	}

	for(std::vector<geLayout*>::iterator it = childLeftLayoutList.begin(); it != childLeftLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->traverseMouseMoveEvent(x, y, flag);
	}
}

void geLayout::setActiveWindow(int index)
{
	setActiveWindow(childWindowList[index]);
}

void geLayout::setActiveWindow(geWindow* wnd)
{
	activeWindow=wnd;
	setActiveWindowPtrOnlyForLayout(activeWindow);
}

geLayout* geLayout::selectLayout(int x, int y)
{
	geLayout* selectedLayout=NULL;

	if(parentLayout!=NULL && isPointInsideWindow(x, y))
	{
		return this;
	}

	for(std::vector<geLayout*>::iterator it = childTopLayoutList.begin(); it != childTopLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		selectedLayout=obj->selectLayout(x, y);
		if(selectedLayout)
			return selectedLayout;
	}

	for(std::vector<geLayout*>::iterator it = childBottomLayoutList.begin(); it != childBottomLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		selectedLayout=obj->selectLayout(x, y);
		if(selectedLayout)
			return selectedLayout;
	}

	for(std::vector<geLayout*>::iterator it = childRightLayoutList.begin(); it != childRightLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		selectedLayout=obj->selectLayout(x, y);
		if(selectedLayout)
			return selectedLayout;
	}

	for(std::vector<geLayout*>::iterator it = childLeftLayoutList.begin(); it != childLeftLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		selectedLayout=obj->selectLayout(x, y);
		if(selectedLayout)
			return selectedLayout;
	}
	return NULL;
}

void appendToList(geLayout* layout, std::vector<geLayout*>* vList)
{
	if(std::find(vList->begin(), vList->end(), layout)==vList->end())
	{
		vList->push_back(layout);
	}
}

void geLayout::getResizableOnLeftSide(int x, int y, std::vector<geLayout*>* vList)
{
	if(x>m_cPos.x+m_cSize.x-(BORDER_LAYOUT_OFFSET<<2) && x<m_cPos.x+m_cSize.x+(BORDER_LAYOUT_OFFSET<<2)/**/)
		appendToList(this, vList);

	for(std::vector<geLayout*>::iterator it = childTopLayoutList.begin(); it != childTopLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnLeftSide(x, y, vList);
	}

	for(std::vector<geLayout*>::iterator it = childBottomLayoutList.begin(); it != childBottomLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnLeftSide(x, y, vList);
	}

	for(std::vector<geLayout*>::iterator it = childRightLayoutList.begin(); it != childRightLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnLeftSide(x, y, vList);
	}

	for(std::vector<geLayout*>::iterator it = childLeftLayoutList.begin(); it != childLeftLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnLeftSide(x, y, vList);
	}
}

void geLayout::getResizableOnRightSide(int x, int y, std::vector<geLayout*>* vList)
{
	if(/**/x>m_cPos.x-(BORDER_LAYOUT_OFFSET<<2) && x<m_cPos.x+(BORDER_LAYOUT_OFFSET<<2))
		appendToList(this, vList);

	for(std::vector<geLayout*>::iterator it = childTopLayoutList.begin(); it != childTopLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnRightSide(x, y, vList);
	}

	for(std::vector<geLayout*>::iterator it = childBottomLayoutList.begin(); it != childBottomLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnRightSide(x, y, vList);
	}

	for(std::vector<geLayout*>::iterator it = childRightLayoutList.begin(); it != childRightLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnRightSide(x, y, vList);
	}

	for(std::vector<geLayout*>::iterator it = childLeftLayoutList.begin(); it != childLeftLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnRightSide(x, y, vList);
	}
}

void geLayout::getResizableOnTopSide(int x, int y, std::vector<geLayout*>* vList)
{
	if(y>m_cPos.y+m_cSize.y-(BORDER_LAYOUT_OFFSET<<2) && y<m_cPos.y+m_cSize.y+(BORDER_LAYOUT_OFFSET<<2)/**/)
		appendToList(this, vList);

	for(std::vector<geLayout*>::iterator it = childTopLayoutList.begin(); it != childTopLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnTopSide(x, y, vList);
	}

	for(std::vector<geLayout*>::iterator it = childBottomLayoutList.begin(); it != childBottomLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnTopSide(x, y, vList);
	}

	for(std::vector<geLayout*>::iterator it = childRightLayoutList.begin(); it != childRightLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnTopSide(x, y, vList);
	}

	for(std::vector<geLayout*>::iterator it = childLeftLayoutList.begin(); it != childLeftLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnTopSide(x, y, vList);
	}
}

void geLayout::getResizableOnBottomSide(int x, int y, std::vector<geLayout*>* vList)
{
	if(y>m_cPos.y-(BORDER_LAYOUT_OFFSET<<2) && y<m_cPos.y+(BORDER_LAYOUT_OFFSET<<2)/**/)
		appendToList(this, vList);

	for(std::vector<geLayout*>::iterator it = childTopLayoutList.begin(); it != childTopLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnBottomSide(x, y, vList);
	}

	for(std::vector<geLayout*>::iterator it = childBottomLayoutList.begin(); it != childBottomLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnBottomSide(x, y, vList);
	}

	for(std::vector<geLayout*>::iterator it = childRightLayoutList.begin(); it != childRightLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnBottomSide(x, y, vList);
	}

	for(std::vector<geLayout*>::iterator it = childLeftLayoutList.begin(); it != childLeftLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnBottomSide(x, y, vList);
	}
}

geLayout* geLayout::checkResizableOnLeftSide(int x, int y)
{
	if(x>m_cPos.x-(BORDER_LAYOUT_OFFSET<<2) && x<m_cPos.x+(BORDER_LAYOUT_OFFSET<<2) && y>m_cPos.y && y<m_cPos.y+m_cSize.y)
		return this;

	for(std::vector<geLayout*>::iterator it = childTopLayoutList.begin(); it != childTopLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		geLayout* retVal = obj->checkResizableOnLeftSide(x, y);
		if(retVal)
			return retVal;
	}

	for(std::vector<geLayout*>::iterator it = childBottomLayoutList.begin(); it != childBottomLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		geLayout* retVal = obj->checkResizableOnLeftSide(x, y);
		if(retVal)
			return retVal;
	}

	for(std::vector<geLayout*>::iterator it = childRightLayoutList.begin(); it != childRightLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		geLayout* retVal = obj->checkResizableOnLeftSide(x, y);
		if(retVal)
			return retVal;
	}

	for(std::vector<geLayout*>::iterator it = childLeftLayoutList.begin(); it != childLeftLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		geLayout* retVal = obj->checkResizableOnLeftSide(x, y);
		if(retVal)
			return retVal;
	}

	return NULL;
}

geLayout* geLayout::checkResizableOnRightSide(int x, int y)
{
	if(x>m_cPos.x+m_cSize.x-(BORDER_LAYOUT_OFFSET<<2) && x<m_cPos.x+m_cSize.x+(BORDER_LAYOUT_OFFSET<<2) && y>m_cPos.y && y<m_cPos.y+m_cSize.y)
		return this;

	for(std::vector<geLayout*>::iterator it = childTopLayoutList.begin(); it != childTopLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		geLayout* retVal = obj->checkResizableOnRightSide(x, y);
		if(retVal)
			return retVal;
	}

	for(std::vector<geLayout*>::iterator it = childBottomLayoutList.begin(); it != childBottomLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		geLayout* retVal = obj->checkResizableOnRightSide(x, y);
		if(retVal)
			return retVal;
	}

	for(std::vector<geLayout*>::iterator it = childRightLayoutList.begin(); it != childRightLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		geLayout* retVal = obj->checkResizableOnRightSide(x, y);
		if(retVal)
			return retVal;
	}

	for(std::vector<geLayout*>::iterator it = childLeftLayoutList.begin(); it != childLeftLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		geLayout* retVal = obj->checkResizableOnRightSide(x, y);
		if(retVal)
			return retVal;
	}

	return NULL;
}

geLayout* geLayout::checkResizableOnTopSide(int x, int y)
{
	if(x>m_cPos.x && x<m_cPos.x+m_cSize.x && y>m_cPos.y-(BORDER_LAYOUT_OFFSET<<2) && y<m_cPos.y+(BORDER_LAYOUT_OFFSET<<2))
		return this;

	for(std::vector<geLayout*>::iterator it = childTopLayoutList.begin(); it != childTopLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		geLayout* retVal = obj->checkResizableOnTopSide(x, y);
		if(retVal)
			return retVal;
	}

	for(std::vector<geLayout*>::iterator it = childBottomLayoutList.begin(); it != childBottomLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		geLayout* retVal = obj->checkResizableOnTopSide(x, y);
		if(retVal)
			return retVal;
	}

	for(std::vector<geLayout*>::iterator it = childRightLayoutList.begin(); it != childRightLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		geLayout* retVal = obj->checkResizableOnTopSide(x, y);
		if(retVal)
			return retVal;
	}

	for(std::vector<geLayout*>::iterator it = childLeftLayoutList.begin(); it != childLeftLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		geLayout* retVal = obj->checkResizableOnTopSide(x, y);
		if(retVal)
			return retVal;
	}

	return NULL;
}

geLayout* geLayout::checkResizableOnBottomSide(int x, int y)
{
	if(x>m_cPos.x && x<m_cPos.x+m_cSize.x && y>m_cPos.y+m_cSize.y-(BORDER_LAYOUT_OFFSET<<2) && y<m_cPos.y+m_cSize.y+(BORDER_LAYOUT_OFFSET<<2))
		return this;

	for(std::vector<geLayout*>::iterator it = childTopLayoutList.begin(); it != childTopLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		geLayout* retVal = obj->checkResizableOnBottomSide(x, y);
		if(retVal)
			return retVal;
	}

	for(std::vector<geLayout*>::iterator it = childBottomLayoutList.begin(); it != childBottomLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		geLayout* retVal = obj->checkResizableOnBottomSide(x, y);
		if(retVal)
			return retVal;
	}

	for(std::vector<geLayout*>::iterator it = childRightLayoutList.begin(); it != childRightLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		geLayout* retVal = obj->checkResizableOnBottomSide(x, y);
		if(retVal)
			return retVal;
	}

	for(std::vector<geLayout*>::iterator it = childLeftLayoutList.begin(); it != childLeftLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		geLayout* retVal = obj->checkResizableOnBottomSide(x, y);
		if(retVal)
			return retVal;
	}

	return NULL;
}

void geLayout::onMouseEnterClientArea()
{
}

void geLayout::onMouseExitClientArea()
{
	//CancelEngagedControls();
}

void geLayout::onCancelEngagedControls()
{
	activeWindow->CancelEngagedControls();
}