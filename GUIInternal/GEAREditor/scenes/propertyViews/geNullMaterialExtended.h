#ifndef GENULLMATERIALEXTENDED_H
#define GENULLMATERIALEXTENDED_H

#include "../../gui/geTextureThumbnail.h"
#include "../assetUserData.h"
#include "../../../../GEAREngine/src/mono/src/monoWrapper.h"

class geNullMaterialExtended : public geTextureThumbnail
{
public:
	geNullMaterialExtended(geFontManager* fontmanager):
	  geTextureThumbnail(fontmanager)
	{
	}

	~geNullMaterialExtended()
	{
	}

	virtual void draw();
//#if !defined(GEAR_APPLE) //disable Drag-Drop
	virtual void onDragDrop(int x, int y, MDropData* dropObject)
	{
		getParent()->DragDrop(x-getPos().x, y-getPos().y, dropObject);
		////geTreeNode* rootNode = gameObjectTreeView->getRoot();
		//geGUIBase* droppedDataObject = dropObject->getActualData();
		//const char* absolutePath=((assetUserData*)((geTreeNode*)droppedDataObject)->getUserData())->getAssetAbsolutePath();

		//if (util::GE_IS_EXTENSION(absolutePath, ".png") || util::GE_IS_EXTENSION(absolutePath, ".PNG") ||
		//	util::GE_IS_EXTENSION(absolutePath, ".tga") || util::GE_IS_EXTENSION(absolutePath, ".TGA"))
		//{
		//	gxMaterial* material = (gxMaterial*)getUserData();
		//	if(material)
		//	{
		//		texture=monoWrapper::mono_engine_loadTextureFromFile(monoWrapper::mono_engine_getWorld(0), material, absolutePath, 0);
		//	}
		//}
	}
//#endif
};

#endif