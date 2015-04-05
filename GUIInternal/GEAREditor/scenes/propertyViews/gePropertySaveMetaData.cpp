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
			if(obj && obj->getParent()==monoWrapper::mono_engine_getWorld(0) && obj->getAssetFileCRC()!=0)
			{
				//save metadata
				stMetaHeader metaHeader;
				struct stat fst;

				if(AssetImporter::readMetaHeader(obj->getAssetFileCRC(), metaHeader, fst))
				{
					char crcFileName[512];
					sprintf(crcFileName, "%s/%s/%x", EditorGEARApp::getProjectHomeDirectory(), "MetaData", obj->getAssetFileCRC());
					AssetImporter::saveObject3DToMetaData(crcFileName, obj, fst);
					saveMaterialRecursiveToMeta(obj);
				}
			}
		}
	}
}

void gePropertySaveMetaData::saveMaterialRecursiveToMeta(object3d* obj)
{
	if(obj->getID()==OBJECT3D_MESH || obj->getID()==OBJECT3D_SKINNED_MESH)
	{
		gxMesh* mesh=(gxMesh*)obj;
		for(int x=0;x<mesh->getNoOfTriInfo();x++)
		{
			gxTriInfo* triinfo=mesh->getTriInfo(x);
			gxMaterial* material=triinfo->getMaterial();
			if(material)
			{
				stMetaHeader metaHeader;
				struct stat fst;
				if(AssetImporter::readMetaHeader(material->getAssetFileCRC(), metaHeader, fst))
				{
					char crcFileName[512];
					sprintf(crcFileName, "%s/%s/%x", EditorGEARApp::getProjectHomeDirectory(), "MetaData", material->getAssetFileCRC());
					AssetImporter::saveMaterialToMetaData(crcFileName, material, fst);
				}
			}
		}
	}

	for(std::vector<object3d*>::iterator it = obj->getChildList()->begin(); it != obj->getChildList()->end(); ++it)
	{
		object3d* child = *it;
		saveMaterialRecursiveToMeta(child);
	}
}