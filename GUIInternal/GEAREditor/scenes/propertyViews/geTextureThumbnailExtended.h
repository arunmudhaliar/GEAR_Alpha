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

	virtual void onDragDrop(int x, int y, MDataObject* dropObject)
	{
		//geTreeNode* rootNode = m_cGameObjectsTreeView.getRoot();
		std::vector<geGUIBase*>* list = dropObject->getActualDataList();
		for(std::vector<geGUIBase*>::iterator it = list->begin(); it != list->end(); ++it)
		{
			geGUIBase* droppedDataObject = *it;
			const char* absolutePath=((assetUserData*)((geTreeNode*)droppedDataObject)->getUserData())->getAssetAbsolutePath();

			if (util::GE_IS_EXTENSION(absolutePath, ".png") || util::GE_IS_EXTENSION(absolutePath, ".PNG") ||
				util::GE_IS_EXTENSION(absolutePath, ".tga") || util::GE_IS_EXTENSION(absolutePath, ".TGA"))
			{
				gxSubMap* map = (gxSubMap*)getUserData();
				if(map)
				{
					m_pTexturePtr=map->load(*monoWrapper::mono_engine_getWorld(0)->getTextureManager(), absolutePath);
				}
			//	object3d* obj = engine_loadAndAppendFBX(EditorApp::getSceneWorldEditor()->getMainWorld(), absolutePath);
			//	createTVNode(rootNode, obj, droppedDataObject->getName());
			}
		}
		//rootNode->traverseSetWidth(m_cSize.x);
		//m_cGameObjectsTreeView.refreshTreeView();
	}

	virtual bool onMouseLButtonDown(float x, float y, int nFlag);

};

#endif