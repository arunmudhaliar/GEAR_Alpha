#ifndef GEAR2D
#pragma once

#include "geGUIBase.h"
#include "../../../GEAREngine/src/core/gxTexture.h"

class geTextureThumbnail : public geGUIBase
{
public:
	geTextureThumbnail(geFontManager* fontmanager);

public:
	virtual ~geTextureThumbnail();

	void create(rendererGL10* renderer, geGUIBase* parent, gxTexture* texture, float x, float y, float cx, float cy);
	virtual void draw();
	
	gxTexture* getTexturePtr()	{	return texture;	}

protected:
	virtual void onPosition(float x, float y, int flag);
	virtual void onSize(float cx, float cy, int flag);

	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
	virtual bool onMouseLButtonUp(float x, float y, int nFlag);
	virtual bool onMouseMove(float x, float y, int flag);

	gxTexture* texture;

private:
	float textureCoordinateArray[8];
	float vertexBufferClientAreaArray[10];
};

#endif