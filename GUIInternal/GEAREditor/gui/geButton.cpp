#include "geButton.h"
#include "geGUIManager.h"

geButtonBase::geButtonBase(unsigned short uGUIID, const char* name):
	geGUIBase(uGUIID, name)
{
	m_eState = BTN_STATE_NORMAL;
	m_bMouseHover=false;
}

geButtonBase::~geButtonBase()
{
}

void geButtonBase::buttonPressed()
{
	EBUTTON_STATE prevState=m_eState;
	m_eState=BTN_STATE_PRESSED;
	onButtonStateChanged(prevState);
}

void geButtonBase::buttonNormal()
{
	EBUTTON_STATE prevState=m_eState;
	m_eState = BTN_STATE_NORMAL;
	onButtonStateChanged(prevState);
}

void geButtonBase::buttonHover()
{
	m_bMouseHover=true;
}

void geButtonBase::buttonUnHover()
{
	m_bMouseHover=false;
}

bool geButtonBase::isButtonPressed()
{
	return (m_eState == BTN_STATE_PRESSED);
}

void geButtonBase::onCancelEngagedControls()
{
	buttonNormal();

	geGUIBase::onCancelEngagedControls();
}

void geButtonBase::onButtonClicked()
{

}

///////////////////////////////////////////////////
geButton::geButton():
	geButtonBase(GEGUI_BUTTON, "Button")
{
}

geButton::geButton(const char* name):
	geButtonBase(GEGUI_BUTTON, name)
{
}
geButton::geButton(unsigned short uGUIID, const char* name):
	geButtonBase(uGUIID, name)
{
}


geButton::~geButton()
{
}

void geButton::create(geGUIBase* parent, const char* name, float x, float y, float cx, float cy)
{
	createBase(parent);

	int width=geGUIManager::g_pFontArial10_84Ptr->calculateStringWidthInPixelTillNewLine(name, strlen(name), 0);
	setSize(cx, cy);
	setPos(x, y);

	STRCPY(m_szName, name);
	setColor(&m_cVBClientArea, 0.3, 0.3, 0.3, 1.0f, EGRADIENT_VERTICAL_UP, 0.3f);
	m_bMouseHover=false;
}

void geButton::create(geGUIBase* parent, const char* name, float x, float y)
{
	createBase(parent);
	int width=geGUIManager::g_pFontArial10_84Ptr->calculateStringWidthInPixelTillNewLine(name, strlen(name), 0);
	setSize(width+20, 20);
	setPos(x, y);

	STRCPY(m_szName, name);
	setColor(&m_cVBClientArea, 0.3, 0.3, 0.3, 1.0f, EGRADIENT_VERTICAL_UP, 0.3f);
	m_bMouseHover=false;
}

void geButton::draw()
{
	glPushMatrix();
	glTranslatef(m_cPos.x, m_cPos.y, 0);
	drawRect(&m_cVBClientArea);
	drawLine(m_cVBClientAreaLine, 0.1, 0.1, 0.1, 1.0f);
	geGUIManager::g_pFontArial10_84Ptr->drawString(m_szName, 10, geGUIManager::g_pFontArial10_84Ptr->getLineHeight()-2, m_cSize.x);
	glPopMatrix();
}
	
void geButton::onPosition(float x, float y, int flag)
{
}

void geButton::onSize(float cx, float cy, int flag)
{
	const float title_vertLst[8] =
	{
		cx,	0,
		0,		0,
		cx,	cy,
		0,		cy,
	};
	memcpy(m_cVBClientArea.m_cszVertexList, title_vertLst, sizeof(title_vertLst));

	const float clientarea_linevertLst[8] =
	{
		cx,	0,
		0,	0,
		0,	cy-0.5f,
		cx,	cy-0.5f,
	};
	memcpy(m_cVBClientAreaLine, clientarea_linevertLst, sizeof(clientarea_linevertLst));
}

bool geButton::onMouseLButtonDown(float x, float y, int nFlag)
{
	if(m_eState==BTN_STATE_NORMAL)
	{
		buttonPressed();
	}

	return true;
}

bool geButton::onMouseLButtonUp(float x, float y, int nFlag)
{
	if(m_eState==BTN_STATE_PRESSED)
	{
		buttonNormal();
		onButtonClicked();
	}

	return true;
}

bool geButton::onMouseMove(float x, float y, int flag)
{
	return true;
}

void geButton::onButtonStateChanged(EBUTTON_STATE eFromState)
{
	switch(m_eState)
	{
	case BTN_STATE_NORMAL:
		setColor(&m_cVBClientArea, 0.3, 0.3, 0.3, 1.0f, EGRADIENT_VERTICAL_UP, 0.3f);
		break;
	case BTN_STATE_PRESSED:
		setColor(&m_cVBClientArea, 0.3, 0.3, 0.3, 1.0f, EGRADIENT_VERTICAL_DOWN, 0.3f);
		break;
	}

	if(m_pGUIObserver)
		m_pGUIObserver->onButtonClicked(this);
}

void geButton::onMouseEnterClientArea()
{
	if(m_eState==BTN_STATE_PRESSED)
	{
		buttonHover();
		setColor(&m_cVBClientArea, 0.3, 0.3, 0.3, 1.0f, EGRADIENT_VERTICAL_DOWN, 0.3f);
	}
}

void geButton::onMouseExitClientArea()
{
	if(m_eState>=BTN_STATE_PRESSED)
	{
		buttonUnHover();
		setColor(&m_cVBClientArea, 0.3, 0.3, 0.3, 1.0f, EGRADIENT_VERTICAL_UP, 0.3f);
	}
}