#ifndef GETEXTURETHUMBNAILEXTENDED_H
#define GETEXTURETHUMBNAILEXTENDED_H

#include "../../gui/geTextureThumbnail.h"
#include "../assetUserData.h"
#include "../../../../GEAREngine/src/mono/src/monoWrapper.h"
#include "../../gui/geTreeView.h"

class geTextureThumbnailExtended : public geTextureThumbnail
{
public:
	geTextureThumbnailExtended(geFontManager* fontmanager):
	  geTextureThumbnail(fontmanager)
	{
	}

	~geTextureThumbnailExtended()
	{
	}

//#if !defined(GEAR_APPLE) //disable Drag-Drop
	virtual void onDragDrop(int x, int y, MDropData* dropObject);
//#endif
	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
};

#endif