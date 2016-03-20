#include "geLayout.h"
#include "geGUIManager.h"
#include "../secondryViews/geModalDialog.h"

geLayout::geLayout(const char* name, geFontManager* fontmanager):
	geGUIBase(GEGUI_LAYOUT, name, fontmanager)
{
	activeWindow=NULL;
    pinLayout();
    memset(is_AreaGrabbed, 0, sizeof(is_AreaGrabbed));
    grabberAreaRects[0].setRect(gxRectf(), vector4f(0.25f, 0.24f, 0.27f, 0.3f));
    grabberAreaRects[1].setRect(gxRectf(), vector4f(0.25f, 0.24f, 0.27f, 0.3f));
    grabberAreaRects[2].setRect(gxRectf(), vector4f(0.25f, 0.24f, 0.27f, 0.3f));
    grabberAreaRects[3].setRect(gxRectf(), vector4f(0.25f, 0.24f, 0.27f, 0.3f));
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
    
    immediateChildLayoutList.clear();
}

void geLayout::clearAllList()
{
    childTopLayoutList.clear();
    childBottomLayoutList.clear();
    childRightLayoutList.clear();
    childLeftLayoutList.clear();
    immediateChildLayoutList.clear();
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

    unPinSprite.loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
    unPinSprite.setOffset(0.0f, 0.0f);
    unPinSprite.setClip(130, 110, 16, 16);
    unPinSprite.setRGB(0.5f, 0.5f, 0.5f);
    unPinSprite.setAlpha(0.7f);
    
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
    immediateChildLayoutList.push_back(childLayout);
}

void geLayout::appendRightChildLayout(geLayout* childLayout)
{
	childLayout->setParentLayout(this);
	childRightLayoutList.push_back(childLayout);
    immediateChildLayoutList.push_back(childLayout);
}

void geLayout::appendTopChildLayout(geLayout* childLayout)
{
	childLayout->setParentLayout(this);
	childTopLayoutList.push_back(childLayout);
    immediateChildLayoutList.push_back(childLayout);
}

void geLayout::appendBottomChildLayout(geLayout* childLayout)
{
	childLayout->setParentLayout(this);
	childBottomLayoutList.push_back(childLayout);
    immediateChildLayoutList.push_back(childLayout);
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

void geLayout::removeWindow(geWindow* window)
{
    if(std::find(childWindowList.begin(), childWindowList.end(), window)==childWindowList.end())
        return;

    if(activeWindow==window)
    {
        setActiveWindow(nullptr);
    }
    window->setIamOnLayout(nullptr);
    if(window->getParent())
        window->getParent()->removeChildControl(window);
    window->setParent(nullptr);
    childWindowList.erase(std::remove(childWindowList.begin(), childWindowList.end(), window));
    
    if(activeWindow==nullptr && childWindowList.size())
    {
        setActiveWindow(childWindowList[0]);
    }
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

	if(parentLayout!=NULL && activeWindow)
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
        
        if(is_Pinned)
        {
            glPushMatrix();
            glTranslatef(m_cSize.x-20, GE_WND_TITLE_HEIGHT*0.5f, 0);
            glScalef(0.7f, 0.7f, 1.0f);
            //unPinSprite.debugDraw();
            unPinSprite.draw();
            glPopMatrix();
        }
		activeWindow->draw();
        
        //check for grabbing
        for(int iterator=0;iterator<4;iterator++)
        {
            if(is_AreaGrabbed[iterator])
            {
                drawRect(&grabberAreaRects[iterator]);
            }
            is_AreaGrabbed[iterator]=false;
        }
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
    immediateChildLayoutList.erase(std::remove(immediateChildLayoutList.begin(), immediateChildLayoutList.end(), childLayout), immediateChildLayoutList.end());

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
#ifdef GEAR_WINDOWS
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
    {
        childLeftLayoutList.push_back(pLayout);
        immediateChildLayoutList.push_back(pLayout);
    }
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
#ifdef GEAR_WINDOWS
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
    {
		childRightLayoutList.push_back(pLayout);
        immediateChildLayoutList.push_back(pLayout);
    }
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
#ifdef GEAR_WINDOWS
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
    {
        childTopLayoutList.push_back(pLayout);
        immediateChildLayoutList.push_back(pLayout);
    }
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
#ifdef GEAR_WINDOWS
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
    {
        childBottomLayoutList.push_back(pLayout);
        immediateChildLayoutList.push_back(pLayout);
    }
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

void geLayout::reAdjustLayoutOnPlug(geLayout* parent, geLayout* grabbed, float ratio, int whichArea)
{
    geVector2f parentPos(parent->getPos());
    geVector2f parentSz(parent->getSize());
    
    //geVector2f
    switch (whichArea)
    {
        case 0: //left
        {
            grabbed->setPos(parentPos.x, parentPos.y);
            grabbed->setSize(parentSz.x*ratio, parentSz.y);
            grabbed->setLayoutDirection(LEFT_TO_PARENT);
            
            parent->setPos(parentPos.x+parentSz.x*ratio, parentPos.y);
            parent->setSize(parentSz.x*(1.0f-ratio), parentSz.y);
            parent->appendLeftChildLayout(grabbed);
//            pLayout->create(rendererGUI, pParentOfNewLayout, this->getPos().x, this->getPos().y, this->getSize().x*ratio,  this->getSize().y);
//            pLayout->setLayoutDirection((pParentOfNewLayout==this)?LEFT_TO_PARENT:RIGHT_TO_PARENT);
//            
//            this->setPos(this->getPos().x+this->getSize().x*ratio, this->getPos().y);
//            this->setSize(this->getSize().x*(1.0f-ratio),  this->getSize().y);
        }
            break;
        case 1: //right
        {
            grabbed->setPos(parentPos.x+parentSz.x*(1.0f-ratio), parentPos.y);
            grabbed->setSize(parentSz.x*ratio, parentSz.y);
            grabbed->setLayoutDirection(RIGHT_TO_PARENT);
            
            parent->setSize(parentSz.x*(1.0f-ratio), parentSz.y);
            parent->appendRightChildLayout(grabbed);
//            pLayout->create(rendererGUI, pParentOfNewLayout, this->getPos().x+this->getSize().x*(1.0f-ratio), this->getPos().y, this->getSize().x*ratio,  this->getSize().y);
//            pLayout->setLayoutDirection((pParentOfNewLayout==this)?RIGHT_TO_PARENT:LEFT_TO_PARENT);
//            
//            this->setSize(this->getSize().x*(1.0f-ratio),  this->getSize().y);

        }
            break;
        case 2: //top
        {
            grabbed->setPos(parentPos.x, parentPos.y);
            grabbed->setSize(parentSz.x, parentSz.y*ratio);
            grabbed->setLayoutDirection(TOP_TO_PARENT);

            parent->setPos(parentPos.x, parentPos.y+parentSz.y*ratio);
            parent->setSize(parentSz.x, parentSz.y*(1.0f-ratio));
            parent->appendTopChildLayout(grabbed);
            //pLayout->create(rendererGUI, pParentOfNewLayout, this->getPos().x, this->getPos().y, this->getSize().x,  this->getSize().y*ratio);
            //this->setPos(this->getPos().x, this->getPos().y+this->getSize().y*ratio);
            //this->setSize(this->getSize().x,  this->getSize().y*(1.0f-ratio));

        }
            break;
        case 3: //bottom
        {
            grabbed->setPos(parentPos.x, parentPos.y+parentSz.y*(1.0f-ratio));
            grabbed->setSize(parentSz.x, parentSz.y*ratio);
            grabbed->setLayoutDirection(BOTTOM_TO_PARENT);
            
            parent->setSize(parentSz.x, parentSz.y*(1.0f-ratio));
            parent->appendBottomChildLayout(grabbed);
            //this->getPos().x, this->getPos().y+this->getSize().y*(1.0f-ratio), this->getSize().x,  this->getSize().y*ratio
            //this->getSize().x,  this->getSize().y*(1.0f-ratio)
        }
            break;
            
        default:
            printf("ERROR : whichArea is %d", whichArea);
            break;
    }
}

//geLayout* geLayout::createAsParent(geWindow* window)
//{
//	geLayout* pLayout = new geLayout("layout", fontManagerGUI);
//	pLayout->create(rendererGUI, this, this->getPos().x, this->getPos().y, this->getSize().x,  this->getSize().y);
//	pLayout->setLayoutDirection(LAYOUT_PARENT);
//	this->setSize(this->getSize().x,  this->getSize().y);
//	appendLeftChildLayout(pLayout);
//	pLayout->appendWindow(window);
//
//	return pLayout;
//}

bool geLayout::onMouseLButtonDown(float x, float y, int nFlag)
{
	if(y<GE_WND_TITLE_HEIGHT)
	{
        bool handled=false;
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
                    handled=true;
				}
			}
		}        
	}

	mousePreviousPos.set(x, y);
	return false;
}

void geLayout::doUnPlug(geLayout* rootLayout)
{
    auto parent = getParentLayout();
    
    std::vector<geLayout*> fillingLayouts[4];
    ELAYOUT_DIRECTION directions[4] ={LEFT_TO_PARENT, RIGHT_TO_PARENT, TOP_TO_PARENT, BOTTOM_TO_PARENT};
    float fillingLengths[4]={0.0f, 0.0f, 0.0f, 0.0f};
    float currentLayoutSz[4] = { m_cSize.y, m_cSize.y, m_cSize.x, m_cSize.x };
    float diff_vec[4];
    
    for(int iterator = 0; iterator<4; iterator++)
    {
        checkToFitLayout(rootLayout, directions[iterator], fillingLayouts[iterator]);
        for (auto layout : fillingLayouts[iterator])
        {
            if(directions[iterator]==LEFT_TO_PARENT || directions[iterator]==RIGHT_TO_PARENT)
            {
                fillingLengths[iterator]+=layout->getSize().y;
            }
            else
            {
                fillingLengths[iterator]+=layout->getSize().x;
            }
        }
        
        diff_vec[iterator] = fillingLengths[iterator]-currentLayoutSz[iterator];
    }
    
    float min_val = 1000.0f;    //a value above 0.0f
    int bestIndex = -1;
    for(int xx=0;xx<4;xx++)
    {
        if(diff_vec[xx]>0.0f)
            continue;
        
        if(std::abs(diff_vec[xx])<min_val)
        {
            min_val = std::abs(diff_vec[xx]);
            bestIndex=xx;
        }
    }
    
    if(bestIndex>=0)
    {
        moveLayoutTo(fillingLayouts[bestIndex], currentLayoutSz[4-(bestIndex+1)], directions[bestIndex]);
    }
    
    //unpin the selected layout.
    parent->removeChildLayout(this);
    
    //move child layouts to parent layout
    for (auto childLayout: immediateChildLayoutList)
    {
        switch (childLayout->getLayoutDirection())
        {
            case geLayout::LEFT_TO_PARENT:
                parent->appendLeftChildLayout(childLayout);
                break;
            case geLayout::RIGHT_TO_PARENT:
                parent->appendRightChildLayout(childLayout);
                break;
            case geLayout::TOP_TO_PARENT:
                parent->appendTopChildLayout(childLayout);
                break;
            case geLayout::BOTTOM_TO_PARENT:
                parent->appendBottomChildLayout(childLayout);
                break;
            default:
                printf("ERROR : Undefined Direction\n");
                break;
        }
    }
    
    clearAllList();
}

bool geLayout::doUnPin(float x, float y, geLayout* rootLayout, bool& deleteLayout)
{
    deleteLayout=false;
    
    if(!isOverlapWithPinRect(x, y))
        return false;
    
    //do unpin the active window.
    auto tmp = activeWindow;
    removeWindow(activeWindow);
    geModalDialog::createModalDialog(tmp, geVector2i(m_cPos.x, m_cPos.y));
    
    //check if there is any child layout or not
    if(!activeWindow)
    {
        auto parent = getParentLayout();

        std::vector<geLayout*> fillingLayouts[4];
        ELAYOUT_DIRECTION directions[4] ={LEFT_TO_PARENT, RIGHT_TO_PARENT, TOP_TO_PARENT, BOTTOM_TO_PARENT};
        float fillingLengths[4]={0.0f, 0.0f, 0.0f, 0.0f};
        float currentLayoutSz[4] = { m_cSize.y, m_cSize.y, m_cSize.x, m_cSize.x };
        float diff_vec[4];
        
        for(int iterator = 0; iterator<4; iterator++)
        {
            checkToFitLayout(rootLayout, directions[iterator], fillingLayouts[iterator]);
            for (auto layout : fillingLayouts[iterator])
            {
                if(directions[iterator]==LEFT_TO_PARENT || directions[iterator]==RIGHT_TO_PARENT)
                {
                    fillingLengths[iterator]+=layout->getSize().y;
                }
                else
                {
                    fillingLengths[iterator]+=layout->getSize().x;
                }
            }
            
            diff_vec[iterator] = fillingLengths[iterator]-currentLayoutSz[iterator];
        }
        
        float min_val = 1000.0f;    //a value above 0.0f
        int bestIndex = -1;
        for(int xx=0;xx<4;xx++)
        {
            if(diff_vec[xx]>0.0f)
                continue;
            
            if(std::abs(diff_vec[xx])<min_val)
            {
                min_val = std::abs(diff_vec[xx]);
                bestIndex=xx;
            }
        }
        
        if(bestIndex>=0)
        {
            moveLayoutTo(fillingLayouts[bestIndex], currentLayoutSz[4-(bestIndex+1)], directions[bestIndex]);
        }
        
        //unpin the selected layout.
        parent->removeChildLayout(this);

        //move child layouts to parent layout
        for (auto childLayout: immediateChildLayoutList)
        {
            switch (childLayout->getLayoutDirection())
            {
                case geLayout::LEFT_TO_PARENT:
                    parent->appendLeftChildLayout(childLayout);
                    break;
                case geLayout::RIGHT_TO_PARENT:
                    parent->appendRightChildLayout(childLayout);
                    break;
                case geLayout::TOP_TO_PARENT:
                    parent->appendTopChildLayout(childLayout);
                    break;
                case geLayout::BOTTOM_TO_PARENT:
                    parent->appendBottomChildLayout(childLayout);
                    break;
                default:
                    printf("ERROR : Undefined Direction\n");
                    break;
            }
        }
        
        clearAllList();
        
        //delete the selected layout.
        deleteLayout = true;
    }
    
    return deleteLayout;
}

bool geLayout::isOverlapWithPinRect(float x, float y)
{
    geVector2f unpinRectSz(getUnPinRectSize());
    
    float leftEdge = m_cPos.x+m_cSize.x-20-unpinRectSz.x*0.5f;
    float rightEdge=m_cPos.x+m_cSize.x-20+unpinRectSz.x*0.5f;
    float topEdge =m_cPos.y+GE_WND_TITLE_HEIGHT*0.5f-unpinRectSz.y*0.5f;
    float bottomEdge =m_cPos.y+GE_WND_TITLE_HEIGHT*0.5f+unpinRectSz.y*0.5f;
    
    return (x>leftEdge && x<rightEdge && y>topEdge && y<bottomEdge);
}

void geLayout::checkToFitLayout(geLayout* layout, ELAYOUT_DIRECTION direction, std::vector<geLayout*>& fillingLayouts)
{
    geVector2f layoutPos(layout->getPos());
    geVector2f layoutSz(layout->getSize());
        
    //check if the layout is sharing any edge with me
    bool shareWithLeftEdge = false;
    bool shareWithRightEdge = false;
    bool shareWithTopEdge = false;
    bool shareWithBottomEdge = false;

    
    //NOTE :- LEFT_TO_PARENT : PARENT is the currentLayout in this logic, LEFT is the fitting layout.
    
    if(layout!=this)
    {
        switch (direction)
        {
            case LEFT_TO_PARENT:
            {
                shareWithLeftEdge = (std::abs(m_cPos.x-(layoutPos.x+layoutSz.x))<0.5f);
                bool canFit = m_cSize.y>=layoutSz.y && m_cPos.y<=layoutPos.y && m_cPos.y+m_cSize.y>=layoutPos.y+layoutSz.y;
                if(canFit && shareWithLeftEdge)
                {
                    fillingLayouts.push_back(layout);
                }
            }
                break;
            case RIGHT_TO_PARENT:
            {
                shareWithRightEdge = (std::abs((m_cPos.x+m_cSize.x)-layoutPos.x)<0.5f);
                bool canFit = m_cSize.y>=layoutSz.y && m_cPos.y<=layoutPos.y && m_cPos.y+m_cSize.y>=layoutPos.y+layoutSz.y;
                if(canFit && shareWithRightEdge)
                {
                    fillingLayouts.push_back(layout);
                }
            }
                break;
            case TOP_TO_PARENT:
            {
                shareWithTopEdge = (std::abs(m_cPos.y-(layoutPos.y+layoutSz.y))<0.5f);
                bool canFit = m_cSize.x>=layoutSz.x && m_cPos.x<=layoutPos.x && m_cPos.x+m_cSize.x>=layoutPos.x+layoutSz.x;
                if(canFit && shareWithTopEdge)
                {
                    fillingLayouts.push_back(layout);
                }
                
            }
                break;
            case BOTTOM_TO_PARENT:
            {
                shareWithBottomEdge = (std::abs((m_cPos.y+m_cSize.y)-layoutPos.y)<0.5f);
                bool canFit = m_cSize.x>=layoutSz.x && m_cPos.x<=layoutPos.x && m_cPos.x+m_cSize.x>=layoutPos.x+layoutSz.x;
                if(canFit && shareWithBottomEdge)
                {
                    fillingLayouts.push_back(layout);
                }
            }
                break;
                
            default:
                break;
        }
    }
    auto immediateList = layout->getImmediateChildLayoutList();
    for(auto immediate : immediateList)
        checkToFitLayout(immediate, direction, fillingLayouts);
}

void geLayout::moveLayoutTo(std::vector<geLayout*>& fillingLayouts, float delta, ELAYOUT_DIRECTION direction)
{
    for(auto layout : fillingLayouts)
    {
        geVector2f layoutPos(layout->getPos());
        geVector2f layoutSz(layout->getSize());

        switch (direction)
        {
            case LAYOUT_PARENT:
                printf("ERROR : This can not be possible.\n");
                break;
            case RIGHT_TO_PARENT:
                layout->setPos(layoutPos + geVector2f(-delta, 0.0f));
                layout->setSize(layoutSz + geVector2f(delta, 0.0f));
                break;
            case LEFT_TO_PARENT:
                layout->setSize(layoutSz + geVector2f(delta, 0.0f));
                break;
            case BOTTOM_TO_PARENT:
                layout->setPos(layoutPos + geVector2f(0.0f, -delta));
                layout->setSize(layoutSz + geVector2f(0.0f, delta));
                break;
            case TOP_TO_PARENT:
                layout->setSize(layoutSz + geVector2f(0.0f, delta));
                break;
                
            default:
                printf("ERROR : Undefined Direction\n");
                break;
        }
    }
}

bool geLayout::canGrab(float x, float y)
{
    return (x>m_cPos.x && x<m_cPos.x+m_cSize.x && y>m_cPos.y && y<m_cPos.y+GE_WND_TITLE_HEIGHT);
}

geLayout* geLayout::doGrabOverlapAreaCheck(geLayout* grabbed, float x, float y, int& whichArea)
{
    if(getLayoutDirection()!=LAYOUT_PARENT && grabbed!=this)
    {
        if(y>m_cPos.y && y<m_cPos.y+m_cSize.y)
        {
            if(x>m_cPos.x && x<m_cPos.x+m_cSize.x*0.2f)
            {
                //do left grab
                is_AreaGrabbed[0]=true;
                grabberAreaRects[0].updateRect(0, 0, m_cSize.x*0.2f, m_cSize.y);
                whichArea=0;
                return this;
            }
            else if(x>m_cPos.x+m_cSize.x-m_cSize.x*0.2f && x<m_cPos.x+m_cSize.x)
            {
                //do right grab
                is_AreaGrabbed[1]=true;
                grabberAreaRects[1].updateRect(m_cSize.x-m_cSize.x*0.2f, 0, m_cSize.x*0.2f, m_cSize.y);
                whichArea=1;
                return this;
            }
        }
        
        if(x>m_cPos.x && x<m_cPos.x+m_cSize.x)
        {
            if(y>m_cPos.y && y<m_cPos.y+m_cSize.y*0.2f)
            {
                //do top grab
                is_AreaGrabbed[2]=true;
                grabberAreaRects[2].updateRect(0, 0, m_cSize.x, m_cSize.y*0.2f);
                whichArea=2;
                return this;
            }
            else if(y>m_cPos.y+m_cSize.y-m_cSize.y*0.2f && y<m_cPos.y+m_cSize.y)
            {
                //do bottom grab
                is_AreaGrabbed[3]=true;
                grabberAreaRects[3].updateRect(0, m_cSize.y-m_cSize.y*0.2f, m_cSize.x, m_cSize.y*0.2f);
                whichArea=3;
                return this;
            }
        }
    }
    
    for(auto immediate : immediateChildLayoutList)
    {
        auto ret_val = immediate->doGrabOverlapAreaCheck(grabbed, x, y, whichArea);
        if(ret_val)
            return ret_val;
    }
    
    return nullptr;
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
    if(activeWindow)
        activeWindow->CancelEngagedControls();
}