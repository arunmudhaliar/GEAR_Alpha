#include "geTextureThumbnailExtended.h"
#include "../../secondryViews/geTextureDlg.h"
#include "../../EditorApp.h"

bool geTextureThumbnailExtended::onMouseLButtonDown(float x, float y, int nFlag)
{
	geTextureDlg* newTextureDlg	 = new geTextureDlg(this);
	newTextureDlg->showView(EditorApp::getMainWindowHandle());

	return true;
}

void geTextureThumbnailExtended::onDragDrop(int x, int y, MDataObject* dropObject)
{
	std::vector<geGUIBase*>* list = dropObject->getActualDataList();
	for(std::vector<geGUIBase*>::iterator it = list->begin(); it != list->end(); ++it)
	{
		geGUIBase* droppedDataObject = *it;
		const char* relativePath=((assetUserData*)((geTreeNode*)droppedDataObject)->getUserData())->getAssetPath();

		if (util::GE_IS_EXTENSION(relativePath, ".png") || util::GE_IS_EXTENSION(relativePath, ".PNG") ||
			util::GE_IS_EXTENSION(relativePath, ".tga") || util::GE_IS_EXTENSION(relativePath, ".TGA"))
		{
			gxSubMap* map = (gxSubMap*)getUserData();
			if(map)
			{
				char absolutepath[1024];
				sprintf(absolutepath, "%s/Assets%s", EditorApp::getProjectHomeDirectory(), relativePath);
				m_pTexturePtr=map->load(*monoWrapper::mono_engine_getWorld(0)->getTextureManager(), absolutepath);
			}
		}
	}
}