#include "geLayoutManager.h"
#include "../win32/cursorUtil.h"
#include <assert.h>

geLayoutManager::geLayoutManager(geFontManager* fontmanager):
	geGUIBase(GEGUI_LAYOUT_MANAGER, "LayoutManager", fontmanager)
{
	rootLayout=NULL;
	selectedLayout=NULL;
    is_SelectedLayoutGrabbed=false;
    putGrabbedLayoutInToMe=nullptr;
}

geLayoutManager:: ~geLayoutManager()
{
	leftResizeLayoutList.clear();
	rightResizeLayoutList.clear();

	topResizeLayoutList.clear();
	bottomResizeLayoutList.clear();

	GE_DELETE(rootLayout);
}

void geLayoutManager::create(rendererGL10* renderer, float x, float y, float cx, float cy)
{
	rendererGUI=renderer;
	previousScale.set(1.0f, 1.0f);
	setPos(x, y);
	setSize(cx, cy);

	rootLayout = new geLayout("layout", fontManagerGUI);
	rootLayout->create(renderer, NULL, x, y, cx, cy);
	rootLayout->setLayoutDirection(geLayout::LAYOUT_PARENT);

	cursorUtil::init();
}

void geLayoutManager::draw()
{
	if(rootLayout)
		rootLayout->draw();
}

void geLayoutManager::onPosition(float x, float y, int flag)
{
}

void geLayoutManager::onSize(float cx, float cy, int flag)
{
	if(rootLayout)
	{
		rootLayout->resize(cx/previousScale.x, cy/previousScale.y);
	}
	previousScale.set(cx, cy);
}

void geLayoutManager::mouseMoveWithGrabbedWindow(int x, int y, geWindow* grabbedWindow)
{
}

void geLayoutManager::dropGrabbedWindow(int x, int y, geWindow* grabbedWindow)
{
//	if(rootLayout)
//	{
//		geLayout* layouttodrop=rootLayout->dropWindowOnMe(grabbedWindow);
//	}
}

bool geLayoutManager::onMouseLButtonDown(float x, float y, int nFlag)
{
    is_SelectedLayoutGrabbed = false;
    putGrabbedLayoutInToMe = nullptr;
    
	cursorUtil::changeCursor(0);
	if(rootLayout->checkResizableOnRightSide(x, y))
	{
		rootLayout->getResizableOnLeftSide(x, y, &leftResizeLayoutList);
		rootLayout->getResizableOnRightSide(x, y, &rightResizeLayoutList);
        if(leftResizeLayoutList.size() && rightResizeLayoutList.size())
            cursorUtil::changeCursor(2);
	}
	else if(rootLayout->checkResizableOnLeftSide(x, y))
	{
		rootLayout->getResizableOnLeftSide(x, y, &leftResizeLayoutList);
		rootLayout->getResizableOnRightSide(x, y, &rightResizeLayoutList);
        if(leftResizeLayoutList.size() && rightResizeLayoutList.size())
            cursorUtil::changeCursor(2);
	}
	else if(rootLayout->checkResizableOnTopSide(x, y))
	{
		rootLayout->getResizableOnTopSide(x, y, &topResizeLayoutList);
		rootLayout->getResizableOnBottomSide(x, y, &bottomResizeLayoutList);
        if(topResizeLayoutList.size() && bottomResizeLayoutList.size())
            cursorUtil::changeCursor(1);
	}
	else if(rootLayout->checkResizableOnBottomSide(x, y))
	{
		rootLayout->getResizableOnTopSide(x, y, &topResizeLayoutList);
		rootLayout->getResizableOnBottomSide(x, y, &bottomResizeLayoutList);
        if(topResizeLayoutList.size() && bottomResizeLayoutList.size())
            cursorUtil::changeCursor(1);
	}
	else
	{
		geLayout* selectedLayout = rootLayout->selectLayout(x, y);
		if(this->selectedLayout!=selectedLayout)
		{
			if(this->selectedLayout)
				this->selectedLayout->focusLost();
		}
		this->selectedLayout=selectedLayout;
		if(this->selectedLayout)
        {
            bool unPinned = false;
            //check if we can unpin the selected layout
            bool handled = this->selectedLayout->MouseLButtonDown(x, y, nFlag);
            if(!handled && this->selectedLayout->isPinned())
            {
                bool deleteLayout=false;
                unPinned = this->selectedLayout->doUnPin(x, y, rootLayout, deleteLayout);
                if(deleteLayout)
                {
                    //delete the selected layout.
                    GE_DELETE(this->selectedLayout);
                }
            }
            
            if(!unPinned && this->selectedLayout)
            {
                //check the selected layout for grabbing
                is_SelectedLayoutGrabbed = this->selectedLayout->canGrab(x, y);
            }
        }
	}

	mousePreviousPos.set(x, y);

	return false;
}

bool geLayoutManager::onMouseLButtonUp(float x, float y, int nFlag)
{
	if(selectedLayout)
	{
		selectedLayout->MouseLButtonUp(x, y, nFlag);
		if(!selectedLayout->isPointInsideWindow(x, y))
			selectedLayout->CancelEngagedControls();
	}

	for(std::vector<geLayout*>::iterator it = rightResizeLayoutList.begin(); it != rightResizeLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->resizeComplete();
	}

	for(std::vector<geLayout*>::iterator it = leftResizeLayoutList.begin(); it != leftResizeLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->resizeComplete();
	}

	for(std::vector<geLayout*>::iterator it = topResizeLayoutList.begin(); it != topResizeLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->resizeComplete();
	}

	for(std::vector<geLayout*>::iterator it = bottomResizeLayoutList.begin(); it != bottomResizeLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		obj->resizeComplete();
	}

	leftResizeLayoutList.clear();
	rightResizeLayoutList.clear();

	topResizeLayoutList.clear();
	bottomResizeLayoutList.clear();

	mousePreviousPos.set(x, y);

    //check if any grabbing
    if(is_SelectedLayoutGrabbed)
    {
        //TODO: do recalculate layout.
        int whichArea=-1;
        putGrabbedLayoutInToMe = rootLayout->doGrabOverlapAreaCheck(selectedLayout, x, y, whichArea);
        if(putGrabbedLayoutInToMe)
        {
            this->selectedLayout->doUnPlug(rootLayout);
            geLayout::reAdjustLayoutOnPlug(putGrabbedLayoutInToMe, selectedLayout, 0.5f, whichArea);
            //this->selectedLayout->getParentLayout()->removeChildLayout(this->selectedLayout);
            
            putGrabbedLayoutInToMe = nullptr;
        }
    }
    
	return true;
}


bool geLayoutManager::onMouseRButtonDown(float x, float y, int nFlag)
{
	return false;
}

void geLayoutManager::onMouseRButtonUp(float x, float y, int nFlag)
{
}

bool geLayoutManager::onMouseMButtonDown(float x, float y, int nFlag)
{
    if(selectedLayout)
    {
        return selectedLayout->MouseMButtonDown(x, y, nFlag);
    }
    
    return false;
}

void geLayoutManager::onMouseMButtonUp(float x, float y, int nFlag)
{
    if(selectedLayout)
    {
        selectedLayout->MouseMButtonUp(x, y, nFlag);
    }
}

bool geLayoutManager::onMouseMove(float x, float y, int flag)
{
	bool bLayoutChangeLogicIssued=false;
	if((flag&0x0001)==0)	//MK_LBUTTON
	{
		cursorUtil::changeCursor(0);
		geLayout* layoutToResize;
		layoutToResize=rootLayout->checkResizableOnRightSide(x, y);
		if(layoutToResize)
		{
			cursorUtil::changeCursor(2);
		}
		layoutToResize=rootLayout->checkResizableOnLeftSide(x, y);
		if(layoutToResize)
		{
			cursorUtil::changeCursor(2);
		}
		layoutToResize=rootLayout->checkResizableOnTopSide(x, y);
		if(layoutToResize)
		{
			cursorUtil::changeCursor(1);
		}
		layoutToResize=rootLayout->checkResizableOnBottomSide(x, y);
		if(layoutToResize)
		{
			cursorUtil::changeCursor(1);
		}
	}
	else
	{
		if(rightResizeLayoutList.size() && leftResizeLayoutList.size())
		{
			bLayoutChangeLogicIssued=true;
			expandOrContractLeftAndRightLayoutsOnMouseDrag(x, y);
		}
		else if(topResizeLayoutList.size() && bottomResizeLayoutList.size())
		{
			bLayoutChangeLogicIssued=true;
			expandOrContractTopAndBottomLayoutsOnMouseDrag(x, y);
		}
	}

	if(!bLayoutChangeLogicIssued)
	{
        //check any grabbing
        if(is_SelectedLayoutGrabbed)
        {
            if(selectedLayout && ((flag&0x0001)>0))
            {
                int whichArea=-1;
                putGrabbedLayoutInToMe = rootLayout->doGrabOverlapAreaCheck(selectedLayout, x, y, whichArea);
            }
        }
        else
        {
            if(selectedLayout)
                selectedLayout->MouseMove(x, y, flag);
            //rootLayout->traverseMouseMoveEvent(x, y, flag);
        }
	}

	mousePreviousPos.set(x, y);

	return false;
}

void geLayoutManager::onMouseWheel(int zDelta, int x, int y, int flag)
{
	if(selectedLayout)
		selectedLayout->MouseWheel(zDelta, x, y, flag);
}

void geLayoutManager::expandOrContractLeftAndRightLayoutsOnMouseDrag(int x, int y)
{
	geVector2i diff(geVector2i(x, y)-mousePreviousPos);

	bool bStop=false;
	bool bRightListExpand=true;
	bool bLeftListExpand=true;

	//check any layout resist expansion
	for(std::vector<geLayout*>::iterator it = rightResizeLayoutList.begin(); it != rightResizeLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		if(obj->getSize().x-diff.x<=20)
		{
			bRightListExpand=false;
			break;
		}
	}

	for(std::vector<geLayout*>::iterator it = leftResizeLayoutList.begin(); it != leftResizeLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		if(obj->getSize().x+diff.x<=20)
		{
			bLeftListExpand=false;
			break;
		}
	}

	if(((!bRightListExpand && diff.x<0) || bRightListExpand) && ((!bLeftListExpand && diff.x>0) || bLeftListExpand))
	for(std::vector<geLayout*>::iterator it = rightResizeLayoutList.begin(); it != rightResizeLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		geVector2f layout_pos(obj->getPos());
		geVector2f layout_sz(obj->getSize());
		obj->setPos(layout_pos.x+diff.x, layout_pos.y);
		obj->setSize(layout_sz.x-diff.x, layout_sz.y);
	}

	if(!bStop && ((!bRightListExpand && diff.x<0) || bRightListExpand) && ((!bLeftListExpand && diff.x>0) || bLeftListExpand))
	{
		for(std::vector<geLayout*>::iterator it = leftResizeLayoutList.begin(); it != leftResizeLayoutList.end(); ++it)
		{
			geLayout* obj = *it;
			geVector2f layout_sz(obj->getSize());
			obj->setSize(layout_sz.x+diff.x, layout_sz.y);
		}
	}
}

void geLayoutManager::expandOrContractTopAndBottomLayoutsOnMouseDrag(int x, int y)
{
	geVector2i diff(geVector2i(x, y)-mousePreviousPos);

	bool bStop=false;
	bool bRightListExpand=true;
	bool bLeftListExpand=true;

	//check any layout resist expansion
	for(std::vector<geLayout*>::iterator it = bottomResizeLayoutList.begin(); it != bottomResizeLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		if(obj->getSize().y-diff.y<=20)
		{
			bRightListExpand=false;
			break;
		}
	}

	for(std::vector<geLayout*>::iterator it = topResizeLayoutList.begin(); it != topResizeLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		if(obj->getSize().y+diff.y<=20)
		{
			bLeftListExpand=false;
			break;
		}
	}

	if(((!bRightListExpand && diff.y<0) || bRightListExpand) && ((!bLeftListExpand && diff.y>0) || bLeftListExpand))
	for(std::vector<geLayout*>::iterator it = bottomResizeLayoutList.begin(); it != bottomResizeLayoutList.end(); ++it)
	{
		geLayout* obj = *it;
		geVector2f layout_pos(obj->getPos());
		geVector2f layout_sz(obj->getSize());
		obj->setPos(layout_pos.x, layout_pos.y+diff.y);
		obj->setSize(layout_sz.x, layout_sz.y-diff.y);
	}

	if(!bStop && ((!bRightListExpand && diff.y<0) || bRightListExpand) && ((!bLeftListExpand && diff.y>0) || bLeftListExpand))
	{
		for(std::vector<geLayout*>::iterator it = topResizeLayoutList.begin(); it != topResizeLayoutList.end(); ++it)
		{
			geLayout* obj = *it;
			geVector2f layout_sz(obj->getSize());
			obj->setSize(layout_sz.x, layout_sz.y+diff.y);
		}
	}
}

//#if !defined(__APPLE__) //disable Drag-Drop
void geLayoutManager::onDragEnter(int x, int y)
{
	geLayout* selectedLayout = rootLayout->selectLayout(x, y);
	if(selectedLayout)
		selectedLayout->DragEnter(x, y);
}

void geLayoutManager::onDragDrop(int x, int y, MDropData* dropObject)
{
	geLayout* selectedLayout = rootLayout->selectLayout(x, y);
	if(selectedLayout)
		selectedLayout->DragDrop(x, y, dropObject);
}

void geLayoutManager::onDragLeave()
{
	//there is only one window. so practically no dragLeave on logical windows
}
//#endif

bool geLayoutManager::onKeyDown(int charValue, int flag)
{
	if(selectedLayout)
		return selectedLayout->KeyDown(charValue, flag);

	return false;
}

bool geLayoutManager::onKeyUp(int charValue, int flag)
{
	if(selectedLayout)
		return selectedLayout->KeyUp(charValue, flag);

	return false;
}

void geLayoutManager::onCommand(int cmd)
{
	if(selectedLayout)
		return selectedLayout->DoCommand(cmd);

}