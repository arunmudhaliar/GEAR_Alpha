#include "geSeperator.h"

geSeperator::geSeperator(geFontManager* fontmanager):
	geGUIBase(GEGUI_SEPERATOR, "Seperator", fontmanager)
{
}

geSeperator::~geSeperator()
{
}

void geSeperator::create(rendererGL10* renderer, geGUIBase* parent, float x, float y, float cx)
{
	createBase(renderer, parent);

	setSize(cx, 3);
	setPos(x, y);

	setStyle(STYLE_3D);

	setMouseBoundCheck(false);
}

void geSeperator::setStyle(ESEPERATOR_STYPE style)
{
	seperatorStyle=style;
}

void geSeperator::draw()
{
	glPushMatrix();
	glTranslatef(m_cPos.x, m_cPos.y, 0);
	drawLine(&vertexBufferClientAreaArray[4], 0.25f, 0.25f, 0.25f, 1.0f, 2, false);
	if(seperatorStyle==STYLE_3D)
		drawLine(vertexBufferClientAreaArray, 0.17f, 0.17f, 0.17f, 1.0f, 2, false);
	glPopMatrix();
}

void geSeperator::onSize(float cx, float cy, int flag)
{
	const float clientarea_linevertLst[8] =
	{
		0,	0,
		cx,	0,
		0,	1,
		cx,	1
	};
	memcpy(vertexBufferClientAreaArray, clientarea_linevertLst, sizeof(clientarea_linevertLst));
}