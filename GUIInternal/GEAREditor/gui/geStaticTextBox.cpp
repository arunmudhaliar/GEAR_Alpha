#include "geStaticTextBox.h"
#include "geGUIManager.h"

geStaticTextBox::geStaticTextBox():
geGUIBase(GEGUI_STATICTEXTBOX, "Static Text Box")
{
	m_pFont=NULL;
	m_fYOffset=0.0f;
}

geStaticTextBox::geStaticTextBox(const char* name):
	geGUIBase(GEGUI_STATICTEXTBOX, name)
{
	m_pFont=NULL;
	m_fYOffset=0.0f;
}

geStaticTextBox::~geStaticTextBox()
{
}

void geStaticTextBox::create(rendererGL10* renderer, geGUIBase* parent, const char* name, float x, float y, float yoffset, geFont* pFont)
{
	createBase(renderer, parent);

	m_pFont=pFont;
	m_fYOffset=yoffset;
	int width=m_pFont->calculateStringWidthInPixelTillNewLine(name, strlen(name), 0);
	setSize(width, m_pFont->getLineHeight());
	setPos(x, y);

	STRCPY(m_szName, name);
	setColor(&m_cVBClientArea, 0.21f, 0.21f, 0.21f, 1.0f);
	setColor(&m_cVBSelectionArea, 0.24f, 0.38f, 0.57f, 1.0f);
}

void geStaticTextBox::draw()
{
	glPushMatrix();
	glTranslatef(m_cPos.x, m_cPos.y, 0);
	//drawRect(&m_cVBClientArea);
	m_pFont->drawString(m_szName, 0, m_pFont->getLineHeight()+m_fYOffset, m_cSize.x);
	glPopMatrix();
}
	
void geStaticTextBox::onPosition(float x, float y, int flag)
{
}

void geStaticTextBox::onSize(float cx, float cy, int flag)
{
	const float title_vertLst[8] =
	{
		cx,	0,
		0,		0,
		cx,	cy,
		0,		cy,
	};
	memcpy(m_cVBClientArea.m_cszVertexList, title_vertLst, sizeof(title_vertLst));

	const float clientarea_linevertLst[10] =
	{
		cx,	0,
		0,	0,
		0,	cy-0.5f,
		cx,	cy-0.5f,
		cx,	0,
	};
	memcpy(m_cVBClientAreaLine, clientarea_linevertLst, sizeof(clientarea_linevertLst));
}

void geStaticTextBox::onSetName()
{
	if(m_pGUIObserver)
		m_pGUIObserver->onTextChange(this);
}