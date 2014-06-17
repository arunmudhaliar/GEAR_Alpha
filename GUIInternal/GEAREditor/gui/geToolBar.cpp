#include "geToolBar.h"
#include "geGUIManager.h"


geToolBarButton::geToolBarButton():
	geButtonBase(GEGUI_TOOLBAR_BUTTON, "ToolBarButton")
{
	//no implementation
}

geToolBarButton::geToolBarButton(rendererGL10* renderer, const char* name, geGUIBase* parent):
	geButtonBase(GEGUI_TOOLBAR_BUTTON, name)
{
	createBase(renderer, parent);

	int width=geGUIManager::g_pFontArial10_84Ptr->calculateStringWidthInPixelTillNewLine(name, strlen(name), 0);
	setSize(width+20, parent->getSize().y);

	setClientAreaPrimaryActiveForeColor(0.2f, 0.2f, 0.2f, 1.0f);
	applyPrimaryColorToVBClientArea(EGRADIENT_VERTICAL_UP, 0.45f);
	setClientAreaSecondryActiveForeColor(0.4, 0.4, 0.4, 1.0f);

	m_bImageLoaded=false;

	//create(parent, name, 0, 0);
}

geToolBarButton::~geToolBarButton()
{
}

void geToolBarButton::loadImage(const char* filename, int clipx, int clipy)
{
	m_cSprite.loadTexture(&geGUIManager::g_cTextureManager, filename);
	m_cSprite.setClip(clipx, clipy, 16, 16);
	setSize(26, getSize().y);
	m_bImageLoaded=true;
}

void geToolBarButton::draw()
{
	glPushMatrix();
	glTranslatef(m_cPos.x, m_cPos.y, 0);
	drawRect(&m_cVBClientArea);
	drawLine(m_cVBClientAreaLine, 0.1, 0.1, 0.1, 1.0f, 2, false);
	if(m_bImageLoaded)
	{
		geVector2f offsetPos(5, 0);
		m_cSprite.draw(&offsetPos);
	}
	else
	{
		geGUIManager::g_pFontArial10_84Ptr->drawString(m_szName, 10, geGUIManager::g_pFontArial10_84Ptr->getLineHeight()-5, m_cSize.x);
	}
	glPopMatrix();
}
	
void geToolBarButton::onPosition(float x, float y, int flag)
{
}

void geToolBarButton::onSize(float cx, float cy, int flag)
{
	const float title_vertLst[8] =
	{
		cx-1,	0,
		-1,	0,
		cx-1,	cy,
		-1,	cy,
	};

	memcpy(m_cVBClientArea.m_cszVertexList, title_vertLst, sizeof(title_vertLst));

	const float clientarea_linevertLst[4] =
	{
		cx-1,	0,
		cx-1,	cy,
	};
	memcpy(m_cVBClientAreaLine, clientarea_linevertLst, sizeof(clientarea_linevertLst));
}

void geToolBarButton::onButtonStateChanged(EBUTTON_STATE eFromState, bool dontPassEventToObserver)
{
	switch(m_eState)
	{
	case BTN_STATE_NORMAL:
		applyPrimaryColorToVBClientArea(EGRADIENT_VERTICAL_UP, 0.45f);
		break;
	case BTN_STATE_PRESSED:
		applySecondryColorToVBClientArea(EGRADIENT_VERTICAL_DOWN, 0.45f);
		break;
	case BTN_STATE_CANCEL:
		applyPrimaryColorToVBClientArea(EGRADIENT_VERTICAL_UP, 0.45f);
		m_eState=BTN_STATE_NORMAL;
		break;
	}
}

void geToolBarButton::onButtonClicked()
{
	if(m_pGUIObserver)
		m_pGUIObserver->onButtonClicked(this);
}

bool geToolBarButton::onMouseLButtonDown(float x, float y, int nFlag)
{
	if(m_eState==BTN_STATE_NORMAL)
	{
		buttonPressed(false);
	}
	else if(m_eState==BTN_STATE_PRESSED)
	{
		buttonNormal(false);
	}

	return true;
}

bool geToolBarButton::onMouseLButtonUp(float x, float y, int nFlag)
{
	onButtonClicked();
	return true;
}

void geToolBarButton::onMouseEnterClientArea()
{
	//char buffer[256];
	//sprintf(buffer, "%s onMouseEnterClientArea\n",m_szName);
	//OutputDebugString(buffer);
	//setColor(&m_cVBClientArea, 0.3, 0.3, 0.3, 1.0f, EGRADIENT_VERTICAL_UP, 0.4f);
}

void geToolBarButton::onMouseExitClientArea()
{
	//char buffer[256];
	//sprintf(buffer, "%s onMouseExitClientArea\n",m_szName);
	//OutputDebugString(buffer);
	//setColor(&m_cVBClientArea, 0.2, 0.2, 0.2, 1.0f, EGRADIENT_VERTICAL_UP, 0.45f);
}

void geToolBarButton::onCancelEngagedControls()
{
	//setColor(&m_cVBClientArea, 0.3, 0.3, 0.3, 1.0f, EGRADIENT_VERTICAL_UP, 0.3f);
}

////////////////////////////////////////////////////////////////////////////
geToolBar::geToolBar():
	geGUIBase(GEGUI_TOOLBAR, "ToolBar")
{
}

geToolBar::geToolBar(unsigned short uGUIID, const char* name):
	geGUIBase(uGUIID, name)
{
}

geToolBar::~geToolBar()
{
	for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
	{
		geGUIBase* node = *it;
		GE_DELETE(node);
	}
}

void geToolBar::create(rendererGL10* renderer, geGUIBase* parent, float x, float y, float cx, float cy)
{
	createBase(renderer, parent);

	setPos(x, y);
	setSize(cx, cy);
	setClientAreaPrimaryActiveForeColor(0.2f, 0.2f, 0.2f, 1.0f);
	applyPrimaryColorToVBClientArea(EGRADIENT_VERTICAL_UP, 0.45f);
	setMouseBoundCheck(false);
}

void geToolBar::onPosition(float x, float y, int flag)
{
}

void geToolBar::onSize(float cx, float cy, int flag)
{
	const float title_vertLst[8] =
	{
		cx,	0,
		0,	0,
		cx,	cy,
		0,	cy,
	};
	memcpy(m_cVBClientArea.m_cszVertexList, title_vertLst, sizeof(title_vertLst));
}

void geToolBar::draw()
{
	glPushMatrix();
	glTranslatef(m_cPos.x, m_cPos.y+getParent()->getTopMarginOffsetHeight(), 0);
	drawRect(&m_cVBClientArea);

	for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
	{
		geGUIBase* obj = *it;
		obj->draw();
	}

	glPopMatrix();
}

geGUIBase* geToolBar::appendToolBarControl(geGUIBase* ctrl)
{
	int xOffset=2;
	for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
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