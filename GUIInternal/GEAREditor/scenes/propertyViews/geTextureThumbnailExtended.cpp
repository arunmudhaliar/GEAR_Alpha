#include "geTextureThumbnailExtended.h"
#include "../../secondryViews/geTextureDlg.h"
#include "../../EditorApp.h"

bool geTextureThumbnailExtended::onMouseLButtonDown(float x, float y, int nFlag)
{
    geTextureDlg* newTextureDlg	 = new geTextureDlg(this, m_pFontManagerPtr, m_pRenderer);
#if DEPRECATED
	newTextureDlg->showView(EditorGEARApp::getMainWindowHandle());
#else
    newTextureDlg->showView();
#endif
	return true;
}

//#if !defined(__APPLE__) //disable Drag-Drop
void geTextureThumbnailExtended::onDragDrop(int x, int y, MDropData* dropObject)
{
	std::vector<geGUIBase*>* list = dropObject->getActualDataList();
	for(std::vector<geGUIBase*>::iterator it = list->begin(); it != list->end(); ++it)
	{
		geGUIBase* droppedDataObject = *it;
		const char* relativePath=((assetUserData*)((geTreeNode*)droppedDataObject)->getUserData())->getAssetPath();

		if(util::GE_IS_EXTENSION(relativePath, ".png") || util::GE_IS_EXTENSION(relativePath, ".PNG") ||
			util::GE_IS_EXTENSION(relativePath, ".tga") || util::GE_IS_EXTENSION(relativePath, ".TGA") ||
			util::GE_IS_EXTENSION(relativePath, ".bmp") || util::GE_IS_EXTENSION(relativePath, ".BMP") ||
			util::GE_IS_EXTENSION(relativePath, ".ico") || util::GE_IS_EXTENSION(relativePath, ".ICO") ||
			util::GE_IS_EXTENSION(relativePath, ".jpeg") || util::GE_IS_EXTENSION(relativePath, ".JPEG") ||
			util::GE_IS_EXTENSION(relativePath, ".pcx") || util::GE_IS_EXTENSION(relativePath, ".PCX") ||
			util::GE_IS_EXTENSION(relativePath, ".tif") || util::GE_IS_EXTENSION(relativePath, ".TIF") ||
			util::GE_IS_EXTENSION(relativePath, ".psd") || util::GE_IS_EXTENSION(relativePath, ".PSD") ||
			util::GE_IS_EXTENSION(relativePath, ".gif") || util::GE_IS_EXTENSION(relativePath, ".GIF") ||
			util::GE_IS_EXTENSION(relativePath, ".hdr") || util::GE_IS_EXTENSION(relativePath, ".HDR")
			)
		{
			gxSubMap* map = (gxSubMap*)getUserData();
			if(map)
			{
				char absolutepath[FILENAME_MAX];
				sprintf(absolutepath, "%s/Assets%s", EditorGEARApp::getProjectHomeDirectory(), relativePath);
				m_pTexturePtr=map->load(*monoWrapper::mono_engine_getWorld(0)->getTextureManager(), absolutepath);
			}
		}
	}
}
//#endif