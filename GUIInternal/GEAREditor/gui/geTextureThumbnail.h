#ifndef GETEXTURETHUMBNAIL_H
#define GETEXTURETHUMBNAIL_H

#include "geGUIBase.h"
#include "../../../GEAREngine/src/core/gxTexture.h"

class geTextureThumbnail : public geGUIBase
{
public:
	geTextureThumbnail();

public:
	virtual ~geTextureThumbnail();

	void create(geGUIBase* parent, gxTexture* texture, float x, float y, float cx, float cy);
	virtual void draw();
	
protected:
	virtual void onPosition(float x, float y, int flag);
	virtual void onSize(float cx, float cy, int flag);

	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
	virtual bool onMouseLButtonUp(float x, float y, int nFlag);
	virtual bool onMouseMove(float x, float y, int flag);

	gxTexture* m_pTexturePtr;
private:

	stVertexBuffer m_cVBClientArea;

	float m_cszTextureCoord[8];
	float m_cVBClientAreaLine[10];
};

#endif