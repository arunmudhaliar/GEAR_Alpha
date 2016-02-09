#include "gePushButton.h"
#include "geGUIManager.h"

gePushButton::gePushButton(geFontManager* fontManager):
	geButtonBase(GEGUI_PUSH_BUTTON, "Push Button", fontManager)
{
}

gePushButton::gePushButton(const char* name, geFontManager* fontManager):
	geButtonBase(GEGUI_PUSH_BUTTON, name, fontManager)
{
}

gePushButton::gePushButton(unsigned short uGUIID, const char* name, geFontManager* fontManager):
	geButtonBase(uGUIID, name, fontManager)
{
}


gePushButton::~gePushButton()
{
}

void gePushButton::create(rendererGL10* renderer, geGUIBase* parent, const char* name, float x, float y)
{
	createBase(renderer, parent);
//	int width=geFontManager::g_pFontArial10_84Ptr->calculateStringWidthInPixelTillNewLine(name, strlen(name), 0);
	setSize(14, 14);
	setPos(x, y);

	STRCPY(m_szName, name);
	setClientAreaPrimaryActiveForeColor(0.3, 0.3, 0.3, 1.0f);
	applyPrimaryColorToVBClientArea(EGRADIENT_VERTICAL_UP, 0.3f);
	setColor(&vertexBufferCheckMark, 0.7, 0.7, 0.7, 1.0f, EGRADIENT_VERTICAL_DOWN, 0.5f/**/);

	isMouseHover=false;
	is_Check=false;
}

void gePushButton::draw()
{
	glPushMatrix();
	glTranslatef(m_cPos.x, m_cPos.y, 0);
	drawRect(&vertexBufferClientArea);
	drawLine(vertexBufferClientAreaArray, 0.1, 0.1, 0.1, 1.0f);
	if(is_Check)
	{
		drawRect(&vertexBufferCheckMark);
	}
	geFontManager::g_pFontArial10_84Ptr->drawString(m_szName, 17, geFontManager::g_pFontArial10_84Ptr->getLineHeight()-5, m_cSize.x);
	glPopMatrix();
}
	
void gePushButton::onPosition(float x, float y, int flag)
{
}

void gePushButton::onSize(float cx, float cy, int flag)
{
	const float title_vertLst[8] =
	{
		cx,	0,
		0,		0,
		cx,	cy,
		0,		cy,
	};
	memcpy(vertexBufferClientArea.vertexArray, title_vertLst, sizeof(title_vertLst));


	const float checkMark_vertLst[8] =
	{
		cx-3,	3,
		3,		3,
		cx-3,	cy-3,
		3,		cy-3,
	};
	memcpy(vertexBufferCheckMark.vertexArray, checkMark_vertLst, sizeof(checkMark_vertLst));
	

	const float clientarea_linevertLst[8] =
	{
		cx,	0,
		0,	0,
		0,	cy-0.5f,
		cx,	cy-0.5f,
	};
	memcpy(vertexBufferClientAreaArray, clientarea_linevertLst, sizeof(clientarea_linevertLst));
}

bool gePushButton::onMouseLButtonDown(float x, float y, int nFlag)
{
	if(buttonState==BTN_STATE_NORMAL)
	{
		buttonPressed(true);
	}

	return true;
}

bool gePushButton::onMouseLButtonUp(float x, float y, int nFlag)
{
	if(buttonState==BTN_STATE_PRESSED)
	{
		onButtonClicked();
		buttonNormal(false);
	}

	return true;
}

bool gePushButton::onMouseMove(float x, float y, int flag)
{
	return true;
}

void gePushButton::refresh()
{
	switch(buttonState)
	{
	case BTN_STATE_NORMAL:
		applyPrimaryColorToVBClientArea(EGRADIENT_VERTICAL_UP, 0.3f);
		break;
	case BTN_STATE_PRESSED:
		applyPrimaryColorToVBClientArea(EGRADIENT_VERTICAL_DOWN, 0.3f);
		break;
	case BTN_STATE_CANCEL:
		applyPrimaryColorToVBClientArea(EGRADIENT_VERTICAL_UP, 0.3f);
		break;
	}
}

void gePushButton::onButtonStateChanged(EBUTTON_STATE eFromState, bool dontPassEventToObserver)
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

void gePushButton::onMouseEnterClientArea()
{
	if(buttonState==BTN_STATE_PRESSED)
	{
		buttonHover();
		applyPrimaryColorToVBClientArea(EGRADIENT_VERTICAL_DOWN, 0.3f);
	}
}

void gePushButton::onMouseExitClientArea()
{
	if(buttonState>=BTN_STATE_PRESSED)
	{
		buttonUnHover();
		applyPrimaryColorToVBClientArea(EGRADIENT_VERTICAL_UP, 0.3f);
	}
}

void gePushButton::onButtonClicked()
{
	is_Check=!is_Check;
}