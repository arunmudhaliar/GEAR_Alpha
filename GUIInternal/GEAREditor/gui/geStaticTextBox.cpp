#include "geStaticTextBox.h"
#include "geGUIManager.h"

geStaticTextBox::geStaticTextBox(geFontManager* fontmanager):
geGUIBase(GEGUI_STATICTEXTBOX, "Static Text Box", fontmanager)
{
	fontGUI=NULL;
	fontYOffset=0.0f;
}

geStaticTextBox::geStaticTextBox(const char* name, geFontManager* fontmanager):
	geGUIBase(GEGUI_STATICTEXTBOX, name, fontmanager)
{
	fontGUI=NULL;
	fontYOffset=0.0f;
}

geStaticTextBox::~geStaticTextBox()
{
}

void geStaticTextBox::create(rendererGL10* renderer, geGUIBase* parent, const char* name, float x, float y, float yoffset, geFont* pFont)
{
	createBase(renderer, parent);

	fontGUI=pFont;
	fontYOffset=yoffset;
	int width=fontGUI->calculateStringWidthInPixelTillNewLine(name, (int)strlen(name), 0);
    
    setName(name);
	setSize(width, fontGUI->getLineHeight());
	setPos(x, y);

	setClientAreaPrimaryActiveForeColor(0.21f, 0.21f, 0.21f, 1.0f);
	applyPrimaryColorToVBClientArea();
	setColor(&vertexBufferSelectionArea, 0.24f, 0.38f, 0.57f, 1.0f);
}

void geStaticTextBox::draw()
{
	glPushMatrix();
	glTranslatef(m_cPos.x, m_cPos.y, 0);
	//drawRect(&vertexBufferClientArea);
	fontGUI->drawString(m_szName, 0, fontGUI->getLineHeight()+fontYOffset, m_cSize.x);
	glPopMatrix();
}
	
void geStaticTextBox::onPosition(float x, float y, int flag)
{
}

void geStaticTextBox::onSize(float cx, float cy, int flag)
{
    vertexBufferClientArea.updateRect(0, 0, cx, cy);
    
	const float clientarea_linevertLst[10] =
	{
		cx,	0,
		0,	0,
		0,	cy-0.5f,
		cx,	cy-0.5f,
		cx,	0,
	};
    
    
	memcpy(vertexBufferClientAreaArray, clientarea_linevertLst, sizeof(clientarea_linevertLst));
}

void geStaticTextBox::onSetName()
{
	if(guiObserver)
		guiObserver->onTextChange(this);
}