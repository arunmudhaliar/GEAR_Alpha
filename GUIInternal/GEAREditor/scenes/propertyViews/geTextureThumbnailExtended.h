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
		geGUIBase* droppedDataObject = dropObject->getActualData();
		const char* absolutePath=((assetUserData*)((geTreeNode*)droppedDataObject)->getUserData())->getAssetAbsolutePath();

		if (util::GE_IS_EXTENSION(absolutePath, ".png") || util::GE_IS_EXTENSION(absolutePath, ".PNG") ||
			util::GE_IS_EXTENSION(absolutePath, ".tga") || util::GE_IS_EXTENSION(absolutePath, ".TGA"))
		{
			gxMaterial* material = (gxMaterial*)getUserData();
			if(material)
			{
				m_pTexturePtr=monoWrapper::mono_engine_loadTextureFromFile(monoWrapper::mono_engine_getWorld(0), material, absolutePath, 0);
			}
		//	object3d* obj = engine_loadAndAppendFBX(EditorApp::getSceneWorldEditor()->getMainWorld(), absolutePath);
		//	createTVNode(rootNode, obj, droppedDataObject->getName());
		}

		//rootNode->traverseSetWidth(m_cSize.x);
		//m_cGameObjectsTreeView.refreshTreeView();
	}

	virtual bool onMouseLButtonDown(float x, float y, int nFlag);

};

#endif