#include "geButton.h"
#include "geGUIManager.h"

geButtonBase::geButtonBase(unsigned short uGUIID, const char* name, geFontManager* fontManager):
	geGUIBase(uGUIID, name, fontManager)
{
	buttonState = BTN_STATE_NORMAL;
	isMouseHover=false;
}

geButtonBase::~geButtonBase()
{
}

void geButtonBase::buttonPressed(bool dontPassEventToObserver)
{
	EBUTTON_STATE prevState=buttonState;
	buttonState=BTN_STATE_PRESSED;
	onButtonStateChanged(prevState, dontPassEventToObserver);
}

void geButtonBase::buttonNormal(bool dontPassEventToObserver)
{
	EBUTTON_STATE prevState=buttonState;
	buttonState = BTN_STATE_NORMAL;
	onButtonStateChanged(prevState, dontPassEventToObserver);
}

void geButtonBase::buttonCancel()
{
	EBUTTON_STATE prevState=buttonState;
	buttonState = BTN_STATE_CANCEL;
	onButtonStateChanged(prevState, true);
}

void geButtonBase::buttonHover()
{
	isMouseHover=true;
}

void geButtonBase::buttonUnHover()
{
	isMouseHover=false;
}

bool geButtonBase::isButtonPressed()
{
	return (buttonState == BTN_STATE_PRESSED);
}

void geButtonBase::onCancelEngagedControls()
{
	buttonCancel();

	geGUIBase::onCancelEngagedControls();
}

void geButtonBase::onButtonClicked()
{

}

///////////////////////////////////////////////////
geButton::geButton(geFontManager* fontManager):
	geButtonBase(GEGUI_BUTTON, "Button", fontManager)
{
}

geButton::geButton(const char* name, geFontManager* fontManager):
	geButtonBase(GEGUI_BUTTON, name, fontManager)
{
}
geButton::geButton(unsigned short uGUIID, const char* name, geFontManager* fontManager):
	geButtonBase(uGUIID, name, fontManager)
{
}


geButton::~geButton()
{
}

void geButton::create(rendererGL10* renderer, geGUIBase* parent, const char* name, float x, float y, float cx, float cy)
{
	createBase(renderer, parent);

	//int width=geFontManager::g_pFontArial10_84Ptr->calculateStringWidthInPixelTillNewLine(name, strlen(name), 0);
	setSize(cx, cy);
	setPos(x, y);

	STRCPY(m_szName, name);
	setClientAreaPrimaryActiveForeColor(0.3, 0.3, 0.3, 1.0f);
	applyPrimaryColorToVBClientArea(EGRADIENT_VERTICAL_UP, 0.3f);

	isMouseHover=false;
}

void geButton::create(rendererGL10* renderer, geGUIBase* parent, const char* name, float x, float y)
{
	createBase(renderer, parent);
	int width=geFontManager::g_pFontArial10_84Ptr->calculateStringWidthInPixelTillNewLine(name, (int)strlen(name), 0);
	setSize(width+20, 20);
	setPos(x, y);

	STRCPY(m_szName, name);
	setClientAreaPrimaryActiveForeColor(0.3, 0.3, 0.3, 1.0f);
	applyPrimaryColorToVBClientArea(EGRADIENT_VERTICAL_UP, 0.3f);

	setClientAreaSecondryActiveForeColor(0.3, 0.3, 0.3, 1.0f);
	isMouseHover=false;
}

void geButton::draw()
{
	glPushMatrix();
	glTranslatef(m_cPos.x, m_cPos.y, 0);
	drawRect(&vertexBufferClientArea);
	drawLine(vertexBufferClientAreaArray, 0.1, 0.1, 0.1, 1.0f);
	geFontManager::g_pFontArial10_84Ptr->drawString(m_szName, 10, geFontManager::g_pFontArial10_84Ptr->getLineHeight()-2, m_cSize.x);
	glPopMatrix();
}
	
void geButton::onPosition(float x, float y, int flag)
{
}

void geButton::onSize(float cx, float cy, int flag)
{
    vertexBufferClientArea.updateRect(0, 0, cx, cy);
    
	const float clientarea_linevertLst[8] =
	{
		cx,	0,
		0,	0,
		0,	cy-0.5f,
		cx,	cy-0.5f,
	};
	memcpy(vertexBufferClientAreaArray, clientarea_linevertLst, sizeof(clientarea_linevertLst));
}

bool geButton::onMouseLButtonDown(float x, float y, int nFlag)
{
	if(buttonState==BTN_STATE_NORMAL)
	{
		buttonPressed(false);
	}

	return true;
}

bool geButton::onMouseLButtonUp(float x, float y, int nFlag)
{
	if(buttonState==BTN_STATE_PRESSED)
	{
		onButtonClicked();
		buttonNormal(false);
	}

	return true;
}

bool geButton::onMouseMove(float x, float y, int flag)
{
	return true;
}

void geButton::onButtonStateChanged(EBUTTON_STATE eFromState, bool dontPassEventToObserver)
{
	bool bClicked=false;
	switch(buttonState)
	{
	case BTN_STATE_NORMAL:
		applyPrimaryColorToVBClientArea(EGRADIENT_VERTICAL_UP, 0.3f);
		bClicked=true;
		break;
	case BTN_STATE_PRESSED:
		applyPrimaryColorToVBClientArea(EGRADIENT_VERTICAL_DOWN, 0.3f);
		bClicked=true;
		break;
	case BTN_STATE_CANCEL:
		applyPrimaryColorToVBClientArea(EGRADIENT_VERTICAL_UP, 0.3f);
		buttonState=BTN_STATE_NORMAL;
		break;
	}

	if(guiObserver && bClicked && !dontPassEventToObserver)
		guiObserver->onButtonClicked(this);
}

void geButton::onMouseEnterClientArea()
{
	if(buttonState==BTN_STATE_PRESSED)
	{
		buttonHover();
		applyPrimaryColorToVBClientArea(EGRADIENT_VERTICAL_DOWN, 0.3f);
	}
}

void geButton::onMouseExitClientArea()
{
	if(buttonState>=BTN_STATE_PRESSED)
	{
		buttonUnHover();
		applyPrimaryColorToVBClientArea(EGRADIENT_VERTICAL_UP, 0.3f);
	}
}