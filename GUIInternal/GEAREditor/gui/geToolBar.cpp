#include "geToolBar.h"
#include "geGUIManager.h"


geToolBarButton::geToolBarButton(geFontManager* fontManager):
	geButtonBase(GEGUI_TOOLBAR_BUTTON, "ToolBarButton", fontManager)
{
	//no implementation
}

geToolBarButton::geToolBarButton(rendererGL10* renderer, const char* name, geGUIBase* parent, geFontManager* fontManager):
	geButtonBase(GEGUI_TOOLBAR_BUTTON, name, fontManager)
{
    int width=geFontManager::g_pFontArial10_84Ptr->calculateStringWidthInPixelTillNewLine(name, (int)strlen(name), 0);
	createBase(renderer, parent, m_cPos.x, m_cPos.y, width+20, parent->getSize().y);

	setClientAreaPrimaryActiveForeColor(0.3f, 0.3f, 0.3f, 1.0f);
	applyPrimaryColorToVBClientArea(EGRADIENT_VERTICAL_UP, 0.45f);
	setClientAreaSecondryActiveForeColor(0.32, 0.32, 0.32, 1.0f);

	isImageLoaded=false;
}

geToolBarButton::~geToolBarButton()
{
}

void geToolBarButton::loadImage(const char* filename, int clipx, int clipy)
{
	sprite.loadTexture(&geGUIManager::g_cTextureManager, filename);
	sprite.setClip(clipx, clipy, 16, 16);
	setSize(26, getSize().y);
	isImageLoaded=true;
}

void geToolBarButton::draw()
{
	glPushMatrix();
	glTranslatef(m_cPos.x, m_cPos.y, 0);
	drawRect(&vertexBufferClientArea);
	drawLine(vertexBufferClientAreaArray, 0.1, 0.1, 0.1, 1.0f, 2, false);
	if(isImageLoaded)
	{
		geVector2f offsetPos(5, 0);
    
        auto clr = &btnStateColor[buttonState];
        sprite.setRGB(clr->x, clr->y, clr->z);
		sprite.draw(&offsetPos);
	}
	else
	{
		geFontManager::g_pFontArial10_84Ptr->drawString(m_szName, 10, geFontManager::g_pFontArial10_84Ptr->getLineHeight()-5, m_cSize.x);
	}
	glPopMatrix();
}
	
void geToolBarButton::onPosition(float x, float y, int flag)
{
}

void geToolBarButton::onSize(float cx, float cy, int flag)
{
//	const float title_vertLst[8] =
//	{
//		cx-1,	0,
//		-1,	0,
//		cx-1,	cy,
//		-1,	cy,
//	};
//
//	memcpy(vertexBufferClientArea.vertexArray, title_vertLst, sizeof(title_vertLst));
    vertexBufferClientArea.updateRect(-1, 0, cx, cy);
    
	const float clientarea_linevertLst[4] =
	{
		cx-1,	0,
		cx-1,	cy,
	};
	memcpy(vertexBufferClientAreaArray, clientarea_linevertLst, sizeof(clientarea_linevertLst));
}

void geToolBarButton::onButtonStateChanged(EBUTTON_STATE eFromState, bool dontPassEventToObserver)
{
	switch(buttonState)
	{
	case BTN_STATE_NORMAL:
		applyPrimaryColorToVBClientArea(EGRADIENT_VERTICAL_UP, 0.45f);
		break;
	case BTN_STATE_PRESSED:
		applySecondryColorToVBClientArea(EGRADIENT_VERTICAL_DOWN, 0.45f);
		break;
	case BTN_STATE_CANCEL:
		applyPrimaryColorToVBClientArea(EGRADIENT_VERTICAL_UP, 0.45f);
		buttonState=BTN_STATE_NORMAL;
		break;
	}
    
    if(guiObserver && !dontPassEventToObserver)
        guiObserver->onButtonClicked(this);
}

void geToolBarButton::onButtonClicked()
{
	if(guiObserver)
		guiObserver->onButtonClicked(this);
}

bool geToolBarButton::onMouseLButtonDown(float x, float y, int nFlag)
{
	if(buttonState==BTN_STATE_NORMAL)
	{
		buttonPressed(false);
	}
	else if(buttonState==BTN_STATE_PRESSED)
	{
		buttonNormal(false);
	}

	return true;
}

bool geToolBarButton::onMouseLButtonUp(float x, float y, int nFlag)
{
	//onButtonClicked();
	return true;
}

void geToolBarButton::onMouseEnterClientArea()
{
	//char buffer[256];
	//sprintf(buffer, "%s onMouseEnterClientArea\n",m_szName);
	//OutputDebugString(buffer);
	//setColor(&vertexBufferClientArea, 0.3, 0.3, 0.3, 1.0f, EGRADIENT_VERTICAL_UP, 0.4f);
}

void geToolBarButton::onMouseExitClientArea()
{
	//char buffer[256];
	//sprintf(buffer, "%s onMouseExitClientArea\n",m_szName);
	//OutputDebugString(buffer);
	//setColor(&vertexBufferClientArea, 0.2, 0.2, 0.2, 1.0f, EGRADIENT_VERTICAL_UP, 0.45f);
}

void geToolBarButton::onCancelEngagedControls()
{
	//setColor(&vertexBufferClientArea, 0.3, 0.3, 0.3, 1.0f, EGRADIENT_VERTICAL_UP, 0.3f);
}

////////////////////////////////////////////////////////////////////////////
geToolBar::geToolBar(geFontManager* fontmanager):
	geGUIBase(GEGUI_TOOLBAR, "ToolBar", fontmanager)
{
}

geToolBar::geToolBar(unsigned short uGUIID, const char* name, geFontManager* fontmanager):
	geGUIBase(uGUIID, name, fontmanager)
{
}

geToolBar::~geToolBar()
{
	for(std::vector<geGUIBase*>::iterator it = childControlList.begin(); it != childControlList.end(); ++it)
	{
		geGUIBase* node = *it;
		GE_DELETE(node);
	}
}

void geToolBar::create(rendererGL10* renderer, geGUIBase* parent, float x, float y, float cx, float cy)
{
	createBase(renderer, parent, x, y, cx, cy);
	setMouseBoundCheck(false);
    
	setClientAreaPrimaryActiveForeColor(0.2f, 0.2f, 0.2f, 1.0f);
	applyPrimaryColorToVBClientArea(EGRADIENT_VERTICAL_UP, 0.45f);
}

void geToolBar::onPosition(float x, float y, int flag)
{
}

void geToolBar::onSize(float cx, float cy, int flag)
{
//	const float title_vertLst[8] =
//	{
//		cx,	0,
//		0,	0,
//		cx,	cy,
//		0,	cy,
//	};
//	memcpy(vertexBufferClientArea.vertexArray, title_vertLst, sizeof(title_vertLst));
    vertexBufferClientArea.updateRect(0, 0, cx, cy);
}

void geToolBar::draw()
{
	glPushMatrix();
	glTranslatef(m_cPos.x, m_cPos.y+getParent()->getTopMarginOffsetHeight(), 0);
	drawRect(&vertexBufferClientArea);

	for(std::vector<geGUIBase*>::iterator it = childControlList.begin(); it != childControlList.end(); ++it)
	{
		geGUIBase* obj = *it;
		obj->draw();
	}

	glPopMatrix();
}

geGUIBase* geToolBar::appendToolBarControl(geGUIBase* ctrl)
{
	int xOffset=2;
	for(std::vector<geGUIBase*>::iterator it = childControlList.begin(); it != childControlList.end(); ++it)
	{
		geGUIBase* obj = *it;
		if(ctrl==obj) continue;
		xOffset+=(obj->getSize().x);
	}

	ctrl->setPos(xOffset, ctrl->getPos().y);

	return ctrl;
}

void geToolBar::onAppendChild(geGUIBase* child)
{
}