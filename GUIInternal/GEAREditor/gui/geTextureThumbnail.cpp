#ifndef GEAR2D
#include "geTextureThumbnail.h"
#include "geGUIManager.h"

geTextureThumbnail::geTextureThumbnail(geFontManager* fontmanager):
	geGUIBase(GEGUI_TEXTURE_THUMBNAIL, "TextureThumbnail", fontmanager)
{
	texture=NULL;
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

	this->texture=texture;
}

void geTextureThumbnail::draw()
{
	//if(texture==NULL) return;

	glPushMatrix();
	glTranslatef(m_cPos.x, m_cPos.y, 0);

	if(texture)
		drawRect(&vertexBufferClientArea, textureCoordinateArray, texture->getTextureID());
	else
		drawRect(&vertexBufferClientArea);

	drawLine(vertexBufferClientAreaArray, 0.13f, 0.13f, 0.13f, 1.0f, 3, false);
	drawLine(&vertexBufferClientAreaArray[4], 0.3f, 0.3f, 0.3f, 1.0f, 3, false);
	glPopMatrix();
}
	
void geTextureThumbnail::onPosition(float x, float y, int flag)
{
}

void geTextureThumbnail::onSize(float cx, float cy, int flag)
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
    
	const float uvLst[8] =
	{
		1,	0,
		0,	0,
		1,	1,
		0,	1,
	};
	memcpy(textureCoordinateArray, uvLst, sizeof(uvLst));

	
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