#ifndef GETEXTURETHUMBNAILEXTENDED_H
#define GETEXTURETHUMBNAILEXTENDED_H

#include "../../gui/geTextureThumbnail.h"
#include "../assetUserData.h"
#include "../../../mono/src/monoWrapper.h"
#include "../../gui/geTreeView.h"

class geTextureThumbnailExtended : public geTextureThumbnail
{
public:
	geTextureThumbnailExtended():
	  geTextureThumbnail()
	{
	}

	~geTextureThumbnailExtended()
	{
	}

	virtual void onDragDrop(int x, int y, MDataObject* dropObject);
	virtual bool onMouseLButtonDown(float x, float y, int nFlag);

};

#endif