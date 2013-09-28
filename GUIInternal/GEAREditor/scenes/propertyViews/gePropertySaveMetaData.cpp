#include "gePropertySaveMetaData.h"
#include "../../AssetImporter.h"
#include "../../EditorApp.h"

void gePropertySaveMetaData::onButtonClicked(geGUIBase* btn)
{
	if(btn==m_pButtonSave)
	{
		if(m_pButtonSave->isButtonPressed())
		{
			object3d* obj=m_pObject3dPtr;
			if(obj && obj->getParent()==monoWrapper::mono_engine_getWorld(0) && obj->getFileCRC()!=0)
			{
				//save metadata
				stMetaHeader metaHeader;
				struct stat fst;

				if(AssetImporter::readMetaHeader(obj->getFileCRC(), metaHeader, fst))
				{
					char crcFileName[512];
					sprintf(crcFileName, "%s/%s/%x", EditorApp::getProjectHomeDirectory(), "MetaData", obj->getFileCRC());
					AssetImporter::saveObject3DToMetaData(crcFileName, obj, fst);
				}
			}
		}
	}
}
