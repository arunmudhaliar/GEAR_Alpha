#ifndef GEAR2D
#include "geTextureThumbnail.h"
#include "geGUIManager.h"

geTextureThumbnail::geTextureThumbnail():
	geGUIBase(GEGUI_TEXTURE_THUMBNAIL, "TextureThumbnail")
{
	m_pTexturePtr=NULL;
}

geTextureThumbnail::~geTextureThumbnail()
{
}

void geTextureThumbnail::create(rendererGL10* renderer, geGUIBase* parent, gxTexture* texture, float x, float y, float cx, float cy)
{
	createBase(renderer, parent);

	setSize(cx, cy);
	setPos(x, y);

	setClientAreaPrimaryActiveForeColor(0.21f, 0.21f, 0.21f, 1.0f);
	applyPrimaryColorToVBClientArea();

	m_pTexturePtr=texture;
}

void geTextureThumbnail::draw()
{
	//if(m_pTexturePtr==NULL) return;

	glPushMatrix();
	glTranslatef(m_cPos.x, m_cPos.y, 0);

	if(m_pTexturePtr)
		drawRect(&m_cVBClientArea, m_cszTextureCoord, m_pTexturePtr->getTextureID());
	else
		drawRect(&m_cVBClientArea);

	drawLine(m_cVBClientAreaLine, 0.13f, 0.13f, 0.13f, 1.0f, 3, false);
	drawLine(&m_cVBClientAreaLine[4], 0.3f, 0.3f, 0.3f, 1.0f, 3, false);
	glPopMatrix();
}
	
void geTextureThumbnail::onPosition(float x, float y, int flag)
{
}

void geTextureThumbnail::onSize(float cx, float cy, int flag)
{
	const float title_vertLst[8] =
	{
		cx,	0,
		0,		0,
		cx,	cy,
		0,		cy,
	};
	memcpy(m_cVBClientArea.m_cszVertexList, title_vertLst, sizeof(title_vertLst));

	const float uvLst[8] =
	{
		1,	0,
		0,	0,
		1,	1,
		0,	1,
	};
	memcpy(m_cszTextureCoord, uvLst, sizeof(uvLst));

	
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

bool geTextureThumbnail::onMouseLButtonDown(float x, float y, int nFlag)
{
	return true;
}

bool geTextureThumbnail::onMouseLButtonUp(float x, float y, int nFlag)
{
	return true;
}

bool geTextureThumbnail::onMouseMove(float x, float y, int flag)
{
	return true;
}
#endif