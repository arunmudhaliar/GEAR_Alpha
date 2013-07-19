#include "geLayoutManager.h"
#include "../win32/cursorUtil.h"
#include <assert.h>

geLayoutManager::geLayoutManager():
	geGUIBase(GEGUI_LAYOUT_MANAGER, "LayoutManager")
{
	m_pRootLayout=NULL;
	m_pSelectedLayout=NULL;
}

geLayoutManager:: ~geLayoutManager()
{
	m_vLeftResizeList.clear();
	m_vRightResizeList.clear();

	m_vTopResizeList.clear();
	m_vBottomResizeList.clear();

	GE_DELETE(m_pRootLayout);
}

void geLayoutManager::create(float x, float y, float cx, float cy)
{
	m_cPrevScale.set(1.0f, 1.0f);
	setPos(x, y);
	setSize(cx, cy);

	m_pRootLayout = new geLayout();
	m_pRootLayout->create(NULL, x, y, cx, cy);
	m_pRootLayout->setLayoutDirection(geLayout::LAYOUT_PARENT);

	cursorUtil::init();
}

void geLayoutManager::draw()
{
	if(m_pRootLayout)
		m_pRootLayout->draw();
}

void geLayoutManager::onPosition(float x, float y, int flag)
{
}

void geLayoutManager::onSize(float cx, float cy, int flag)
{
	if(m_pRootLayout)
	{
		m_pRootLayout->resize(cx/m_cPrevScale.x, cy/m_cPrevScale.y);
	}
	m_cPrevScale.set(cx, cy);
}

void geLayoutManager::mouseMoveWithGrabbedWindow(int x, int y, geWindow* grabbedWindow)
{
}

void geLayoutManager::dropGrabbedWindow(int x, int y, geWindow* grabbedWindow)
{
	if(m_pRootLayout)
	{
		geLayout* layouttodrop=m_pRootLayout->dropWindowOnMe(grabbedWindow);
	}
}

bool geLayoutManager::onMouseLButtonDown(float x, float y, int nFlag)
{
	cursorUtil::changeCursor(0);
	geLayout* layoutToResize=NULL;
	if(layoutToResize=m_pRootLayout->checkResizableOnRightSide(x, y))
	{
		m_pRootLayout->getResizableOnLeftSide(x, y, &m_vLeftResizeList);
		m_pRootLayout->getResizableOnRightSide(x, y, &m_vRightResizeList);
		cursorUtil::changeCursor(2);
	}
	else if(layoutToResize=m_pRootLayout->checkResizableOnLeftSide(x, y))
	{
		m_pRootLayout->getResizableOnLeftSide(x, y, &m_vLeftResizeList);
		m_pRootLayout->getResizableOnRightSide(x, y, &m_vRightResizeList);
		cursorUtil::changeCursor(2);
	}
	else if(layoutToResize=m_pRootLayout->checkResizableOnTopSide(x, y))
	{
		m_pRootLayout->getResizableOnTopSide(x, y, &m_vTopResizeList);
		m_pRootLayout->getResizableOnBottomSide(x, y, &m_vBottomResizeList);
		cursorUtil::changeCursor(1);
	}
	else if(layoutToResize=m_pRootLayout->checkResizableOnBottomSide(x, y))
	{
		m_pRootLayout->getResizableOnTopSide(x, y, &m_vTopResizeList);
		m_pRootLayout->getResizableOnBottomSide(x, y, &m_vBottomResizeList);
		cursorUtil::changeCursor(1);
	}
	else
	{
		geLayout* selectedLayout = m_pRootLayout->selectLayout(x, y);
		if(selectedLayout!=m_pSelectedLayout)
		{
			if(m_pSelectedLayout)
				m_pSelectedLayout->focusLost();
		}
		m_pSelectedLayout=selectedLayout;
		if(m_pSelectedLayout)
			m_pSelectedLayout->MouseLButtonDown(x, y, nFlag);
	}

	m_cMousePreviousPos.set(x, y);

	return false;
}

bool geLayoutManager::onMouseLButtonUp(float x, float y, int nFlag)
{
	if(m_pSelectedLayout)
	{
		m_pSelectedLayout->MouseLButtonUp(x, y, nFlag);
		if(!m_pSelectedLayout->isPointInsideWindow(x, y))
			m_pSelectedLayout->CancelEngagedControls();
	}

	for(std::vector<geLayout*>::iterator it = m_vRightResizeList.begin(); it != m_vRightResizeList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->resizeComplete();
	}

	for(std::vector<geLayout*>::iterator it = m_vLeftResizeList.begin(); it != m_vLeftResizeList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->resizeComplete();
	}

	for(std::vector<geLayout*>::iterator it = m_vTopResizeList.begin(); it != m_vTopResizeList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->resizeComplete();
	}

	for(std::vector<geLayout*>::iterator it = m_vBottomResizeList.begin(); it != m_vBottomResizeList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->resizeComplete();
	}

	m_vLeftResizeList.clear();
	m_vRightResizeList.clear();

	m_vTopResizeList.clear();
	m_vBottomResizeList.clear();

	m_cMousePreviousPos.set(x, y);

	return true;
}


bool geLayoutManager::onMouseRButtonDown(float x, float y, int nFlag)
{
	return false;
}

void geLayoutManager::onMouseRButtonUp(float x, float y, int nFlag)
{
}

void geLayoutManager::onMouseMove(float x, float y, int flag)
{
	bool bLayoutChangeLogicIssued=false;
	if((flag&0x0001)==0)	//MK_LBUTTON
	{
		cursorUtil::changeCursor(0);
		geLayout* layoutToResize;
		layoutToResize=m_pRootLayout->checkResizableOnRightSide(x, y);
		if(layoutToResize)
		{
			cursorUtil::changeCursor(2);
		}
		layoutToResize=m_pRootLayout->checkResizableOnLeftSide(x, y);
		if(layoutToResize)
		{
			cursorUtil::changeCursor(2);
		}
		layoutToResize=m_pRootLayout->checkResizableOnTopSide(x, y);
		if(layoutToResize)
		{
			cursorUtil::changeCursor(1);
		}
		layoutToResize=m_pRootLayout->checkResizableOnBottomSide(x, y);
		if(layoutToResize)
		{
			cursorUtil::changeCursor(1);
		}
	}
	else
	{
		if(m_vRightResizeList.size() && m_vLeftResizeList.size())
		{
			bLayoutChangeLogicIssued=true;
			expandOrContractLeftAndRightLayoutsOnMouseDrag(x, y);
		}
		else if(m_vTopResizeList.size() && m_vBottomResizeList.size())
		{
			bLayoutChangeLogicIssued=true;
			expandOrContractTopAndBottomLayoutsOnMouseDrag(x, y);
		}
	}

	if(!bLayoutChangeLogicIssued)
	{
		if(m_pSelectedLayout)
			m_pSelectedLayout->MouseMove(x, y, flag);

		//m_pRootLayout->traverseMouseMoveEvent(x, y, flag);
	}

	m_cMousePreviousPos.set(x, y);
}

void geLayoutManager::onMouseWheel(int zDelta, int x, int y, int flag)
{
	if(m_pSelectedLayout)
		m_pSelectedLayout->MouseWheel(zDelta, x, y, flag);
}

void geLayoutManager::expandOrContractLeftAndRightLayoutsOnMouseDrag(int x, int y)
{
	geVector2i diff(geVector2i(x, y)-m_cMousePreviousPos);

	bool bStop=false;
	bool bRightListExpand=true;
	bool bLeftListExpand=true;

	//check any layout resist expansion
	for(std::vector<geLayout*>::iterator it = m_vRightResizeList.begin(); it != m_vRightResizeList.end(); ++it)
	{
		geLayout* obj = *it;
		if(obj->getSize().x-diff.x<=20)
		{
			bRightListExpand=false;
			break;
		}
	}

	for(std::vector<geLayout*>::iterator it = m_vLeftResizeList.begin(); it != m_vLeftResizeList.end(); ++it)
	{
		geLayout* obj = *it;
		if(obj->getSize().x+diff.x<=20)
		{
			bLeftListExpand=false;
			break;
		}
	}

	if((!bRightListExpand && diff.x<0 || bRightListExpand) && (!bLeftListExpand && diff.x>0 || bLeftListExpand))
	for(std::vector<geLayout*>::iterator it = m_vRightResizeList.begin(); it != m_vRightResizeList.end(); ++it)
	{
		geLayout* obj = *it;
		geVector2f layout_pos(obj->getPos());
		geVector2f layout_sz(obj->getSize());
		obj->setPos(layout_pos.x+diff.x, layout_pos.y);
		obj->setSize(layout_sz.x-diff.x, layout_sz.y);
	}

	if(!bStop && (!bRightListExpand && diff.x<0 || bRightListExpand) && (!bLeftListExpand && diff.x>0 || bLeftListExpand))
	{
		for(std::vector<geLayout*>::iterator it = m_vLeftResizeList.begin(); it != m_vLeftResizeList.end(); ++it)
		{
			geLayout* obj = *it;
			geVector2f layout_sz(obj->getSize());
			obj->setSize(layout_sz.x+diff.x, layout_sz.y);
		}
	}
}

void geLayoutManager::expandOrContractTopAndBottomLayoutsOnMouseDrag(int x, int y)
{
	geVector2i diff(geVector2i(x, y)-m_cMousePreviousPos);

	bool bStop=false;
	bool bRightListExpand=true;
	bool bLeftListExpand=true;

	//check any layout resist expansion
	for(std::vector<geLayout*>::iterator it = m_vBottomResizeList.begin(); it != m_vBottomResizeList.end(); ++it)
	{
		geLayout* obj = *it;
		if(obj->getSize().y-diff.y<=20)
		{
			bRightListExpand=false;
			break;
		}
	}

	for(std::vector<geLayout*>::iterator it = m_vTopResizeList.begin(); it != m_vTopResizeList.end(); ++it)
	{
		geLayout* obj = *it;
		if(obj->getSize().y+diff.y<=20)
		{
			bLeftListExpand=false;
			break;
		}
	}

	if((!bRightListExpand && diff.y<0 || bRightListExpand) && (!bLeftListExpand && diff.y>0 || bLeftListExpand))
	for(std::vector<geLayout*>::iterator it = m_vBottomResizeList.begin(); it != m_vBottomResizeList.end(); ++it)
	{
		geLayout* obj = *it;
		geVector2f layout_pos(obj->getPos());
		geVector2f layout_sz(obj->getSize());
		obj->setPos(layout_pos.x, layout_pos.y+diff.y);
		obj->setSize(layout_sz.x, layout_sz.y-diff.y);
	}

	if(!bStop && (!bRightListExpand && diff.y<0 || bRightListExpand) && (!bLeftListExpand && diff.y>0 || bLeftListExpand))
	{
		for(std::vector<geLayout*>::iterator it = m_vTopResizeList.begin(); it != m_vTopResizeList.end(); ++it)
		{
			geLayout* obj = *it;
			geVector2f layout_sz(obj->getSize());
			obj->setSize(layout_sz.x, layout_sz.y+diff.y);
		}
	}
}

void geLayoutManager::onDragEnter(int x, int y)
{
	geLayout* selectedLayout = m_pRootLayout->selectLayout(x, y);
	if(selectedLayout)
		selectedLayout->DragEnter(x, y);
}

void geLayoutManager::onDragDrop(int x, int y, MDataObject* dropObject)
{
	geLayout* selectedLayout = m_pRootLayout->selectLayout(x, y);
	if(selectedLayout)
		selectedLayout->DragDrop(x, y, dropObject);
}

void geLayoutManager::onDragLeave()
{
	//there is only one window. so practically no dragLeave on logical windows
}

bool geLayoutManager::onKeyDown(int charValue, int flag)
{
	if(m_pSelectedLayout)
		return m_pSelectedLayout->KeyDown(charValue, flag);

	return false;
}

bool geLayoutManager::onKeyUp(int charValue, int flag)
{
	if(m_pSelectedLayout)
		return m_pSelectedLayout->KeyUp(charValue, flag);

	return false;
}