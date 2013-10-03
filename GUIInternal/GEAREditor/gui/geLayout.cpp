#include "geLayout.h"

geLayout::geLayout(const char* name):
	geGUIBase(GEGUI_LAYOUT, name)
{
}

geLayout:: ~geLayout()
{
	for(std::vector<geLayout*>::iterator it = m_vChildTopLayouts.begin(); it != m_vChildTopLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		GE_DELETE(obj);
	}
	m_vChildTopLayouts.clear();

	for(std::vector<geLayout*>::iterator it = m_vChildBottomLayouts.begin(); it != m_vChildBottomLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		GE_DELETE(obj);
	}
	m_vChildBottomLayouts.clear();

	for(std::vector<geLayout*>::iterator it = m_vChildRightLayouts.begin(); it != m_vChildRightLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		GE_DELETE(obj);
	}
	m_vChildRightLayouts.clear();

	for(std::vector<geLayout*>::iterator it = m_vChildLeftLayouts.begin(); it != m_vChildLeftLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		GE_DELETE(obj);
	}
	m_vChildLeftLayouts.clear();
}

void geLayout::create(geLayout* pParentLayout, float x, float y, float cx, float cy)
{
	m_pParentLayout=pParentLayout;
	m_pWindowPointer = NULL;
	setPos(x, y);
	setSize(cx, cy);
	setColor(&m_cVBClientArea, 1.0f, 0.09f, 0.09f, 1.0f);
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
	m_vChildLeftLayouts.push_back(childLayout);
}

void geLayout::appendRightChildLayout(geLayout* childLayout)
{
	childLayout->setParentLayout(this);
	m_vChildRightLayouts.push_back(childLayout);
}

void geLayout::appendTopChildLayout(geLayout* childLayout)
{
	childLayout->setParentLayout(this);
	m_vChildTopLayouts.push_back(childLayout);
}

void geLayout::appendBottomChildLayout(geLayout* childLayout)
{
	childLayout->setParentLayout(this);
	m_vChildBottomLayouts.push_back(childLayout);
}

void geLayout::setWindow(geWindow* window)
{
	m_pWindowPointer=window;
	m_pWindowPointer->setIamOnLayout(this);
	m_pWindowPointer->setParent(this);
	//m_pWindowPointer->setParent(this);
	//reposition the window
	m_pWindowPointer->setPos(BORDER_LAYOUT_OFFSET+LEFT_LAYOUT_MARGIN, BORDER_LAYOUT_OFFSET);

	//resize the window
	m_pWindowPointer->setSize(getSize().x-(BORDER_LAYOUT_OFFSET<<1), getSize().y-((BORDER_LAYOUT_OFFSET<<1)+LEFT_LAYOUT_MARGIN));

	m_vChildWindows.push_back(window);
}

void geLayout::draw()
{
	if(m_pParentLayout!=NULL)
	{
		glViewport(m_cPos.x+(BORDER_LAYOUT_OFFSET>>1), rendererGL10::g_pRendererGL10->getViewPortSz().y-(m_cSize.y+m_cPos.y+(BORDER_LAYOUT_OFFSET>>1)), m_cSize.x-(BORDER_LAYOUT_OFFSET), m_cSize.y-(BORDER_LAYOUT_OFFSET));	
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0.0f, (int)(m_cSize.x-(BORDER_LAYOUT_OFFSET)), (int)(m_cSize.y-(BORDER_LAYOUT_OFFSET)), 0.0f);
		glMatrixMode(GL_MODELVIEW);
		drawLine(m_cVBClientAreaLine, 0.13f, 0.13f, 0.13f, 1.0f, 3, false); 

		m_pWindowPointer->draw();
	}

	//draw childs
	for(std::vector<geLayout*>::iterator it = m_vChildTopLayouts.begin(); it != m_vChildTopLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		obj->draw();
	}
	for(std::vector<geLayout*>::iterator it = m_vChildBottomLayouts.begin(); it != m_vChildBottomLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		obj->draw();
	}
	for(std::vector<geLayout*>::iterator it = m_vChildRightLayouts.begin(); it != m_vChildRightLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		obj->draw();
	}
	for(std::vector<geLayout*>::iterator it = m_vChildLeftLayouts.begin(); it != m_vChildLeftLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		obj->draw();
	}
}

void geLayout::onPosition(float x, float y, int flag)
{
	if(m_pWindowPointer)
	{
		//reposition the window
		m_pWindowPointer->setPos(BORDER_LAYOUT_OFFSET+LEFT_LAYOUT_MARGIN, BORDER_LAYOUT_OFFSET);

		//resize the window
		//m_pWindowPointer->setSize(getSize().x-(BORDER_LAYOUT_OFFSET<<1), getSize().y-(BORDER_LAYOUT_OFFSET<<1));
	}
}

void geLayout::resize(double xScale, double yScale)
{
	setPos(m_cPos.x*xScale, m_cPos.y*yScale);
	setSize(m_cSize.x*xScale, m_cSize.y*yScale);
	resizeComplete();

	for(std::vector<geLayout*>::iterator it = m_vChildTopLayouts.begin(); it != m_vChildTopLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		obj->resize(xScale, yScale);
	}

	for(std::vector<geLayout*>::iterator it = m_vChildBottomLayouts.begin(); it != m_vChildBottomLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		obj->resize(xScale, yScale);
	}

	for(std::vector<geLayout*>::iterator it = m_vChildRightLayouts.begin(); it != m_vChildRightLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		obj->resize(xScale, yScale);
	}

	for(std::vector<geLayout*>::iterator it = m_vChildLeftLayouts.begin(); it != m_vChildLeftLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		obj->resize(xScale, yScale);
	}
}

geLayout* geLayout::dropWindowOnMe(geWindow* window)
{
	if(m_pParentLayout!=NULL /*&& isPointInsideWindow(window->getPos().x, window->getPos().y)*/)
	{
		if(m_pWindowPointer==window)
		{
			//reposition the window
			m_pWindowPointer->setPos(BORDER_LAYOUT_OFFSET+LEFT_LAYOUT_MARGIN, BORDER_LAYOUT_OFFSET);

			////resize the window
			//m_pWindowPointer->setSize(getSize().x-(BORDER_LAYOUT_OFFSET<<1), getSize().y-(BORDER_LAYOUT_OFFSET<<1));
			//m_pWindowPointer->setSize(getSize().x-(BORDER_LAYOUT_OFFSET<<1), m_pWindowPointer->getSize().y);
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

	const float title_vertLst[8] =
	{
		cx,	0,
		0,	0,
		cx,	cy,
		0,	cy,
	};
	memcpy(m_cVBClientArea.m_cszVertexList, title_vertLst, sizeof(title_vertLst));


	const float clientarea_linevertLst[6] =
	{
		cx,	1,
		1,	1,
		1,	cy
	};
	memcpy(m_cVBClientAreaLine, clientarea_linevertLst, sizeof(clientarea_linevertLst));

	if(m_pWindowPointer)
	{
		//reposition the window
		m_pWindowPointer->setPos(BORDER_LAYOUT_OFFSET+LEFT_LAYOUT_MARGIN, BORDER_LAYOUT_OFFSET);

		////resize the window
		m_pWindowPointer->setSize(getSize().x-(BORDER_LAYOUT_OFFSET<<1), getSize().y-((BORDER_LAYOUT_OFFSET<<1)+LEFT_LAYOUT_MARGIN));

	}
}

bool geLayout::removeChildLayout(geLayout* childLayout)
{
	m_vChildTopLayouts.erase(std::remove(m_vChildTopLayouts.begin(), m_vChildTopLayouts.end(), childLayout), m_vChildTopLayouts.end());
	m_vChildBottomLayouts.erase(std::remove(m_vChildBottomLayouts.begin(), m_vChildBottomLayouts.end(), childLayout), m_vChildBottomLayouts.end());
	m_vChildRightLayouts.erase(std::remove(m_vChildRightLayouts.begin(), m_vChildRightLayouts.end(), childLayout), m_vChildRightLayouts.end());
	m_vChildLeftLayouts.erase(std::remove(m_vChildLeftLayouts.begin(), m_vChildLeftLayouts.end(), childLayout), m_vChildLeftLayouts.end());

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
	OutputDebugString("createLeft\n");

	geLayout* pParentOfNewLayout = this;
	//check if i am on the right side of my parent
	if(getLayoutDirection()==RIGHT_TO_PARENT)
	{
		pParentOfNewLayout=getRightMostParentLayout();
	}
	
	geLayout* pLayout = new geLayout();
	pLayout->create(pParentOfNewLayout, this->getPos().x, this->getPos().y, this->getSize().x*ratio,  this->getSize().y);
	pLayout->setLayoutDirection((pParentOfNewLayout==this)?LEFT_TO_PARENT:RIGHT_TO_PARENT);

	this->setPos(this->getPos().x+this->getSize().x*ratio, this->getPos().y);
	this->setSize(this->getSize().x*(1.0f-ratio),  this->getSize().y);

	if(pParentOfNewLayout==this)
		m_vChildLeftLayouts.push_back(pLayout);
	else
	{
		pParentOfNewLayout->appendRightChildLayout(pLayout);
		//remove me from the existing parent and add to child as new layout
		getParentLayout()->removeChildLayout(this);
		pLayout->appendLeftChildLayout(this);
	}

	pLayout->setWindow(window);

	return pLayout;
}

geLayout* geLayout::createRight(geWindow* window, float ratio)
{
	OutputDebugString("createRight\n");

	geLayout* pParentOfNewLayout = this;
	//check if i am on the right side of my parent
	if(getLayoutDirection()==LEFT_TO_PARENT)
	{
		pParentOfNewLayout=getLeftMostParentLayout();
	}

	geLayout* pLayout = new geLayout();
	pLayout->create(pParentOfNewLayout, this->getPos().x+this->getSize().x*(1.0f-ratio), this->getPos().y, this->getSize().x*ratio,  this->getSize().y);
	pLayout->setLayoutDirection((pParentOfNewLayout==this)?RIGHT_TO_PARENT:LEFT_TO_PARENT);

	this->setSize(this->getSize().x*(1.0f-ratio),  this->getSize().y);

	if(pParentOfNewLayout==this)
		m_vChildRightLayouts.push_back(pLayout);
	else
	{
		pParentOfNewLayout->appendLeftChildLayout(pLayout);
		//remove me from the existing parent and add to child as new layout
		getParentLayout()->removeChildLayout(this);
		pLayout->appendRightChildLayout(this);
	}

	pLayout->setWindow(window);

	return pLayout;
}

geLayout* geLayout::createTop(geWindow* window, float ratio)
{
	OutputDebugString("createTop\n");

	geLayout* pParentOfNewLayout = this;
	//check if i am on the right side of my parent
	if(getLayoutDirection()==BOTTOM_TO_PARENT)
	{
		pParentOfNewLayout=getBottomMostParentLayout();
	}

	geLayout* pLayout = new geLayout();
	pLayout->create(pParentOfNewLayout, this->getPos().x, this->getPos().y, this->getSize().x,  this->getSize().y*ratio);
	pLayout->setLayoutDirection((pParentOfNewLayout==this)?TOP_TO_PARENT:BOTTOM_TO_PARENT);

	this->setPos(this->getPos().x, this->getPos().y+this->getSize().y*ratio);
	this->setSize(this->getSize().x,  this->getSize().y*(1.0f-ratio));

	if(pParentOfNewLayout==this)
		m_vChildTopLayouts.push_back(pLayout);
	else
	{
		pParentOfNewLayout->appendBottomChildLayout(pLayout);
		//remove me from the existing parent and add to child as new layout
		getParentLayout()->removeChildLayout(this);
		pLayout->appendTopChildLayout(this);
	}

	pLayout->setWindow(window);

	return pLayout;
}

geLayout* geLayout::createBottom(geWindow* window, float ratio)
{
	OutputDebugString("createBottom\n");

	geLayout* pParentOfNewLayout = this;
	//check if i am on the right side of my parent
	if(getLayoutDirection()==TOP_TO_PARENT)
	{
		pParentOfNewLayout=getTopMostParentLayout();
	}

	geLayout* pLayout = new geLayout();
	pLayout->create(pParentOfNewLayout, this->getPos().x, this->getPos().y+this->getSize().y*(1.0f-ratio), this->getSize().x,  this->getSize().y*ratio);
	pLayout->setLayoutDirection((pParentOfNewLayout==this)?BOTTOM_TO_PARENT:TOP_TO_PARENT);

	this->setSize(this->getSize().x,  this->getSize().y*(1.0f-ratio));

	if(pParentOfNewLayout==this)
		m_vChildBottomLayouts.push_back(pLayout);
	else
	{
		pParentOfNewLayout->appendTopChildLayout(pLayout);
		//remove me from the existing parent and add to child as new layout
		getParentLayout()->removeChildLayout(this);
		pLayout->appendBottomChildLayout(this);
	}

	pLayout->setWindow(window);

	return pLayout;
}

geLayout* geLayout::createAsParent(geWindow* window)
{
	geLayout* pLayout = new geLayout();
	pLayout->create(this, this->getPos().x, this->getPos().y, this->getSize().x,  this->getSize().y);
	pLayout->setLayoutDirection(LAYOUT_PARENT);
	this->setSize(this->getSize().x,  this->getSize().y);
	appendLeftChildLayout(pLayout);
	pLayout->setWindow(window);

	return pLayout;
}

bool geLayout::onMouseLButtonDown(float x, float y, int nFlag)
{
	//m_pWindowPointer->clearVarsAfterWindowMoved();
	//m_pWindowPointer->selectWindow(x-getPos().x, y-getPos().y);
	m_cMousePreviousPos.set(x, y);

	return false;
}

bool geLayout::onMouseLButtonUp(float x, float y, int nFlag)
{
	//if(m_pWindowPointer && m_pWindowPointer->isMovable())
	//{
	//	m_pWindowPointer->clearVarsAfterWindowMoved();
	//	dropWindowOnMe(m_pWindowPointer);
	//}

	m_cMousePreviousPos.set(x, y);

	return true;
}

bool geLayout::onMouseMove(float x, float y, int flag)
{
	//if(flag&0x0001)	//MK_LBUTTON
	//{
	//	if(m_pWindowPointer && m_pWindowPointer->isMovable())
	//	{
	//		geVector2f diff(geVector2f(x, y)-m_cMousePreviousPos);
	//		m_pWindowPointer->setPos(m_pWindowPointer->getPos()+diff);
	//	}
	//}

	m_cMousePreviousPos.set(x, y);

	return false;
}

void geLayout::traverseMouseMoveEvent(int x, int y, int flag)
{
	if(m_pParentLayout!=NULL /*&& isPointInsideWindow(x, y)*/)
	{
		MouseMove(x, y, flag);
	}

	for(std::vector<geLayout*>::iterator it = m_vChildTopLayouts.begin(); it != m_vChildTopLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		obj->traverseMouseMoveEvent(x, y, flag);
	}

	for(std::vector<geLayout*>::iterator it = m_vChildBottomLayouts.begin(); it != m_vChildBottomLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		obj->traverseMouseMoveEvent(x, y, flag);
	}

	for(std::vector<geLayout*>::iterator it = m_vChildRightLayouts.begin(); it != m_vChildRightLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		obj->traverseMouseMoveEvent(x, y, flag);
	}

	for(std::vector<geLayout*>::iterator it = m_vChildLeftLayouts.begin(); it != m_vChildLeftLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		obj->traverseMouseMoveEvent(x, y, flag);
	}
}

geLayout* geLayout::selectLayout(int x, int y)
{
	geLayout* selectedLayout=NULL;

	if(m_pParentLayout!=NULL && isPointInsideWindow(x, y))
	{
		return this;
	}

	for(std::vector<geLayout*>::iterator it = m_vChildTopLayouts.begin(); it != m_vChildTopLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		selectedLayout=obj->selectLayout(x, y);
		if(selectedLayout)
			return selectedLayout;
	}

	for(std::vector<geLayout*>::iterator it = m_vChildBottomLayouts.begin(); it != m_vChildBottomLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		selectedLayout=obj->selectLayout(x, y);
		if(selectedLayout)
			return selectedLayout;
	}

	for(std::vector<geLayout*>::iterator it = m_vChildRightLayouts.begin(); it != m_vChildRightLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		selectedLayout=obj->selectLayout(x, y);
		if(selectedLayout)
			return selectedLayout;
	}

	for(std::vector<geLayout*>::iterator it = m_vChildLeftLayouts.begin(); it != m_vChildLeftLayouts.end(); ++it)
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

	for(std::vector<geLayout*>::iterator it = m_vChildTopLayouts.begin(); it != m_vChildTopLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnLeftSide(x, y, vList);
	}

	for(std::vector<geLayout*>::iterator it = m_vChildBottomLayouts.begin(); it != m_vChildBottomLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnLeftSide(x, y, vList);
	}

	for(std::vector<geLayout*>::iterator it = m_vChildRightLayouts.begin(); it != m_vChildRightLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnLeftSide(x, y, vList);
	}

	for(std::vector<geLayout*>::iterator it = m_vChildLeftLayouts.begin(); it != m_vChildLeftLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnLeftSide(x, y, vList);
	}
}

void geLayout::getResizableOnRightSide(int x, int y, std::vector<geLayout*>* vList)
{
	if(/**/x>m_cPos.x-(BORDER_LAYOUT_OFFSET<<2) && x<m_cPos.x+(BORDER_LAYOUT_OFFSET<<2))
		appendToList(this, vList);

	for(std::vector<geLayout*>::iterator it = m_vChildTopLayouts.begin(); it != m_vChildTopLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnRightSide(x, y, vList);
	}

	for(std::vector<geLayout*>::iterator it = m_vChildBottomLayouts.begin(); it != m_vChildBottomLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnRightSide(x, y, vList);
	}

	for(std::vector<geLayout*>::iterator it = m_vChildRightLayouts.begin(); it != m_vChildRightLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnRightSide(x, y, vList);
	}

	for(std::vector<geLayout*>::iterator it = m_vChildLeftLayouts.begin(); it != m_vChildLeftLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnRightSide(x, y, vList);
	}
}

void geLayout::getResizableOnTopSide(int x, int y, std::vector<geLayout*>* vList)
{
	if(y>m_cPos.y+m_cSize.y-(BORDER_LAYOUT_OFFSET<<2) && y<m_cPos.y+m_cSize.y+(BORDER_LAYOUT_OFFSET<<2)/**/)
		appendToList(this, vList);

	for(std::vector<geLayout*>::iterator it = m_vChildTopLayouts.begin(); it != m_vChildTopLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnTopSide(x, y, vList);
	}

	for(std::vector<geLayout*>::iterator it = m_vChildBottomLayouts.begin(); it != m_vChildBottomLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnTopSide(x, y, vList);
	}

	for(std::vector<geLayout*>::iterator it = m_vChildRightLayouts.begin(); it != m_vChildRightLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnTopSide(x, y, vList);
	}

	for(std::vector<geLayout*>::iterator it = m_vChildLeftLayouts.begin(); it != m_vChildLeftLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnTopSide(x, y, vList);
	}
}

void geLayout::getResizableOnBottomSide(int x, int y, std::vector<geLayout*>* vList)
{
	if(y>m_cPos.y-(BORDER_LAYOUT_OFFSET<<2) && y<m_cPos.y+(BORDER_LAYOUT_OFFSET<<2)/**/)
		appendToList(this, vList);

	for(std::vector<geLayout*>::iterator it = m_vChildTopLayouts.begin(); it != m_vChildTopLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnBottomSide(x, y, vList);
	}

	for(std::vector<geLayout*>::iterator it = m_vChildBottomLayouts.begin(); it != m_vChildBottomLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnBottomSide(x, y, vList);
	}

	for(std::vector<geLayout*>::iterator it = m_vChildRightLayouts.begin(); it != m_vChildRightLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnBottomSide(x, y, vList);
	}

	for(std::vector<geLayout*>::iterator it = m_vChildLeftLayouts.begin(); it != m_vChildLeftLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		obj->getResizableOnBottomSide(x, y, vList);
	}
}

geLayout* geLayout::checkResizableOnLeftSide(int x, int y)
{
	if(x>m_cPos.x-(BORDER_LAYOUT_OFFSET<<2) && x<m_cPos.x+(BORDER_LAYOUT_OFFSET<<2) && y>m_cPos.y && y<m_cPos.y+m_cSize.y)
		return this;

	for(std::vector<geLayout*>::iterator it = m_vChildTopLayouts.begin(); it != m_vChildTopLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		geLayout* retVal = obj->checkResizableOnLeftSide(x, y);
		if(retVal)
			return retVal;
	}

	for(std::vector<geLayout*>::iterator it = m_vChildBottomLayouts.begin(); it != m_vChildBottomLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		geLayout* retVal = obj->checkResizableOnLeftSide(x, y);
		if(retVal)
			return retVal;
	}

	for(std::vector<geLayout*>::iterator it = m_vChildRightLayouts.begin(); it != m_vChildRightLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		geLayout* retVal = obj->checkResizableOnLeftSide(x, y);
		if(retVal)
			return retVal;
	}

	for(std::vector<geLayout*>::iterator it = m_vChildLeftLayouts.begin(); it != m_vChildLeftLayouts.end(); ++it)
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

	for(std::vector<geLayout*>::iterator it = m_vChildTopLayouts.begin(); it != m_vChildTopLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		geLayout* retVal = obj->checkResizableOnRightSide(x, y);
		if(retVal)
			return retVal;
	}

	for(std::vector<geLayout*>::iterator it = m_vChildBottomLayouts.begin(); it != m_vChildBottomLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		geLayout* retVal = obj->checkResizableOnRightSide(x, y);
		if(retVal)
			return retVal;
	}

	for(std::vector<geLayout*>::iterator it = m_vChildRightLayouts.begin(); it != m_vChildRightLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		geLayout* retVal = obj->checkResizableOnRightSide(x, y);
		if(retVal)
			return retVal;
	}

	for(std::vector<geLayout*>::iterator it = m_vChildLeftLayouts.begin(); it != m_vChildLeftLayouts.end(); ++it)
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

	for(std::vector<geLayout*>::iterator it = m_vChildTopLayouts.begin(); it != m_vChildTopLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		geLayout* retVal = obj->checkResizableOnTopSide(x, y);
		if(retVal)
			return retVal;
	}

	for(std::vector<geLayout*>::iterator it = m_vChildBottomLayouts.begin(); it != m_vChildBottomLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		geLayout* retVal = obj->checkResizableOnTopSide(x, y);
		if(retVal)
			return retVal;
	}

	for(std::vector<geLayout*>::iterator it = m_vChildRightLayouts.begin(); it != m_vChildRightLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		geLayout* retVal = obj->checkResizableOnTopSide(x, y);
		if(retVal)
			return retVal;
	}

	for(std::vector<geLayout*>::iterator it = m_vChildLeftLayouts.begin(); it != m_vChildLeftLayouts.end(); ++it)
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

	for(std::vector<geLayout*>::iterator it = m_vChildTopLayouts.begin(); it != m_vChildTopLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		geLayout* retVal = obj->checkResizableOnBottomSide(x, y);
		if(retVal)
			return retVal;
	}

	for(std::vector<geLayout*>::iterator it = m_vChildBottomLayouts.begin(); it != m_vChildBottomLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		geLayout* retVal = obj->checkResizableOnBottomSide(x, y);
		if(retVal)
			return retVal;
	}

	for(std::vector<geLayout*>::iterator it = m_vChildRightLayouts.begin(); it != m_vChildRightLayouts.end(); ++it)
	{
		geLayout* obj = *it;
		geLayout* retVal = obj->checkResizableOnBottomSide(x, y);
		if(retVal)
			return retVal;
	}

	for(std::vector<geLayout*>::iterator it = m_vChildLeftLayouts.begin(); it != m_vChildLeftLayouts.end(); ++it)
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
	m_pWindowPointer->CancelEngagedControls();
}