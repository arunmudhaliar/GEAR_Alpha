#include "geGUIBase.h"


geGUIBase::geGUIBase()
{
	m_pRenderer=NULL;
	m_pSelectedControlPtr=NULL;
	m_pParentPtr=NULL;
	setMouseEntered(false);
	setSizable(false);
	setMouseBoundCheck(true);
	m_pUserData=NULL;
	m_pGUIObserver = NULL;
	m_pActiveWindowPtrOnlyForLayout=NULL;
	setClientAreaPrimaryActiveForeColor(1.0f, 1.0f, 1.0f);
	setClientAreaSecondryActiveForeColor(0.5f, 0.5f, 0.5f);
}

geGUIBase::geGUIBase(unsigned short uGUIID, const char* name):
	m_uGUIID(uGUIID)
{
	m_pRenderer=NULL;
	setSizable(false);
	m_pSelectedControlPtr=NULL;
	m_pParentPtr=NULL;
	setMouseEntered(false);
	setMouseBoundCheck(true);
	m_pUserData=NULL;

	if(name!=NULL)
	{
		STRCPY(m_szName, name);
	}
	m_pGUIObserver=NULL;
	m_pActiveWindowPtrOnlyForLayout=NULL;

	setClientAreaPrimaryActiveForeColor(1.0f, 1.0f, 1.0f);
	setClientAreaSecondryActiveForeColor(0.5f, 0.5f, 0.5f);
}

geGUIBase::~geGUIBase()
{
	//for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
	//{
	//	geGUIBase* node = *it;
	//	GE_DELETE(node);
	//}
	m_vControls.clear();
}

void geGUIBase::createBase(rendererGL10* renderer, geGUIBase* parent)
{
	m_pRenderer=renderer;
	setSizable(false);
	m_pSelectedControlPtr=NULL;
	setMouseEntered(false);
	setMouseBoundCheck(true);
	if(parent)
	{
		m_pParentPtr=parent;
		parent->appendChildControl(this);
	}
}

void geGUIBase::setParent(geGUIBase* parent)
{
	if(parent)
	{
		m_pParentPtr=parent;
		parent->appendChildControl(this);
	}
}

void geGUIBase::setPos(const geVector2f& pos)
{
	setPos(pos.x, pos.y);
}

void geGUIBase::setPos(float x, float y)
{
	m_cPos.set(x, y);
	onPosition(x, y, 0);
}
void geGUIBase::setSize(float cx, float cy)
{
	if(cx<=0.0f || cy<=0.0f)
	{
		cx=cy=1.0f;
	}
	m_cSize.set(cx, cy);

	for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
	{
		geGUIBase* obj = *it;
		if(obj->isSizable())
			obj->setSize(cx, cy-getTopMarginOffsetHeight());
	}

	onSize(cx, cy, 0);
}

void geGUIBase::setSize(const geVector2f& size)
{
	setSize(size.x, size.y);
}

void geGUIBase::onResizeComplete()
{
}

void geGUIBase::resizeComplete()
{
	for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
	{
		geGUIBase* obj = *it;
		if(obj->isSizable())
			obj->resizeComplete();
	}

	onResizeComplete();
}

void geGUIBase::applyPrimaryColorToVBClientArea(ESTYLE_GRADIENT eGradientStyle, float gradientScale)
{
	setColor(&m_cVBClientArea, m_fszClientAreaPrimaryActiveForeColor[0],
								m_fszClientAreaPrimaryActiveForeColor[1],
								m_fszClientAreaPrimaryActiveForeColor[2],
								m_fszClientAreaPrimaryActiveForeColor[3],
								eGradientStyle,
								gradientScale);
}

void geGUIBase::applySecondryColorToVBClientArea(ESTYLE_GRADIENT eGradientStyle, float gradientScale)
{
	setColor(&m_cVBClientArea, m_fszClientAreaSecondryActiveForeColor[0],
								m_fszClientAreaSecondryActiveForeColor[1],
								m_fszClientAreaSecondryActiveForeColor[2],
								m_fszClientAreaSecondryActiveForeColor[3],
								eGradientStyle,
								gradientScale);
}

void geGUIBase::setColor(stVertexBuffer* vbuffer, float r, float g, float b, float a, ESTYLE_GRADIENT eGradientStyle, float gradientScale)
{
	switch(eGradientStyle)
	{
	case EGRADIENT_NONE:
		{
			setColor(vbuffer, 0, r, g, b, a);
			setColor(vbuffer, 1, r, g, b, a);
			setColor(vbuffer, 2, r, g, b, a);
			setColor(vbuffer, 3, r, g, b, a);
		}
		break;

	case EGRADIENT_VERTICAL_UP:
		{
			setColor(vbuffer, 0, r, g, b, a);
			setColor(vbuffer, 1, r, g, b, a);
			setColor(vbuffer, 2, r*gradientScale, g*gradientScale, b*gradientScale, a);
			setColor(vbuffer, 3, r*gradientScale, g*gradientScale, b*gradientScale, a);
		}
		break;

	case EGRADIENT_VERTICAL_DOWN:
		{
			setColor(vbuffer, 0, r*gradientScale, g*gradientScale, b*gradientScale, a);
			setColor(vbuffer, 1, r*gradientScale, g*gradientScale, b*gradientScale, a);
			setColor(vbuffer, 2, r, g, b, a);
			setColor(vbuffer, 3, r, g, b, a);
		}
		break;

	case EGRADIENT_HORIZONTAL_RIGHT:
		{
			setColor(vbuffer, 0, r, g, b, a);
			setColor(vbuffer, 2, r, g, b, a);
			setColor(vbuffer, 1, r*gradientScale, g*gradientScale, b*gradientScale, a);
			setColor(vbuffer, 3, r*gradientScale, g*gradientScale, b*gradientScale, a);
		}
		break;

	case EGRADIENT_HORIZONTAL_LEFT:
		{
			setColor(vbuffer, 1, r, g, b, a);
			setColor(vbuffer, 3, r, g, b, a);
			setColor(vbuffer, 0, r*gradientScale, g*gradientScale, b*gradientScale, a);
			setColor(vbuffer, 2, r*gradientScale, g*gradientScale, b*gradientScale, a);
		}
		break;
	}
}

void geGUIBase::setClientAreaPrimaryActiveForeColor(float r, float g, float b, float a)
{
	m_fszClientAreaPrimaryActiveForeColor[0]=r;
	m_fszClientAreaPrimaryActiveForeColor[1]=g;
	m_fszClientAreaPrimaryActiveForeColor[2]=b;
	m_fszClientAreaPrimaryActiveForeColor[3]=a;
}

void geGUIBase::setClientAreaSecondryActiveForeColor(float r, float g, float b, float a)
{
	m_fszClientAreaSecondryActiveForeColor[0]=r;
	m_fszClientAreaSecondryActiveForeColor[1]=g;
	m_fszClientAreaSecondryActiveForeColor[2]=b;
	m_fszClientAreaSecondryActiveForeColor[3]=a;
}

void geGUIBase::setColor(stVertexBuffer* vbuffer, int index, float r, float g, float b, float a)
{
	vbuffer->m_cszVertexColorList[index*4+0] = r;
	vbuffer->m_cszVertexColorList[index*4+1] = g;
	vbuffer->m_cszVertexColorList[index*4+2] = b;
	vbuffer->m_cszVertexColorList[index*4+3] = a;
}

void geGUIBase::drawLine(float* line, float r, float g, float b, float a, int count, bool bLoop)
{
	glColor4f(r, g, b, a);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, line);
	glDrawArrays(bLoop?GL_LINE_LOOP:GL_LINE_STRIP, 0, count);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void geGUIBase::drawTriangle(float* buffer, float r, float g, float b, float a, int count)
{
	glColor4f(r, g, b, a);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, buffer);
	glDrawArrays(GL_TRIANGLES, 0, count);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void geGUIBase::drawRect(stVertexBuffer* vbuffer)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vbuffer->m_cszVertexList);
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_FLOAT, 0, vbuffer->m_cszVertexColorList);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

   	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void geGUIBase::drawRect(stVertexBuffer* vbuffer, float* textureCoord, unsigned int texID)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vbuffer->m_cszVertexList);
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_FLOAT, 0, vbuffer->m_cszVertexColorList);

	if(textureCoord)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glTexCoordPointer(2, GL_FLOAT, 0, textureCoord);
	}
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	if(textureCoord)
	{
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}

   	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

bool geGUIBase::isPointInsideWindow(float x, float y)
{
	return (x>=m_cPos.x && x<=m_cPos.x+m_cSize.x && y>=m_cPos.y && y<=m_cPos.y+m_cSize.y);
}

bool geGUIBase::isPointInsideClientArea(float x, float y)
{
	return (x>=0 && x<=m_cSize.x && y>=0 && y<=m_cSize.y);
}

void geGUIBase::onPosition(float x, float y, int flag)
{
}

void geGUIBase::onSize(float cx, float cy, int flag)
{
}

void geGUIBase::appendChildControl(geGUIBase* child)
{
	onAppendChild(child);
	m_vControls.push_back(child);
}

bool geGUIBase::MouseLButtonDown(float x, float y, int nFlag)
{
	bool commandUsed=false;
	if(isPointInsideWindow(x, y) || !isMouseBoundCheckEnabled())
	{
		if(!commandUsed)
		{
			if(m_uGUIID==GEGUI_LAYOUT && getActiveWindowPtrOnlyForLayout())	//layout hack
			{
				commandUsed=getActiveWindowPtrOnlyForLayout()->MouseLButtonDown(x-getPos().x, y-getPos().y-getTopMarginOffsetHeight(), nFlag);
				if(commandUsed)
					return commandUsed;
			}
			else
			{
				for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
				{
					geGUIBase* obj = *it;
					commandUsed=obj->MouseLButtonDown(x-getPos().x, y-getPos().y-getTopMarginOffsetHeight(), nFlag);
					if(commandUsed)
						return commandUsed;
				}
			}
		}

		if(isPointInsideClientArea(x-getPos().x, y-getPos().y-getTopMarginOffsetHeight()))
		{
			commandUsed=onMouseLButtonDown(x-getPos().x, y-getPos().y-getTopMarginOffsetHeight(), nFlag);
			if(commandUsed)
				m_pSelectedControlPtr=this;
		}
	}

	return commandUsed;
}

void geGUIBase::MouseLButtonUp(float x, float y, int nFlag)
{
	bool bHandled=false;
	if(isPointInsideWindow(x, y) || !isMouseBoundCheckEnabled())
	{
		if(m_uGUIID==GEGUI_LAYOUT && getActiveWindowPtrOnlyForLayout())	//layout hack
		{
			getActiveWindowPtrOnlyForLayout()->MouseLButtonUp(x-getPos().x, y-getPos().y-getTopMarginOffsetHeight(), nFlag);
		}
		else
		{
			for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
			{
				geGUIBase* obj = *it;
				obj->MouseLButtonUp(x-getPos().x, y-getPos().y-getTopMarginOffsetHeight(), nFlag);
			}
		}

		//if(isPointInsideClientArea(x-getPos().x, y-getPos().y-getTopMarginOffsetHeight()))
		{
			bHandled=onMouseLButtonUp(x-getPos().x, y-getPos().y-getTopMarginOffsetHeight(), nFlag);
			//if(m_pSelectedControlPtr==this)
			//	m_pSelectedControlPtr=NULL;
		}
	}

	if(m_pSelectedControlPtr)
	{
		//if(!bHandled)
			m_pSelectedControlPtr->CancelEngagedControls();
		m_pSelectedControlPtr=NULL;
	}
}

bool geGUIBase::MouseRButtonDown(float x, float y, int nFlag)
{
	return false;
}

void geGUIBase::MouseRButtonUp(float x, float y, int nFlag)
{

}

bool geGUIBase::MouseMove(float x, float y, int flag)
{
	if(m_uGUIID==GEGUI_LAYOUT && getActiveWindowPtrOnlyForLayout())	//layout hack
	{
		getActiveWindowPtrOnlyForLayout()->MouseMove(x-getPos().x, y-getPos().y-getTopMarginOffsetHeight(), flag);
	}
	else
	{
		for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
		{
			geGUIBase* obj = *it;
			obj->MouseMove(x-getPos().x, y-getPos().y-getTopMarginOffsetHeight(), flag);
		}
	}

	return onMouseMove(x-getPos().x, y-getPos().y-getTopMarginOffsetHeight(), flag);

	//if(isPointInsideWindow(x, y) || !isMouseBoundCheckEnabled())
	//{
	//	onMouseMove(x, y, flag);
	//	if(!isMouseEntered())
	//	{
	//		mouseEnteredClientArea();
	//	}
	//}
	//else
	//{
	//	if(isMouseEntered())
	//	{
	//		mouseExitClientArea();
	//	}
	//}
}

void geGUIBase::MouseWheel(int zDelta, int x, int y, int flag)
{
	if(m_uGUIID==GEGUI_LAYOUT && getActiveWindowPtrOnlyForLayout())	//layout hack
	{
		getActiveWindowPtrOnlyForLayout()->MouseWheel(zDelta, x-getPos().x, y-getPos().y-getTopMarginOffsetHeight(), flag);
	}
	else
	{
		for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
		{
			geGUIBase* obj = *it;
			obj->MouseWheel(zDelta, x-getPos().x, y-getPos().y-getTopMarginOffsetHeight(), flag);
		}
	}
	onMouseWheel(zDelta, x-getPos().x, y-getPos().y-getTopMarginOffsetHeight(), flag);
}

bool geGUIBase::onMouseLButtonDown(float x, float y, int nFlag)
{
	return false;
}

bool geGUIBase::onMouseLButtonUp(float x, float y, int nFlag)
{
	return false;
}

bool geGUIBase::onMouseRButtonDown(float x, float y, int nFlag)
{
	return false;
}

void geGUIBase::onMouseRButtonUp(float x, float y, int nFlag)
{
}

bool geGUIBase::onMouseMove(float x, float y, int flag)
{
	return false;
}

void geGUIBase::onMouseWheel(int zDelta, int x, int y, int flag)
{
}

void geGUIBase::hoverControl()
{
}

void geGUIBase::unHoverControl()
{
}

void geGUIBase::onHoverControl(int x, int y)
{
}

void geGUIBase::onUnHoverControl(int x, int y)
{
}


void geGUIBase::mouseEnteredClientArea()
{
	setMouseEntered(true);
	onMouseEnterClientArea();
}

void geGUIBase::mouseExitClientArea()
{
	setMouseEntered(false);
	onMouseExitClientArea();
}

void geGUIBase::onMouseEnterClientArea()
{
}

void geGUIBase::onMouseExitClientArea()
{
}

void geGUIBase::CancelEngagedControls()
{
	for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
	{
		geGUIBase* obj = *it;
		obj->CancelEngagedControls();
	}

	onCancelEngagedControls();
}

void geGUIBase::onCancelEngagedControls()
{
	setMouseEntered(false);
}

geVector2f geGUIBase::getAbsolutePositionOnScreen()
{
	return geVector2f();
}

geVector2f geGUIBase::getPositionOnScreen()
{
	geGUIBase* baseGUI=this;
	geVector2f absPos;
	while(baseGUI)
	{
		absPos=absPos+baseGUI->getPos();//+baseGUI->getAbsolutePositionOnScreen();
		absPos.y-=(/*-baseGUI->getTopMarginOffsetHeight()+*/baseGUI->getTitleOffsetHeight());
		baseGUI=baseGUI->getParent();
	}

	return absPos;
}

void geGUIBase::focusLost()
{
	for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
	{
		geGUIBase* obj = *it;
		obj->focusLost();
	}

	onFocusLost();
}

void geGUIBase::onFocusLost()
{
}

void geGUIBase::onCreate()
{
}

void geGUIBase::DragEnter(int x, int y)
{
	if(isPointInsideWindow(x, y))
	{
		if(m_uGUIID==GEGUI_LAYOUT && getActiveWindowPtrOnlyForLayout())	//layout hack
		{
			getActiveWindowPtrOnlyForLayout()->DragEnter(x-getPos().x, y-getPos().y-getTopMarginOffsetHeight());
		}
		else
		{
			for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
			{
				geGUIBase* obj = *it;
				obj->DragEnter(x-getPos().x, y-getPos().y-getTopMarginOffsetHeight());
			}
		}
		onDragEnter(x-getPos().x, y-getPos().y-getTopMarginOffsetHeight());
	}
}

void geGUIBase::DragDrop(int x, int y, MDataObject* dropObject)
{
	if(isPointInsideWindow(x, y) /*|| !isMouseBoundCheckEnabled()*/)
	{
		if(m_uGUIID==GEGUI_LAYOUT && getActiveWindowPtrOnlyForLayout())	//layout hack
		{
			getActiveWindowPtrOnlyForLayout()->DragDrop(x-getPos().x, y-getPos().y-getTopMarginOffsetHeight(), dropObject);
		}
		else
		{
			for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
			{
				geGUIBase* obj = *it;
				obj->DragDrop(x-getPos().x, y-getPos().y-getTopMarginOffsetHeight(), dropObject);
			}
		}
		onDragDrop(x-getPos().x, y-getPos().y-getTopMarginOffsetHeight(), dropObject);
	}
}

void geGUIBase::DragLeave()
{
	if(m_uGUIID==GEGUI_LAYOUT && getActiveWindowPtrOnlyForLayout())	//layout hack
	{
		getActiveWindowPtrOnlyForLayout()->DragLeave();
	}
	else
	{
		for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
		{
			geGUIBase* obj = *it;
			obj->DragLeave();
		}
	}
	onDragLeave();
}

void geGUIBase::onDragEnter(int x, int y)
{
}

void geGUIBase::onDragDrop(int x, int y, MDataObject* dropObject)
{
}

void geGUIBase::onDragLeave()
{
}

void geGUIBase::onAppendChild(geGUIBase* child)
{
}

bool geGUIBase::KeyDown(int charValue, int flag)
{
	if(m_uGUIID==GEGUI_LAYOUT && getActiveWindowPtrOnlyForLayout())	//layout hack
	{
		if(getActiveWindowPtrOnlyForLayout()->KeyDown(charValue, flag))
			return true;
	}
	else
	{
		for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
		{
			geGUIBase* obj = *it;
			if(obj->KeyDown(charValue, flag))
				return true;
		}
	}
	return onKeyDown(charValue, flag);
}

bool geGUIBase::KeyUp(int charValue, int flag)
{
	if(m_uGUIID==GEGUI_LAYOUT && getActiveWindowPtrOnlyForLayout())	//layout hack
	{
		if(getActiveWindowPtrOnlyForLayout()->KeyUp(charValue, flag))
			return true;
	}
	else
	{
		for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
		{
			geGUIBase* obj = *it;
			if(obj->KeyUp(charValue, flag))
				return true;
		}
	}
	return onKeyUp(charValue, flag);
}

bool geGUIBase::onKeyDown(int charValue, int flag)
{
	return false;
}

bool geGUIBase::onKeyUp(int charValue, int flag)
{
	return false;
}

void geGUIBase::onSetName()
{

}

bool geGUIBase::isNodeExistsInTree(geGUIBase* node)
{
	if(this==node)
		return true;

	if(m_uGUIID==GEGUI_LAYOUT && getActiveWindowPtrOnlyForLayout())	//layout hack: no need to implement this, but for consistency purpose i kept
	{
		if(getActiveWindowPtrOnlyForLayout()->isNodeExistsInTree(node))
			return true;
	}
	else
	{
		for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
		{
			geGUIBase* obj = *it;
			if(obj->isNodeExistsInTree(node))
				return true;
		}
	}
	return false;
}

void geGUIBase::DoCommand(int cmd)
{
	if(m_uGUIID==GEGUI_LAYOUT && getActiveWindowPtrOnlyForLayout())	//layout hack: no need to implement this, but for consistency purpose i kept
	{
		getActiveWindowPtrOnlyForLayout()->DoCommand(cmd);
	}
	else
	{
		for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
		{
			geGUIBase* obj = *it;
			obj->DoCommand(cmd);
		}
	}
	return onCommand(cmd);
}

void geGUIBase::onCommand(int cmd)
{

}