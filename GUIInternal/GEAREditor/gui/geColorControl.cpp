#ifndef GEAR2D
#include "geColorControl.h"
#include "geGUIManager.h"
#include "../secondryViews/geColorDlg.h"
#ifndef GEAR2D
#include "../EditorApp.h"
#else
#include "../../../../GEAR2D_Alpha/GEAR2D/app/Editor2DApp.h"
#endif

geColorControl::geColorControl(geFontManager* fontManager):
	geGUIBase(GEGUI_COLOR_CONTROL, "Color Control", fontManager)
{
}

geColorControl::~geColorControl()
{
}

void geColorControl::create(rendererGL10* renderer, geGUIBase* parent, float x, float y)
{
	createBase(renderer, parent, x, y, 16, 16);

	setClientAreaPrimaryActiveForeColor(0.21f, 0.21f, 0.21f, 1.0f);
	applyPrimaryColorToVBClientArea();
}

void geColorControl::setControlColor(float r, float g, float b, float a)
{
	diffuseColor.set(r, g, b, a);
	setClientAreaPrimaryActiveForeColor(r, g, b, a);
	applyPrimaryColorToVBClientArea();
	if(guiObserver)
		guiObserver->onColorChange(this);
}

void geColorControl::draw()
{
	glPushMatrix();
	glTranslatef(m_cPos.x, m_cPos.y, 0);
	drawRect(&vertexBufferClientArea);
	drawLine(vertexBufferClientAreaArray, 0.13f, 0.13f, 0.13f, 1.0f, 3, false);
	drawLine(&vertexBufferClientAreaArray[4], 0.3f, 0.3f, 0.3f, 1.0f, 3, false);
	glPopMatrix();
}
	
void geColorControl::onPosition(float x, float y, int flag)
{
}

void geColorControl::onSize(float cx, float cy, int flag)
{
//	const float title_vertLst[8] =
//	{
//		cx,	0,
//		0,		0,
//		cx,	cy,
//		0,		cy,
//	};
//	memcpy(vertexBufferClientArea.vertexArray, title_vertLst, sizeof(title_vertLst));
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

bool geColorControl::onMouseLButtonDown(float x, float y, int nFlag)
{
    geColorDlg* view = new geColorDlg(this, fontManagerGUI, rendererGUI);
    view->showView();
    
	return true;
}

bool geColorControl::onMouseLButtonUp(float x, float y, int nFlag)
{
	return true;
}

bool geColorControl::onMouseMove(float x, float y, int flag)
{
	return true;
}
#endif